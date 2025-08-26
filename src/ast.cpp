#include "ast.hpp"

Statement::Statement(StmtType type, int line)
   : type(type), line(line) {}

// Variable declaration

VarDeclaration::VarDeclaration(bool constant, std::vector<Stmt> identifiers, std::vector<Stmt> values, int line)
   : constant(constant), identifiers(std::move(identifiers)), values(std::move(values)), Statement(StmtType::var_decl, line) {}

Stmt VarDeclaration::copy() const {
   std::vector<Stmt> copied_identifiers, copied_values;
   for (const auto& identifier : identifiers) {
      copied_identifiers.push_back(std::move(identifier->copy()));
   }
   
   for (const auto& value : values) {
      copied_values.push_back(std::move(value->copy()));
   }
   return VarDeclaration::make(constant, std::move(copied_identifiers), std::move(copied_values), line);
}

// Function declaration

FnDeclaration::FnDeclaration(Stmt identifier, std::vector<Stmt> arguments, std::vector<Stmt> argument_def, Stmt returns, Stmt return_def, Stmt body, int def_args, int line)
   : identifier(std::move(identifier)), arguments(std::move(arguments)), argument_def(std::move(argument_def)), returns(std::move(returns)), return_def(std::move(return_def)), body(std::move(body)), def_args(def_args), Statement(StmtType::fn_decl, line) {}

Stmt FnDeclaration::copy() const {
   std::vector<Stmt> copied_args, copied_arg_def;
   for (const auto& arg : arguments) {
      copied_args.push_back(arg->copy());
   }
   
   for (const auto& arg_def : argument_def) {
      copied_arg_def.push_back(arg_def->copy());
   }
   return FnDeclaration::make(identifier->copy(), std::move(copied_args), std::move(copied_arg_def), returns->copy(), return_def->copy(), body->copy(), def_args, line);
}

// Exists statement

ExistsStmt::ExistsStmt(Stmt identifier, int line)
   : identifier(std::move(identifier)), Statement(StmtType::exists, line) {}

Stmt ExistsStmt::copy() const {
   return ExistsStmt::make(identifier->copy(), line);
}

// Size of statement

SizeOfStmt::SizeOfStmt(Stmt stmt, int line)
   : stmt(std::move(stmt)), Statement(StmtType::sizeof_stmt, line) {}

Stmt SizeOfStmt::copy() const {
   return SizeOfStmt::make(stmt->copy(), line);
}

// Delete statement

DeleteStmt::DeleteStmt(std::vector<Stmt> identifiers, int line)
   : identifiers(std::move(identifiers)), Statement(StmtType::del, line) {}

Stmt DeleteStmt::copy() const {
   std::vector<Stmt> copied_identifiers;
   for (const auto& identifier : identifiers) {
      copied_identifiers.push_back(std::move(identifier->copy()));
   }
   return DeleteStmt::make(std::move(copied_identifiers), line);
}

// If else statement

IfElseStmt::IfElseStmt(Stmt ifclause, std::vector<Stmt> elifclauses, std::optional<Stmt> elseclause, int line)
   : ifclause(std::move(ifclause)), elifclauses(std::move(elifclauses)), elseclause(std::move(elseclause)), Statement(StmtType::ifelse, line) {}

Stmt IfElseStmt::copy() const {
   std::vector<Stmt> copied_elifclauses;
   for (const auto& elif : elifclauses) {
      copied_elifclauses.push_back(elif->copy());
   }

   if (elseclause.has_value()) {
      return IfElseStmt::make(ifclause->copy(), std::move(copied_elifclauses), elseclause.value()->copy(), line);
   } else {
      return IfElseStmt::make(ifclause->copy(), std::move(copied_elifclauses), line);
   }
}

// If clause statement

IfClauseStmt::IfClauseStmt(const std::string& keyword, Stmt expr, Stmt stmt, int line)
   : keyword(keyword), expr(std::move(expr)), stmt(std::move(stmt)), Statement(StmtType::if_clause, line) {}

Stmt IfClauseStmt::copy() const {
   return IfClauseStmt::make(keyword, expr->copy(), stmt->copy(), line);
}

// While loop statement

WhileStmt::WhileStmt(bool infinite, Stmt expr, Stmt stmt, int line)
   : infinite(infinite), expr(std::move(expr)), stmt(std::move(stmt)), Statement(StmtType::while_loop, line) {}

Stmt WhileStmt::copy() const {
   return WhileStmt::make(infinite, expr->copy(), stmt->copy(), line);
}

// For statement

ForStmt::ForStmt(std::optional<Stmt> initexpr, std::optional<Stmt> condition, std::optional<Stmt> loopexpr, Stmt stmt, int line)
   : initexpr(std::move(initexpr)), condition(std::move(condition)), loopexpr(std::move(loopexpr)), stmt(std::move(stmt)), Statement(StmtType::for_loop, line) {}

Stmt ForStmt::copy() const {
   return ForStmt::make(
      (initexpr.has_value() ? std::optional(initexpr.value()->copy()) : std::nullopt),
      (condition.has_value() ? std::optional(condition.value()->copy()) : std::nullopt),
      (loopexpr.has_value() ? std::optional(loopexpr.value()->copy()) : std::nullopt),
      stmt->copy(), line
   );
}

