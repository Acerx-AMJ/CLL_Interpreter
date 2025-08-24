#include "interpreter.hpp"

#include "fmt.hpp"

Value Interpreter::evaluate(Program& program, Environment& env) {
   Value last;
   int id = ++fn_counter;

   for (auto& ast : program.statements) {
      last = evaluate_stmt(env, std::move(ast));

      if (!return_stack.empty() && return_stack.top() >= id) {
         while (!return_stack.empty()) {
            return_stack.pop();
         }
         return std::move(last);
      }
   }
   return std::move(last);
}

// Evaluate statement functions

Value Interpreter::evaluate_stmt(Environment& env, Stmt stmt) {
   switch (stmt->type) {
   case StmtType::var_decl:
      return evaluate_var_decl(env, std::move(stmt));
   case StmtType::fn_decl:
      return evaluate_fn_decl(env, std::move(stmt));
   case StmtType::del:
      return evaluate_del_stmt(env, std::move(stmt));
   case StmtType::exists:
      return evaluate_exists_stmt(env, std::move(stmt));
   case StmtType::ifelse:
      return evaluate_if_else_stmt(env, std::move(stmt));
   case StmtType::while_loop:
      return evaluate_while_loop(env, std::move(stmt));
   case StmtType::for_loop:
      return evaluate_for_loop(env, std::move(stmt));
   case StmtType::break_stmt:
      should_break = true;
      fmt::raise_if(stmt->line, loop_stack.empty(), "'BreakStatement' outside of a loop.");
      return NullValue::make(stmt->line);
   case StmtType::continue_stmt:
      should_continue = true;
      fmt::raise_if(stmt->line, loop_stack.empty(), "'ContinueStatement' outside of a loop.");
      return NullValue::make(stmt->line);
   case StmtType::return_stmt:
      return_stack.push(fn_counter);
      fmt::raise_if(stmt->line, fn_stack.empty(), "'ReturnStatement' outside of a function.");
      return evaluate_stmt(env, std::move(get_stmt<ReturnStmt>(stmt).value));
   case StmtType::unless_stmt:
      return evaluate_unless_stmt(env, std::move(stmt));
   default:
      return evaluate_expr(env, std::move(stmt));
   }
}

Value Interpreter::evaluate_var_decl(Environment& env, Stmt stmt) {
   auto& decl = get_stmt<VarDeclaration>(stmt);
   size_t isize = decl.identifiers.size(), vsize = decl.values.size();
   
   bool single_decl = (vsize == 1 && isize != 1);
   Value first (single_decl ? evaluate_stmt(env, decl.values.at(0)->copy()) : NullValue::make());

   for (int i = 0; i < isize; ++i) {
      Value value = (single_decl || (vsize != isize && i >= vsize) ? first->copy() : evaluate_stmt(env, std::move(decl.values.at(i))));
      env.declare_variable(get_stmt<IdentLiteral>(decl.identifiers.at(i)).identifier, std::move(value), decl.constant, decl.line);
   }
   return NullValue::make(decl.line);
}

Value Interpreter::evaluate_fn_decl(Environment& env, Stmt stmt) {
   auto& decl = get_stmt<FnDeclaration>(stmt);
   auto identifier = get_stmt<IdentLiteral>(decl.identifier);

   std::vector<std::string> parameters;
   for (const auto& arg : decl.arguments) {
      fmt::raise_if(arg->line, arg->type != StmtType::identifier, "Expected 'IdentifierLiteral', got '{}' instead.", stmt_type_str[int(arg->type)]);
      parameters.push_back(get_stmt<IdentLiteral>(arg).identifier);
   }

   std::vector<Value> parameter_def;
   for (auto& param_def : decl.argument_def) {
      parameter_def.push_back(evaluate_stmt(env, std::move(param_def)));
   }

   std::string returns;
   if (decl.returns->type == StmtType::identifier) {
      returns = get_stmt<IdentLiteral>(decl.returns).identifier;
   }

   Value return_def = NullValue::make(decl.line);
   if (decl.return_def->type != StmtType::null) {
      return_def = evaluate_stmt(env, std::move(decl.return_def));
   }

   auto func = Function::make(identifier.identifier, std::move(parameters), std::move(parameter_def), returns, std::move(return_def), &env, std::move(decl.body), decl.def_args, decl.line);

   env.declare_variable(identifier.identifier, std::move(func), true, decl.line);
   return NullValue::make(decl.line);
}

