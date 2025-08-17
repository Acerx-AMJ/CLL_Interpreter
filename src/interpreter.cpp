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
   auto& decl = static_cast<VarDeclaration&>(*stmt.get());
   size_t isize = decl.identifiers.size(), vsize = decl.values.size();
   
   bool single_decl = (vsize == 1 && isize != 1);
   Value first (single_decl ? evaluate_expr(decl.values.at(0)->copy()) : std::make_unique<NullValue>());

   for (int i = 0; i < isize; ++i) {
      Value value = (single_decl || (vsize != isize && i >= vsize) ? first->copy() : evaluate_expr(std::move(decl.values.at(i))));
      environment.declare_variable(static_cast<IdentLiteral&>(*decl.identifiers.at(i).get()).identifier, std::move(value), decl.constant);
   }
   return std::make_unique<NullValue>();
}

Value Interpreter::evaluate_del_stmt(Stmt stmt) {
   auto& del = static_cast<DeleteStmt&>(*stmt.get());
   for (const auto& identifier : del.identifiers) {
      environment.delete_variable(static_cast<IdentLiteral&>(*identifier.get()).identifier);
   }
   return std::make_unique<NullValue>();
}

Value Interpreter::evaluate_scope(Stmt expr) {
   auto& program = static_cast<Program&>(*expr.get());
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
   case StmtType::binary:
      return evaluate_binary_expr(std::move(expr));
   case StmtType::call:
      return evaluate_call_expr(std::move(expr));
   case StmtType::unary:
      return evaluate_unary_expr(std::move(expr));
   default:
      return evaluate_primary_expr(std::move(expr));
   }
}

Value Interpreter::evaluate_unary_expr(Stmt expr) {
   auto& unary = static_cast<UnaryExpr&>(*expr.get());
   auto value = evaluate_expr(std::move(unary.value));

   switch (unary.op) {
   case Type::plus:
      return std::move(value);
   case Type::minus:
      return std::move(value->negate());
   case Type::increment:
      return std::move(value->increment());
   case Type::decrement:
      return std::move(value->decrement());
   default:
      fmt::raise("Unsupported unary command '{}'.", type_str[int(unary.op)]);
   }
}

Value Interpreter::evaluate_binary_expr(Stmt expr) {
   auto& binary = static_cast<BinaryExpr&>(*expr.get());
   auto left = evaluate_expr(std::move(binary.left));
   auto right = evaluate_expr(std::move(binary.right));

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
   default:
      fmt::raise("Unsupported binary command '{}'.", type_str[int(binary.op)]);
   }
}

Value Interpreter::evaluate_assignment(Stmt expr) {
   auto& assignment = static_cast<AssignmentExpr&>(*expr.get());
   fmt::raise_if(assignment.left->type != StmtType::identifier, "Expected an 'IdentifierLiteral' at the left side of the '{}' operator, got '{}' instead at line {}.", type_str[int(assignment.op)], stmt_type_str[int(assignment.left->type)], assignment.left->line);
   
   auto identifier = static_cast<IdentLiteral&>(*assignment.left.get()).identifier;
   auto value = evaluate_expr(std::move(assignment.right));

   switch (assignment.op) {
   case Type::equals:
      break;
   case Type::plus_equals:
      value = environment.get_variable(identifier)->add(value);
      break;
   case Type::minus_equals:
      value = environment.get_variable(identifier)->subtract(value);
      break;
   case Type::multiply_equals:
      value = environment.get_variable(identifier)->multiply(value);
      break;
   case Type::divide_equals:
      value = environment.get_variable(identifier)->divide(value);
      break;
   case Type::remainder_equals:
      value = environment.get_variable(identifier)->remainder(value);
      break;
   case Type::exponentiate_equals:
      value = environment.get_variable(identifier)->exponentiate(value);
      break;
   default:
      fmt::raise("Unsupported assignment command '{}'.", type_str[int(assignment.op)]);
   };

   environment.assign_variable(identifier, value->copy());
   return std::move(value);
}

Value Interpreter::evaluate_call_expr(Stmt expr) {
   auto& call = static_cast<CallExpr&>(*expr.get());
   std::vector<Value> args;
   for (auto& arg : static_cast<ArgsListExpr&>(*call.args.get()).args)
      args.push_back(std::move(evaluate_expr(std::move(arg))));

   return std::move(environment.call_function(static_cast<IdentLiteral&>(*call.identifier.get()).identifier, args));
}

Value Interpreter::evaluate_primary_expr(Stmt expr) {
   switch (expr->type) {
   case StmtType::identifier: {
      Value ident = std::make_unique<IdentifierValue>(static_cast<IdentLiteral&>(*expr.get()).identifier);
      while (ident->type == ValueType::identifier) {
         ident = std::move(environment.get_variable(static_cast<IdentifierValue&>(*ident.get()).identifier));
      }
      return std::move(ident);
   }
   case StmtType::number:
      return std::make_unique<NumberValue>(static_cast<NumberLiteral&>(*expr.get()).number);
   case StmtType::character:
      return std::make_unique<CharValue>(static_cast<CharLiteral&>(*expr.get()).character);
   case StmtType::string:
      return std::make_unique<StringValue>(static_cast<StringLiteral&>(*expr.get()).string);
   case StmtType::null:
      return std::make_unique<NullValue>();
   case StmtType::program:
      return evaluate_scope(std::move(expr));
   default:
      fmt::raise("Unexpected expression while evaluating.");
   }
}