// Break statement

BreakStmt::BreakStmt(int line)
   : Statement(StmtType::break_stmt, line) {}

Stmt BreakStmt::copy() const {
   return BreakStmt::make(line);
}

// Continue statement

ContinueStmt::ContinueStmt(int line)
   : Statement(StmtType::continue_stmt, line) {}

Stmt ContinueStmt::copy() const {
   return ContinueStmt::make(line);
}

// Return statement

ReturnStmt::ReturnStmt(Stmt value, int line)
   : value(std::move(value)), Statement(StmtType::return_stmt, line) {}

Stmt ReturnStmt::copy() const {
   return ReturnStmt::make(value->copy(), line);
}

// Unless statement

UnlessStmt::UnlessStmt(Stmt expr, Stmt stmt, int line)
   : expr(std::move(expr)), stmt(std::move(stmt)), Statement(StmtType::unless_stmt, line) {}

Stmt UnlessStmt::copy() const {
   return UnlessStmt::make(expr->copy(), stmt->copy(), line);
}

// Assignment expression

AssignmentExpr::AssignmentExpr(Type op, Stmt left, Stmt right, int line)
   : op(op), left(std::move(left)), right(std::move(right)), Statement(StmtType::assignment, line) {}

Stmt AssignmentExpr::copy() const {
   return AssignmentExpr::make(op, std::move(left->copy()), std::move(right->copy()), line);
}

// Ternary expression

TernaryExpr::TernaryExpr(Stmt left, Stmt middle, Stmt right, int line)
   : left(std::move(left)), middle(std::move(middle)), right(std::move(right)), Statement(StmtType::ternary, line) {}

Stmt TernaryExpr::copy() const {
   return TernaryExpr::make(std::move(left->copy()), std::move(middle->copy()), std::move(right->copy()), line);
}

// Binary expression

BinaryExpr::BinaryExpr(Type op, Stmt left, Stmt right, int line)
   : op(op), left(std::move(left)), right(std::move(right)), Statement(StmtType::binary, line) {}

Stmt BinaryExpr::copy() const {
   return BinaryExpr::make(op, std::move(left->copy()), std::move(right->copy()), line);
}

// Unary expression

UnaryExpr::UnaryExpr(Type op, Stmt value, int line)
   : op(op), value(std::move(value)), Statement(StmtType::unary, line) {}

Stmt UnaryExpr::copy() const {
   return UnaryExpr::make(op, std::move(value->copy()), line);
}

// Call expression

CallExpr::CallExpr(Stmt args, Stmt identifier, int line)
   : args(std::move(args)), identifier(std::move(identifier)), Statement(StmtType::call, line) {}

Stmt CallExpr::copy() const {
   return CallExpr::make(std::move(args->copy()), std::move(identifier->copy()), line);
}

// Argument list expression

ArgsListExpr::ArgsListExpr(std::vector<Stmt> args, int line)
   : args(std::move(args)), Statement(StmtType::args, line) {}

Stmt ArgsListExpr::copy() const {
   std::vector<Stmt> copied_args;
   for (const auto& arg : args) {
      copied_args.push_back(std::move(arg->copy()));
   }
   return ArgsListExpr::make(std::move(copied_args), line);
}

// Identifier literal

IdentLiteral::IdentLiteral(const std::string& identifier, int line)
   : identifier(identifier), Statement(StmtType::identifier, line) {}

Stmt IdentLiteral::copy() const {
   return IdentLiteral::make(identifier, line);
}

// Number literal

NumberLiteral::NumberLiteral(long double number, int line)
   : number(number), Statement(StmtType::number, line) {}

Stmt NumberLiteral::copy() const {
   return NumberLiteral::make(number, line);
}

// Character literal

CharLiteral::CharLiteral(char ch, int line)
   : ch(ch), Statement(StmtType::character, line) {}

Stmt CharLiteral::copy() const {
   return CharLiteral::make(ch, line);
}

// String literal

StringLiteral::StringLiteral(const std::string& string, int line)
   : string(string), Statement(StmtType::string, line) {}

Stmt StringLiteral::copy() const {
   return StringLiteral::make(string, line);
}

// Array literal

ArrayLiteral::ArrayLiteral(std::vector<Stmt> array, int line)
   : array(std::move(array)), Statement(StmtType::array, line) {}

Stmt ArrayLiteral::copy() const {
   std::vector<Stmt> copied_array;
   for (const auto& element : array) {
      copied_array.push_back(element->copy());
   }
   return ArrayLiteral::make(std::move(copied_array), line);
}

// Null literal

NullLiteral::NullLiteral(int line)
   : Statement(StmtType::null, line) {}

Stmt NullLiteral::copy() const {
   return NullLiteral::make(line);
}

// Program

Program::Program(int line)
   : Statement(StmtType::program, line) {}

Stmt Program::copy() const {
   auto copied = std::make_unique<Program>(line);
   for (const auto& statement : statements) {
      copied->statements.push_back(statement->copy());
   }
   return std::move(copied);
}