Value Interpreter::evaluate_del_stmt(Environment& env, Stmt stmt) {
   auto& del = get_stmt<DeleteStmt>(stmt);
   for (const auto& identifier : del.identifiers) {
      env.delete_variable(get_stmt<IdentLiteral>(identifier).identifier, del.line);
   }
   return NullValue::make(del.line);
}

Value Interpreter::evaluate_exists_stmt(Environment& env, Stmt stmt) {
   auto& exists = get_stmt<ExistsStmt>(stmt);
   auto& identifier = get_stmt<IdentLiteral>(exists.identifier);
   return BoolValue::make(env.variable_exists(identifier.identifier), exists.line);
}

Value Interpreter::evaluate_if_else_stmt(Environment& env, Stmt stmt) {
   auto& ifelse = get_stmt<IfElseStmt>(stmt);
   auto& ifclause = get_stmt<IfClauseStmt>(ifelse.ifclause);
   
   if (evaluate_stmt(env, std::move(ifclause.expr))->as_bool()) {
      return evaluate_stmt(env, std::move(ifclause.stmt));
   }

   for (auto& elif : ifelse.elifclauses) {
      auto& elifclause = get_stmt<IfClauseStmt>(elif);
      if (evaluate_stmt(env, std::move(elifclause.expr))->as_bool()) {
         return evaluate_stmt(env, std::move(elifclause.stmt));
      }
   }

   if (ifelse.elseclause.has_value()) {
      auto& elseclause = get_stmt<IfClauseStmt>(ifelse.elseclause.value());
      return evaluate_stmt(env, std::move(elseclause.stmt));
   }
   return NullValue::make(ifelse.line);
}

Value Interpreter::evaluate_while_loop(Environment& env, Stmt stmt) {
   auto& while_stmt = get_stmt<WhileStmt>(stmt);
   Value result;
   loop_stack.push(1);

   while (true) {
      if (!while_stmt.infinite && !evaluate_stmt(env, while_stmt.expr->copy())->as_bool()) {
         loop_stack.pop();
         return std::move(result);
      }
      result = evaluate_stmt(env, while_stmt.stmt->copy());

      if (should_break) {
         should_break = false;
         loop_stack.pop();
         return std::move(result);
      }

      if (should_continue) {
         should_continue = false;
      }
   }
}

Value Interpreter::evaluate_for_loop(Environment& env, Stmt stmt) {
   auto& for_stmt = get_stmt<ForStmt>(stmt);
   Value result;
   loop_stack.push(1);

   Environment new_env (&env);
   if (for_stmt.initexpr.has_value()) {
      evaluate_stmt(new_env, std::move(for_stmt.initexpr.value()));
   }
   
   while (true) {
      if (for_stmt.condition.has_value() && !evaluate_stmt(new_env, for_stmt.condition.value()->copy())->as_bool()) {
         loop_stack.pop();
         return std::move(result);
      }
      result = evaluate(static_cast<Program&>(*for_stmt.stmt->copy().get()), new_env);

      if (should_break) {
         should_break = false;
         loop_stack.pop();
         return std::move(result);
      }

      if (should_continue) {
         should_continue = false;
      }

      if (for_stmt.loopexpr.has_value()) {
         evaluate_stmt(new_env, for_stmt.loopexpr.value()->copy());  
      }
   }
}

Value Interpreter::evaluate_unless_stmt(Environment& env, Stmt stmt) {
   auto& unless = get_stmt<UnlessStmt>(stmt);
   if (!evaluate_stmt(env, std::move(unless.expr))->as_bool()) {
      return evaluate_stmt(env, std::move(unless.stmt));
   }
   return NullValue::make(unless.line);
}

// Evaluate expression functions

Value Interpreter::evaluate_expr(Environment& env, Stmt expr) {
   switch (expr->type) {
   case StmtType::args:
      fmt::raise(expr->line, "Unexpected argument list while evaluating.");
   case StmtType::assignment:
      return evaluate_assignment(env, std::move(expr));
   case StmtType::ternary:
      return evaluate_ternary_expr(env, std::move(expr));
   case StmtType::binary:
      return evaluate_binary_expr(env, std::move(expr));
   case StmtType::unary:
      return evaluate_unary_expr(env, std::move(expr));
   case StmtType::call:
      return evaluate_call_expr(env, std::move(expr));
   default:
      return evaluate_primary_expr(env, std::move(expr));
   }
}

Value Interpreter::evaluate_ternary_expr(Environment& env, Stmt expr) {
   auto& ternary = get_stmt<TernaryExpr>(expr);
   auto left = evaluate_stmt(env, std::move(ternary.left));
   return std::move(evaluate_stmt(env, std::move((left->as_bool() ? ternary.middle : ternary.right))));
}

