#ifndef AST_HPP
#define AST_HPP

#include "tokens.hpp"
#include <memory>
#include <vector>

enum class StmtType : char {
   var_decl, del, assignment, binary, unary, call, args, identifier, number, character, string, null, program
};

constexpr std::string_view stmt_type_str[] {
   "VariableDeclaration", "DeleteStatement", "AssignmentExpression", "BinaryExpression", "UnaryExpression",
   "CallExpression", "ArgumentListExpression", "IdentifierLiteral", "NumberLiteral", "CharacterLiteral",
   "StringLiteral", "NullLiteral", "Program"
};

struct Statement;
using Stmt = std::unique_ptr<Statement>;

struct Statement {
   StmtType type;
   int line = 0;

   Statement(StmtType type, int line);
   virtual ~Statement() = default;
   virtual void print(int indentation) const = 0;
   virtual Stmt copy() const = 0;
};

struct VarDeclaration : public Statement {
   bool constant = false;
   std::vector<Stmt> identifiers;
   std::vector<Stmt> values;

   VarDeclaration(bool constant, std::vector<Stmt> identifiers, std::vector<Stmt> values, int line);
   void print(int indentation) const override;
   Stmt copy() const override;
};

struct DeleteStmt : public Statement {
   std::vector<Stmt> identifiers;

   DeleteStmt(std::vector<Stmt> identifiers, int line);
   void print(int indentation) const override;
   Stmt copy() const override;
};

struct AssignmentExpr : public Statement {
   Type op;
   Stmt left;
   Stmt right;

   AssignmentExpr(Type op, Stmt left, Stmt right, int line);
   void print(int indentation) const override;
   Stmt copy() const override;
};

struct BinaryExpr : public Statement {
   Type op;
   Stmt left;
   Stmt right;

   BinaryExpr(Type op, Stmt left, Stmt right, int line);
   void print(int indentation) const override;
   Stmt copy() const override;
};

struct UnaryExpr : public Statement {
   Type op;
   Stmt value;

   UnaryExpr(Type op, Stmt value, int line);
   void print(int indentation) const override;
   Stmt copy() const override;
};

struct CallExpr : public Statement {
   Stmt args;
   Stmt identifier;

   CallExpr(Stmt args, Stmt identifier, int line);
   void print(int indentation) const override;
   Stmt copy() const override;
};

struct ArgsListExpr : public Statement {
   std::vector<Stmt> args;

   ArgsListExpr(std::vector<Stmt> args, int line);
   void print(int indentation) const override;
   Stmt copy() const override;
};

struct IdentLiteral : public Statement {
   std::string identifier;

   IdentLiteral(const std::string& identifier, int line);
   void print(int indentation) const override;
   Stmt copy() const override;
};

struct NumberLiteral : public Statement {
   long double number;

   NumberLiteral(long double number, int line);
   void print(int indentation) const override;
   Stmt copy() const override;
};

struct CharLiteral : public Statement {
   char character;

   CharLiteral(char character, int line);
   void print(int indentation) const override;
   Stmt copy() const override;
};

struct StringLiteral : public Statement {
   std::string string;

   StringLiteral(const std::string& string, int line);
   void print(int indentation) const override;
   Stmt copy() const override;
};

struct NullLiteral : public Statement {
   NullLiteral(int line = 0);
   void print(int indentation) const override;
   Stmt copy() const override;
};

struct Program : public Statement {
   std::vector<Stmt> statements;

   Program(int line = 0);
   void print(int indentation) const override;
   Stmt copy() const override;
};

#endif
