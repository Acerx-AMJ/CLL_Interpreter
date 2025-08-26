#ifndef AST_HPP
#define AST_HPP

#include "tokens.hpp"
#include <memory>
#include <optional>
#include <vector>

enum class StmtType : char {
   var_decl, fn_decl, exists, sizeof_stmt, del, ifelse, if_clause, while_loop, for_loop,
   break_stmt, continue_stmt, return_stmt, unless_stmt,
   assignment, ternary, binary, unary, member,
   call, args,
   identifier, number, character, string, array, null, program
};

constexpr std::string_view stmt_type_str[] {
   "VariableDeclaration", "FunctionDeclaration", "ExistsStatement", "SizeOfStatement", "DeleteStatement", "IfElseStatement", "IfClauseStatement", "WhileLoopStatement", "ForLoopStatement",
   "BreakStatement", "ContinueStatement", "ReturnStatement", "UnlessStatement",
   "AssignmentExpression", "TernaryExpression", "BinaryExpression", "UnaryExpression", "MemberAccess",
   "CallExpression", "ArgumentListExpression",
   "IdentifierLiteral", "NumberLiteral", "CharacterLiteral", "StringLiteral", "ArrayLiteral", "NullLiteral", "Program"
};

struct Statement;
using Stmt = std::unique_ptr<Statement>;

template<class T>
const T& get_stmt(const Stmt& stmt) {
   return static_cast<const T&>(*stmt.get());
}

template<class T>
T& get_stmt(Stmt& stmt) {
   return static_cast<T&>(*stmt.get());
}

struct Statement {
   StmtType type;
   int line = 0;

   Statement(StmtType type, int line);
   virtual ~Statement() = default;
   virtual Stmt copy() const = 0;
};

struct VarDeclaration : public Statement {
   bool constant = false;
   std::vector<Stmt> identifiers;
   std::vector<Stmt> values;

   VarDeclaration(bool constant, std::vector<Stmt> identifiers, std::vector<Stmt> values, int line);
   static Stmt make(bool constant, std::vector<Stmt> identifiers, std::vector<Stmt> values, int line) {
      return std::make_unique<VarDeclaration>(constant, std::move(identifiers), std::move(values), line);
   }
   Stmt copy() const override;
};

struct FnDeclaration : public Statement {
   Stmt identifier;
   std::vector<Stmt> arguments;
   std::vector<Stmt> argument_def;
   Stmt returns;
   Stmt return_def;
   Stmt body;
   int def_args;

   FnDeclaration(Stmt identifier, std::vector<Stmt> arguments, std::vector<Stmt> argument_def, Stmt returns, Stmt return_def, Stmt body, int def_args, int line);
   static Stmt make(Stmt identifier, std::vector<Stmt> arguments, std::vector<Stmt> argument_def, Stmt returns, Stmt return_def, Stmt body, int def_args, int line) {
      return std::make_unique<FnDeclaration>(std::move(identifier), std::move(arguments), std::move(argument_def), std::move(returns), std::move(return_def), std::move(body), def_args, line);
   }
   Stmt copy() const override;
};

struct ExistsStmt : public Statement {
   Stmt identifier;

   ExistsStmt(Stmt identifier, int line);
   static Stmt make(Stmt identifier, int line) {
      return std::make_unique<ExistsStmt>(std::move(identifier), line);
   }
   Stmt copy() const override;
};

struct SizeOfStmt : public Statement {
   Stmt stmt;

   SizeOfStmt(Stmt stmt, int line);
   static Stmt make(Stmt stmt, int line) {
      return std::make_unique<SizeOfStmt>(std::move(stmt), line);
   }
   Stmt copy() const override;
};

struct DeleteStmt : public Statement {
   std::vector<Stmt> identifiers;

   DeleteStmt(std::vector<Stmt> identifiers, int line);
   static Stmt make(std::vector<Stmt> identifiers, int line) {
      return std::make_unique<DeleteStmt>(std::move(identifiers), line);
   }
   Stmt copy() const override;
};

struct IfElseStmt : public Statement {
   Stmt ifclause;
   std::vector<Stmt> elifclauses;
   std::optional<Stmt> elseclause;

   IfElseStmt(Stmt ifclause, std::vector<Stmt> elifclauses, std::optional<Stmt> elseclause, int line);
   static Stmt make(Stmt ifclause, std::vector<Stmt> elifclauses, Stmt elseclause, int line) {
      return std::make_unique<IfElseStmt>(std::move(ifclause), std::move(elifclauses), std::move(elseclause), line);
   }
   static Stmt make(Stmt ifclause, std::vector<Stmt> elifclauses, int line) {
      return std::make_unique<IfElseStmt>(std::move(ifclause), std::move(elifclauses), std::nullopt, line);
   }
   Stmt copy() const override;
};

struct IfClauseStmt : public Statement {
   std::string keyword;
   Stmt expr;
   Stmt stmt;

   IfClauseStmt(const std::string& keyword, Stmt expr, Stmt stmt, int line);
   static Stmt make(const std::string& keyword, Stmt expr, Stmt stmt, int line) {
      return std::make_unique<IfClauseStmt>(keyword, std::move(expr), std::move(stmt), line);
   }
   Stmt copy() const override;
};

struct WhileStmt : public Statement {
   bool infinite;
   Stmt expr;
   Stmt stmt;

   WhileStmt(bool infinite, Stmt expr, Stmt stmt, int line);
   static Stmt make(bool infinite, Stmt expr, Stmt stmt, int line) {
      return std::make_unique<WhileStmt>(infinite, std::move(expr), std::move(stmt), line);
   }
   Stmt copy() const override;
};