Value Interpreter::evaluate_binary_expr(Environment& env, Stmt expr) {
   auto& binary = get_stmt<BinaryExpr>(expr);
   auto left = evaluate_stmt(env, std::move(binary.left));

   // Binary expressions dependant on right side value not being parsed
   if (binary.op == Type::binary_cond) {
      return (left->type == ValueType::null ? std::move(evaluate_stmt(env, std::move(binary.right))) : std::move(left));
   } else if (binary.op == Type::log_and) {
      // If left is false, do not evaluate right
      return BoolValue::make((!left->as_bool() ? false : evaluate_stmt(env, std::move(binary.right))->as_bool()), binary.line);
   } else if (binary.op == Type::log_or) {
      // If left is true, do not evaluate right
      return BoolValue::make((left->as_bool() ? true : evaluate_stmt(env, std::move(binary.right))->as_bool()), binary.line);
   }

   left->line = binary.line;
   auto right = evaluate_stmt(env, std::move(binary.right));

   switch (binary.op) {
   case Type::plus:
      return std::move(left->add(right));
   case Type::minus:
      return std::move(left->subtract(right));
   case Type::multiply:
      return std::move(left->multiply(right));
   case Type::divide:
      return std::move(left->divide(right));
   case Type::remainder:
      return std::move(left->remainder(right));
   case Type::exponentiate:
      return std::move(left->exponentiate(right));
   case Type::divisible:
      return BoolValue::make(!left->remainder(right)->as_bool(), binary.line);
   case Type::equals:
      return BoolValue::make(left->equal(right), binary.line);
   case Type::really_equals:
      return BoolValue::make(left->type == right->type && left->equal(right), binary.line);
   case Type::not_equals:
      return BoolValue::make(!left->equal(right), binary.line);
   case Type::really_not_equals:
      return BoolValue::make(left->type != right->type || !left->equal(right), binary.line);
   case Type::greater:
      return BoolValue::make(left->greater(right, ">"), binary.line);
   case Type::greater_equal:
      return BoolValue::make(!right->greater(left, ">="), binary.line);
   case Type::smaller:
      return BoolValue::make(right->greater(left, "<"), binary.line);
   case Type::smaller_equal:
      return BoolValue::make(!left->greater(right, "<="), binary.line);
   default:
      fmt::raise(binary.line, "Unsupported binary command '{}'.", type_str[int(binary.op)]);
   }
}

Value Interpreter::evaluate_unary_expr(Environment& env, Stmt expr) {
   auto& unary = get_stmt<UnaryExpr>(expr);

   switch (unary.op) {
   case Type::plus: {
      auto value = evaluate_stmt(env, std::move(unary.value));
      return std::move(value);
   }
   case Type::minus: {
      auto value = evaluate_stmt(env, std::move(unary.value));
      return std::move(value->negate());
   }
   case Type::increment: {
      if (unary.value->type == StmtType::identifier) {
         auto& ident = get_stmt<IdentLiteral>(unary.value);
         auto value = evaluate_stmt(env, ident.copy())->increment();
         env.assign_variable(ident.identifier, value->copy(), unary.line);
         return std::move(value);
      }
      auto value = evaluate_stmt(env, std::move(unary.value));
      return std::move(value->increment());
   }
   case Type::decrement: {
      if (unary.value->type == StmtType::identifier) {
         auto& ident = get_stmt<IdentLiteral>(unary.value);
         auto value = evaluate_stmt(env, ident.copy())->decrement();
         env.assign_variable(ident.identifier, value->copy(), unary.line);
         return std::move(value);
      }
      auto value = evaluate_stmt(env, std::move(unary.value));
      return std::move(value->decrement());
   }
   case Type::log_not: {
      auto value = evaluate_stmt(env, std::move(unary.value));
      return BoolValue::make(!value->as_bool(), value->line);
   }
   default:
      fmt::raise(unary.line, "Unsupported unary command '{}'.", type_str[int(unary.op)]);
   }
}

