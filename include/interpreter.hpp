#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "environment.hpp"

class Interpreter {
   Program& program;
   std::unique_ptr<Environment>& environment;

   void evaluate_stmt(Stmt stmt);
   void evaluate_var_decl(Stmt stmt);
   void evaluate_del_stmt(Stmt stmt);
   
   Value evaluate_expr(Stmt expr);
   Value evaluate_unary_expr(Stmt expr);
   Value evaluate_binary_expr(Stmt expr);
   Value evaluate_assignment(Stmt expr);
   Value evaluate_call_expr(Stmt expr);
   Value evaluate_primary_expr(Stmt expr);

public:
   Interpreter(Program& program, std::unique_ptr<Environment>& environment);
   void evaluate();
};

#endif
