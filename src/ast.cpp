#include "ast.hpp"

#include <iostream>

Statement::Statement(StmtType type, int line)
   : type(type), line(line) {}

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
   return std::make_unique<VarDeclaration>(constant, std::move(copied_identifiers), std::move(copied_values), line);
}

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
   return std::make_unique<DeleteStmt>(std::move(copied_identifiers), line);
}

AssignmentExpr::AssignmentExpr(Type op, Stmt left, Stmt right, int line)
   : op(op), left(std::move(left)), right(std::move(right)), Statement(StmtType::assignment, line) {}

void AssignmentExpr::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "Assignment Expression:\n";
   left->print(indentation + 2);
   std::cout << std::string(indentation, ' ') << "Operator: " << type_str[int(op)] << "\n";
   right->print(indentation + 2);
}

Stmt AssignmentExpr::copy() const {
   return std::make_unique<AssignmentExpr>(op, std::move(left->copy()), std::move(right->copy()), line);
}

BinaryExpr::BinaryExpr(Type op, Stmt left, Stmt right, int line)
   : op(op), left(std::move(left)), right(std::move(right)), Statement(StmtType::binary, line) {}

void BinaryExpr::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "Binary Expression:\n";
   left->print(indentation + 2);
   std::cout << std::string(indentation, ' ') << "Operator: " << type_str[int(op)] << "\n";
   right->print(indentation + 2);
}

Stmt BinaryExpr::copy() const {
   return std::make_unique<BinaryExpr>(op, std::move(left->copy()), std::move(right->copy()), line);
}

UnaryExpr::UnaryExpr(Type op, Stmt value, int line)
   : op(op), value(std::move(value)), Statement(StmtType::unary, line) {}

void UnaryExpr::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "Unary Expression:\n";
   std::cout << std::string(indentation, ' ') << "Operator: " << type_str[int(op)] << "\n";
   value->print(indentation + 2);
}

Stmt UnaryExpr::copy() const {
   return std::make_unique<UnaryExpr>(op, std::move(value->copy()), line);
}

CallExpr::CallExpr(Stmt args, Stmt identifier, int line)
   : args(std::move(args)), identifier(std::move(identifier)), Statement(StmtType::call, line) {}

void CallExpr::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "Call Expression:\n";
   identifier->print(indentation + 2);
   args->print(indentation + 2);
}

Stmt CallExpr::copy() const {
   return std::make_unique<CallExpr>(std::move(args->copy()), std::move(identifier->copy()), line);
}

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
   return std::make_unique<ArgsListExpr>(std::move(copied_args), line);
}

IdentLiteral::IdentLiteral(const std::string& identifier, int line)
   : identifier(identifier), Statement(StmtType::identifier, line) {}

void IdentLiteral::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "[" << identifier << "]\n";
}

Stmt IdentLiteral::copy() const {
   return std::make_unique<IdentLiteral>(identifier, line);
}

NumberLiteral::NumberLiteral(long double number, int line)
   : number(number), Statement(StmtType::number, line) {}

void NumberLiteral::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << number << '\n';
}

Stmt NumberLiteral::copy() const {
   return std::make_unique<NumberLiteral>(number, line);
}

CharLiteral::CharLiteral(char ch, int line)
   : ch(ch), Statement(StmtType::character, line) {}

void CharLiteral::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << '\'' << ch << "'\n";
}

Stmt CharLiteral::copy() const {
   return std::make_unique<CharLiteral>(ch, line);
}

StringLiteral::StringLiteral(const std::string& string, int line)
   : string(string), Statement(StmtType::string, line) {}

void StringLiteral::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << '"' << string << "\"\n";
}

Stmt StringLiteral::copy() const {
   return std::make_unique<StringLiteral>(string, line);
}

NullLiteral::NullLiteral(int line)
   : Statement(StmtType::null, line) {}

void NullLiteral::print(int indentation) const {
   std::cout << std::string(indentation, ' ') << "null\n";
}

Stmt NullLiteral::copy() const {
   return std::make_unique<NullLiteral>(line);
}

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