Value Interpreter::evaluate_assignment(Environment& env, Stmt expr) {
   auto& assignment = get_stmt<AssignmentExpr>(expr);
   fmt::raise_if(assignment.left->line, assignment.left->type != StmtType::identifier, "Expected an 'IdentifierLiteral' at the left side of the '{}' operator, got '{}'.", type_str[int(assignment.op)], stmt_type_str[int(assignment.left->type)]);
   
   auto identifier = get_stmt<IdentLiteral>(assignment.left).identifier;
   auto value = evaluate_stmt(env, std::move(assignment.right));

   switch (assignment.op) {
   case Type::assign:
      break;
   case Type::plus_eq:
      value = env.get_variable(identifier, assignment.line)->add(value);
      break;
   case Type::minus_eq:
      value = env.get_variable(identifier, assignment.line)->subtract(value);
      break;
   case Type::multiply_eq:
      value = env.get_variable(identifier, assignment.line)->multiply(value);
      break;
   case Type::divide_eq:
      value = env.get_variable(identifier, assignment.line)->divide(value);
      break;
   case Type::remainder_eq:
      value = env.get_variable(identifier, assignment.line)->remainder(value);
      break;
   case Type::exponentiate_eq:
      value = env.get_variable(identifier, assignment.line)->exponentiate(value);
      break;
   default:
      fmt::raise(assignment.left->line, "Unsupported assignment command '{}'.", type_str[int(assignment.op)]);
   };

   env.assign_variable(identifier, value->copy(), assignment.line);
   return std::move(value);
}

Value Interpreter::evaluate_call_expr(Environment& env, Stmt expr) {
   auto& call = get_stmt<CallExpr>(expr);
   std::vector<Value> args;
   for (auto& arg : get_stmt<ArgsListExpr>(call.args).args)
      args.push_back(std::move(evaluate_stmt(env, std::move(arg))));

   if (call.identifier->type != StmtType::identifier) {
      return call_function(env, evaluate_call_expr(env, std::move(call.identifier)), args, call.line);
   } else {
      return call_function(env, env.get_variable(get_stmt<IdentLiteral>(call.identifier).identifier, call.line), args, call.line);
   }
}

Value Interpreter::evaluate_primary_expr(Environment& env, Stmt expr) {
   switch (expr->type) {
   case StmtType::identifier: {
      Value ident = IdentValue::make(get_stmt<IdentLiteral>(expr).identifier, expr->line);
      while (ident->type == ValueType::identifier) {
         ident = std::move(env.get_variable(get_value<IdentValue>(ident).identifier, ident->line));
      }
      return std::move(ident);
   }
   case StmtType::number:
      return NumberValue::make(get_stmt<NumberLiteral>(expr).number, expr->line);
   case StmtType::character:
      return CharValue::make(get_stmt<CharLiteral>(expr).ch, expr->line);
   case StmtType::string:
      return StringValue::make(get_stmt<StringLiteral>(expr).string, expr->line);
   case StmtType::null:
      return NullValue::make(expr->line);
   case StmtType::program: {
      Environment new_env (&env);
      return evaluate(get_stmt<Program>(expr), new_env);
   }
   default:
      fmt::raise(expr->line, "Unexpected expression while evaluating: '{}'.", stmt_type_str[int(expr->type)]);
   }
}

Value Interpreter::call_function(Environment& env, Value func, std::vector<Value>& args, int line) {
   if (func->type == ValueType::native_fn) {
      auto& native = get_value<NativeFn>(func);
      return native.call(args, line);
   } else if (func->type == ValueType::fn) {
      auto& fn = get_value<Function>(func);
      fmt::raise_if(line, args.size() > fn.parameters.size() || args.size() < fn.parameters.size() - fn.def_args, "Expected 'CallExpression' argument count to match function declaration parameter count. {} != {}.", args.size(), fn.parameters.size());
      fn_stack.push(1);

      Environment new_env (fn.env);
      int def_i = 0;
      for (int i = 0; i < fn.parameters.size(); ++i) {
         if (i < args.size()) {
            new_env.declare_variable(fn.parameters.at(i), std::move(args.at(i)), false, args.at(i)->line);
            if (i >= fn.parameters.size() - fn.def_args) {
               ++def_i;
            }
         } else {
            new_env.declare_variable(fn.parameters.at(i), fn.parameter_def.at(def_i)->copy(), false, fn.parameter_def.at(def_i)->line);
            ++def_i;
         }
      }

      if (!fn.returns.empty()) {
         new_env.declare_variable(fn.returns, fn.return_def->copy(), false, fn.line);
      }

      // Dirty solution to getting a copy of a program
      auto body_ptr = fn.body->copy();
      auto value = evaluate(static_cast<Program&>(*body_ptr.get()), new_env);

      fn_stack.pop();
      return std::move(value);
   } else {
      fmt::raise(line, "Attempted to call '{}', but only 'NativeFunction' and 'Function' are callable.", value_type_str[int(func->type)]);
   }
}
