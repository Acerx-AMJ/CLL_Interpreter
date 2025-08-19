#include "interpreter.hpp"
#include "fmt.hpp"

Interpreter::Interpreter(Program& program, Environment& environment)
   : program(program), environment(environment) {}

Value Interpreter::evaluate() {
   Value last;
   for (auto& ast : program.statements) {
      last = evaluate_stmt(std::move(ast));
   }
   return std::move(last);
}

// Evaluate statement functions

Value Interpreter::evaluate_stmt(Stmt stmt) {
   switch (stmt->type) {
   case StmtType::var_decl:
      return evaluate_var_decl(std::move(stmt));
   case StmtType::del:
      return evaluate_del_stmt(std::move(stmt));
   case StmtType::program:
      return evaluate_scope(std::move(stmt));
   default:
      return evaluate_expr(std::move(stmt));
   }
}

Value Interpreter::evaluate_var_decl(Stmt stmt) {
   auto& decl = get_stmt<VarDeclaration>(stmt);
   size_t isize = decl.identifiers.size(), vsize = decl.values.size();
   
   bool single_decl = (vsize == 1 && isize != 1);
   Value first (single_decl ? evaluate_stmt(decl.values.at(0)->copy()) : NullValue::make());

   for (int i = 0; i < isize; ++i) {
      Value value = (single_decl || (vsize != isize && i >= vsize) ? first->copy() : evaluate_stmt(std::move(decl.values.at(i))));
      environment.declare_variable(get_stmt<IdentLiteral>(decl.identifiers.at(i)).identifier, std::move(value), decl.constant);
   }
   return NullValue::make();
}

Value Interpreter::evaluate_del_stmt(Stmt stmt) {
   auto& del = get_stmt<DeleteStmt>(stmt);
   for (const auto& identifier : del.identifiers) {
      environment.delete_variable(get_stmt<IdentLiteral>(identifier).identifier);
   }
   return NullValue::make();
}

Value Interpreter::evaluate_scope(Stmt expr) {
   auto& program = get_stmt<Program>(expr);
   Environment env (&environment);
   Interpreter interpreter (program, env);
   return interpreter.evaluate();
}

// Evaluate expression functions

Value Interpreter::evaluate_expr(Stmt expr) {
   switch (expr->type) {
   case StmtType::args:
      fmt::raise("Unexpected argument list while evaluating.");
   case StmtType::assignment:
      return evaluate_assignment(std::move(expr));
   case StmtType::ternary:
      return evaluate_ternary_expr(std::move(expr));
   case StmtType::binary:
      return evaluate_binary_expr(std::move(expr));
   case StmtType::unary:
      return evaluate_unary_expr(std::move(expr));
   case StmtType::call:
      return evaluate_call_expr(std::move(expr));
   default:
      return evaluate_primary_expr(std::move(expr));
   }
}

Value Interpreter::evaluate_ternary_expr(Stmt expr) {
   auto& ternary = get_stmt<TernaryExpr>(expr);
   auto left = evaluate_stmt(std::move(ternary.left));
   return std::move(evaluate_stmt(std::move((left->as_bool() ? ternary.middle : ternary.right))));
}

Value Interpreter::evaluate_binary_expr(Stmt expr) {
   auto& binary = get_stmt<BinaryExpr>(expr);
   auto left = evaluate_stmt(std::move(binary.left));

   if (binary.op == Type::binary_cond) {
      return (left->as_bool() ? std::move(evaluate_stmt(std::move(binary.right))) : NullValue::make());
   }

   auto right = evaluate_stmt(std::move(binary.right));

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
   case Type::log_and:
      return BoolValue::make(left->as_bool() && right->as_bool());
   case Type::log_or:
      return BoolValue::make(left->as_bool() || right->as_bool());
   case Type::divisible:
      return BoolValue::make(!left->remainder(right)->as_bool());
   case Type::equals:
      return BoolValue::make(left->equal(right));
   case Type::really_equals:
      return BoolValue::make(left->type == right->type && left->equal(right));
   case Type::not_equals:
      return BoolValue::make(!left->equal(right));
   case Type::really_not_equals:
      return BoolValue::make(left->type != right->type || !left->equal(right));
   case Type::greater:
      return BoolValue::make(left->greater(right, ">"));
   case Type::greater_equal:
      return BoolValue::make(!right->greater(left, ">="));
   case Type::smaller:
      return BoolValue::make(right->greater(left, "<"));
   case Type::smaller_equal:
      return BoolValue::make(!left->greater(right, "<="));
   default:
      fmt::raise("Unsupported binary command '{}'.", type_str[int(binary.op)]);
   }
}

