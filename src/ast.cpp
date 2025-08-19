#include "ast.hpp"

#include <iostream>

Statement::Statement(StmtType type, int line)
   : type(type), line(line) {}

// Variable declaration

VarDeclaration::VarDeclaration(bool constant, std::vector<Stmt> identifiers, std::vector<Stmt> values, int line)
   : constant(constant), identifiers(std::move(identifiers)), values(std::move(values)), Statement(StmtType::var_decl, line) {}

void VarDeclaration::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << (constant ? "Constant" : "Variable") << " Declaration:\n";
   for (size_t i = 0; i < identifiers.size(); ++i) {
      identifiers.at(i)->print(indentation);

      if (values.size() != 1 && i >= values.size()) {
         std::cout << std::string(indentation + 2, ' ') << "null\n";
      } else {
         values.at(std::min(i, values.size() - 1))->print(indentation + 2);
      }
   }
}

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

// Delete statement

DeleteStmt::DeleteStmt(std::vector<Stmt> identifiers, int line)
   : identifiers(std::move(identifiers)), Statement(StmtType::del, line) {}

void DeleteStmt::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "Delete Statement:\n";
   for (const auto& identifier : identifiers) {
      identifier->print(indentation + 2);
   }
}

Stmt DeleteStmt::copy() const {
   std::vector<Stmt> copied_identifiers;
   for (const auto& identifier : identifiers) {
      copied_identifiers.push_back(std::move(identifier->copy()));
   }
   return DeleteStmt::make(std::move(copied_identifiers), line);
}

// Assignment expression

AssignmentExpr::AssignmentExpr(Type op, Stmt left, Stmt right, int line)
   : op(op), left(std::move(left)), right(std::move(right)), Statement(StmtType::assignment, line) {}

void AssignmentExpr::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "Assignment Expression:\n";
   left->print(indentation + 2);
   std::cout << std::string(indentation, ' ') << "Operator: " << type_str[int(op)] << "\n";
   right->print(indentation + 2);
}

Stmt AssignmentExpr::copy() const {
   return AssignmentExpr::make(op, std::move(left->copy()), std::move(right->copy()), line);
}

// Ternary expression

TernaryExpr::TernaryExpr(Stmt left, Stmt middle, Stmt right, int line)
   : left(std::move(left)), middle(std::move(middle)), right(std::move(right)), Statement(StmtType::ternary, line) {}

void TernaryExpr::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "Ternary Expression:\n";
   left->print(indentation + 2);
   std::cout << std::string(indentation, ' ') << "Operator: ?\n";
   middle->print(indentation + 2);
   std::cout << std::string(indentation, ' ') << "Operator: :\n";
   right->print(indentation + 2);
}

Stmt TernaryExpr::copy() const {
   return TernaryExpr::make(std::move(left->copy()), std::move(middle->copy()), std::move(right->copy()), line);
}

// Binary expression

BinaryExpr::BinaryExpr(Type op, Stmt left, Stmt right, int line)
   : op(op), left(std::move(left)), right(std::move(right)), Statement(StmtType::binary, line) {}

void BinaryExpr::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "Binary Expression:\n";
   left->print(indentation + 2);
   std::cout << std::string(indentation, ' ') << "Operator: " << type_str[int(op)] << "\n";
   right->print(indentation + 2);
}

Stmt BinaryExpr::copy() const {
   return BinaryExpr::make(op, std::move(left->copy()), std::move(right->copy()), line);
}

// Unary expression

UnaryExpr::UnaryExpr(Type op, Stmt value, int line)
   : op(op), value(std::move(value)), Statement(StmtType::unary, line) {}

void UnaryExpr::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "Unary Expression:\n";
   std::cout << std::string(indentation, ' ') << "Operator: " << type_str[int(op)] << "\n";
   value->print(indentation + 2);
}

Stmt UnaryExpr::copy() const {
   return UnaryExpr::make(op, std::move(value->copy()), line);
}

// Call expression

CallExpr::CallExpr(Stmt args, Stmt identifier, int line)
   : args(std::move(args)), identifier(std::move(identifier)), Statement(StmtType::call, line) {}

void CallExpr::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "Call Expression:\n";
   identifier->print(indentation + 2);
   args->print(indentation + 2);
}

Stmt CallExpr::copy() const {
   return CallExpr::make(std::move(args->copy()), std::move(identifier->copy()), line);
}

// Argument list expression

ArgsListExpr::ArgsListExpr(std::vector<Stmt> args, int line)
   : args(std::move(args)), Statement(StmtType::args, line) {}

void ArgsListExpr::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "Argument List:\n";
   for (const auto& arg : args) {
      arg->print(indentation + 2);
   }
}

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

void IdentLiteral::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "[" << identifier << "]\n";
}

Stmt IdentLiteral::copy() const {
   return IdentLiteral::make(identifier, line);
}

// Number literal

NumberLiteral::NumberLiteral(long double number, int line)
   : number(number), Statement(StmtType::number, line) {}

void NumberLiteral::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << number << '\n';
}

Stmt NumberLiteral::copy() const {
   return NumberLiteral::make(number, line);
}

// Character literal

CharLiteral::CharLiteral(char ch, int line)
   : ch(ch), Statement(StmtType::character, line) {}

void CharLiteral::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << '\'' << ch << "'\n";
}

Stmt CharLiteral::copy() const {
   return CharLiteral::make(ch, line);
}

// String literal

StringLiteral::StringLiteral(const std::string& string, int line)
   : string(string), Statement(StmtType::string, line) {}

void StringLiteral::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << '"' << string << "\"\n";
}

Stmt StringLiteral::copy() const {
   return StringLiteral::make(string, line);
}

// Null literal

NullLiteral::NullLiteral(int line)
   : Statement(StmtType::null, line) {}

void NullLiteral::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "null\n";
}

Stmt NullLiteral::copy() const {
   return NullLiteral::make(line);
}

// Program

Program::Program(int line)
   : Statement(StmtType::program, line) {}

void Program::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "Scope:\n";
   for (const auto& stmt : statements) {
      stmt->print(indentation + 2);
   }
}

Stmt Program::copy() const {
   auto copied = std::make_unique<Program>(line);
   for (const auto& statement : statements) {
      copied->statements.push_back(statement->copy());
   }
   return std::move(copied);
}