struct ForStmt : public Statement {
   std::optional<Stmt> initexpr;
   std::optional<Stmt> condition;
   std::optional<Stmt> loopexpr;
   Stmt stmt;

   ForStmt(std::optional<Stmt> initexpr, std::optional<Stmt> condition, std::optional<Stmt> loopexpr, Stmt stmt, int line);
   static Stmt make(std::optional<Stmt> initexpr, std::optional<Stmt> condition, std::optional<Stmt> loopexpr, Stmt stmt, int line) {
      return std::make_unique<ForStmt>(std::move(initexpr), std::move(condition), std::move(loopexpr), std::move(stmt), line);
   }
   Stmt copy() const override;
};

struct BreakStmt : public Statement {
   BreakStmt(int line);
   static Stmt make(int line) { return std::make_unique<BreakStmt>(line); }
   Stmt copy() const override;
};

struct ContinueStmt : public Statement {
   ContinueStmt(int line);
   static Stmt make(int line) { return std::make_unique<ContinueStmt>(line); }
   Stmt copy() const override;
};

struct ReturnStmt : public Statement {
   Stmt value;

   ReturnStmt(Stmt value, int line);
   static Stmt make(Stmt value, int line) {
      return std::make_unique<ReturnStmt>(std::move(value), line);
   }
   Stmt copy() const override;
};

struct UnlessStmt : public Statement {
   Stmt expr;
   Stmt stmt;

   UnlessStmt(Stmt expr, Stmt stmt, int line);
   static Stmt make(Stmt expr, Stmt stmt, int line) {
      return std::make_unique<UnlessStmt>(std::move(expr), std::move(stmt), line);
   }
   Stmt copy() const override;
};

struct AssignmentExpr : public Statement {
   Type op;
   Stmt left;
   Stmt right;

   AssignmentExpr(Type op, Stmt left, Stmt right, int line);
   static Stmt make(Type op, Stmt left, Stmt right, int line) {
      return std::make_unique<AssignmentExpr>(op, std::move(left), std::move(right), line);
   }
   Stmt copy() const override;
};

struct TernaryExpr : public Statement {
   Stmt left;
   Stmt middle;
   Stmt right;

   TernaryExpr(Stmt left, Stmt middle, Stmt right, int line);
   static Stmt make(Stmt left, Stmt middle, Stmt right, int line) {
      return std::make_unique<TernaryExpr>(std::move(left), std::move(middle), std::move(right), line);
   }
   Stmt copy() const override;
};

struct BinaryExpr : public Statement {
   Type op;
   Stmt left;
   Stmt right;

   BinaryExpr(Type op, Stmt left, Stmt right, int line);
   static Stmt make(Type op, Stmt left, Stmt right, int line) {
      return std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), line);
   }
   Stmt copy() const override;
};

struct UnaryExpr : public Statement {
   Type op;
   Stmt value;

   UnaryExpr(Type op, Stmt value, int line);
   static Stmt make(Type op, Stmt value, int line) {
      return std::make_unique<UnaryExpr>(op, std::move(value), line);
   }
   Stmt copy() const override;
};

struct MemberAccess : public Statement {
   Stmt left;
   Stmt key;

   MemberAccess(Stmt left, Stmt key, int line);
   static Stmt make(Stmt left, Stmt key, int line) {
      return std::make_unique<MemberAccess>(std::move(left), std::move(key), line);
   }
   Stmt copy() const override;
};

struct CallExpr : public Statement {
   Stmt args;
   Stmt identifier;

   CallExpr(Stmt args, Stmt identifier, int line);
   static Stmt make(Stmt args, Stmt identifier, int line) {
      return std::make_unique<CallExpr>(std::move(args), std::move(identifier), line);
   }

   Stmt copy() const override;
};

struct ArgsListExpr : public Statement {
   std::vector<Stmt> args;

   ArgsListExpr(std::vector<Stmt> args, int line);
   static Stmt make(std::vector<Stmt> args, int line) {
      return std::make_unique<ArgsListExpr>(std::move(args), line);
   }

   Stmt copy() const override;
};

struct IdentLiteral : public Statement {
   std::string identifier;

   IdentLiteral(const std::string& identifier, int line);
   static Stmt make(const std::string& identifier, int line) {
      return std::make_unique<IdentLiteral>(identifier, line);
   }
   Stmt copy() const override;
};

struct NumberLiteral : public Statement {
   long double number;

   NumberLiteral(long double number, int line);
   static Stmt make(long double number, int line) {
      return std::make_unique<NumberLiteral>(number, line);
   }

   Stmt copy() const override;
};

struct CharLiteral : public Statement {
   char ch;

   CharLiteral(char ch, int line);
   static Stmt make(char ch, int line) {
      return std::make_unique<CharLiteral>(ch, line);
   }
   Stmt copy() const override;
};

struct StringLiteral : public Statement {
   std::string string;

   StringLiteral(const std::string& string, int line);
   static Stmt make(const std::string& string, int line) {
      return std::make_unique<StringLiteral>(string, line);
   }
   Stmt copy() const override;
};

struct ArrayLiteral : public Statement {
   std::vector<Stmt> array;

   ArrayLiteral(std::vector<Stmt> array, int line);
   static Stmt make(std::vector<Stmt> array, int line) {
      return std::make_unique<ArrayLiteral>(std::move(array), line);
   }
   Stmt copy() const override;
};

struct NullLiteral : public Statement {
   NullLiteral(int line = 0);
   static Stmt make(int line = 0) {
      return std::make_unique<NullLiteral>(line);
   }
   Stmt copy() const override;
};

struct Program : public Statement {
   std::vector<Stmt> statements;

   Program(int line = 0);
   static Stmt make(int line = 0) {
      return std::make_unique<Program>(line);
   }
   Stmt copy() const override;
};

#endif