Value Interpreter::evaluate_unary_expr(Stmt expr) {
   auto& unary = get_stmt<UnaryExpr>(expr);

   switch (unary.op) {
   case Type::plus: {
      auto value = evaluate_stmt(std::move(unary.value));
      return std::move(value);
   }
   case Type::minus: {
      auto value = evaluate_stmt(std::move(unary.value));
      return std::move(value->negate());
   }
   case Type::increment: {
      if (unary.value->type == StmtType::identifier) {
         auto& ident = get_stmt<IdentLiteral>(unary.value);
         auto value = evaluate_stmt(ident.copy())->increment();
         environment.assign_variable(ident.identifier, value->copy());
         return std::move(value);
      }
      auto value = evaluate_stmt(std::move(unary.value));
      return std::move(value->increment());
   }
   case Type::decrement: {
      if (unary.value->type == StmtType::identifier) {
         auto& ident = get_stmt<IdentLiteral>(unary.value);
         auto value = evaluate_stmt(ident.copy())->decrement();
         environment.assign_variable(ident.identifier, value->copy());
         return std::move(value);
      }
      auto value = evaluate_stmt(std::move(unary.value));
      return std::move(value->decrement());
   }
   case Type::log_not: {
      auto value = evaluate_stmt(std::move(unary.value));
      return BoolValue::make(!value->as_bool());
   }
   default:
      fmt::raise("Unsupported unary command '{}'.", type_str[int(unary.op)]);
   }
}

Value Interpreter::evaluate_assignment(Stmt expr) {
   auto& assignment = get_stmt<AssignmentExpr>(expr);
   fmt::raise_if(assignment.left->type != StmtType::identifier, "Expected an 'IdentifierLiteral' at the left side of the '{}' operator, got '{}' instead at line {}.", type_str[int(assignment.op)], stmt_type_str[int(assignment.left->type)], assignment.left->line);
   
   auto identifier = get_stmt<IdentLiteral>(assignment.left).identifier;
   auto value = evaluate_stmt(std::move(assignment.right));

   switch (assignment.op) {
   case Type::assign:
      break;
   case Type::plus_eq:
      value = environment.get_variable(identifier)->add(value);
      break;
   case Type::minus_eq:
      value = environment.get_variable(identifier)->subtract(value);
      break;
   case Type::multiply_eq:
      value = environment.get_variable(identifier)->multiply(value);
      break;
   case Type::divide_eq:
      value = environment.get_variable(identifier)->divide(value);
      break;
   case Type::remainder_eq:
      value = environment.get_variable(identifier)->remainder(value);
      break;
   case Type::exponentiate_eq:
      value = environment.get_variable(identifier)->exponentiate(value);
      break;
   default:
      fmt::raise("Unsupported assignment command '{}'.", type_str[int(assignment.op)]);
   };

   environment.assign_variable(identifier, value->copy());
   return std::move(value);
}

Value Interpreter::evaluate_call_expr(Stmt expr) {
   auto& call = get_stmt<CallExpr>(expr);
   std::vector<Value> args;
   for (auto& arg : get_stmt<ArgsListExpr>(call.args).args)
      args.push_back(std::move(evaluate_stmt(std::move(arg))));

   return std::move(environment.call_function(get_stmt<IdentLiteral>(call.identifier).identifier, args));
}

Value Interpreter::evaluate_primary_expr(Stmt expr) {
   switch (expr->type) {
   case StmtType::identifier: {
      Value ident = IdentValue::make(get_stmt<IdentLiteral>(expr).identifier);
      while (ident->type == ValueType::identifier) {
         ident = std::move(environment.get_variable(get_value<IdentValue>(ident).identifier));
      }
      return std::move(ident);
   }
   case StmtType::number:
      return NumberValue::make(get_stmt<NumberLiteral>(expr).number);
   case StmtType::character:
      return CharValue::make(get_stmt<CharLiteral>(expr).ch);
   case StmtType::string:
      return StringValue::make(get_stmt<StringLiteral>(expr).string);
   case StmtType::null:
      return NullValue::make();
   case StmtType::program:
      return evaluate_scope(std::move(expr));
   default:
      fmt::raise("Unexpected expression while evaluating.");
   }
}
