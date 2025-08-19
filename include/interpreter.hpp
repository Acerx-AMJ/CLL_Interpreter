#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "environment.hpp"

class Interpreter {
   Program& program;
   Environment& environment;

   Value evaluate_stmt(Stmt stmt);
   Value evaluate_var_decl(Stmt stmt);
   Value evaluate_del_stmt(Stmt stmt);
   Value evaluate_scope(Stmt stmt);
   
   Value evaluate_expr(Stmt expr);
   Value evaluate_ternary_expr(Stmt expr);
   Value evaluate_binary_expr(Stmt expr);
   Value evaluate_unary_expr(Stmt expr);
   Value evaluate_assignment(Stmt expr);
   Value evaluate_call_expr(Stmt expr);
   Value evaluate_primary_expr(Stmt expr);

public:
   Interpreter(Program& program, Environment& environment);
   Value evaluate();
};

#endif
