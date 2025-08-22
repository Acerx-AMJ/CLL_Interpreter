#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "environment.hpp"
#include <stack>

class Interpreter {
   std::stack<int> loop_stack;
   bool should_return = false, should_break = false, should_continue = false;

   Value evaluate_stmt(Environment& env, Stmt stmt);
   Value evaluate_var_decl(Environment& env, Stmt stmt);
   Value evaluate_fn_decl(Environment& env, Stmt stmt);
   Value evaluate_del_stmt(Environment& env, Stmt stmt);
   Value evaluate_exists_stmt(Environment& env, Stmt stmt);
   Value evaluate_if_else_stmt(Environment& env, Stmt stmt);
   Value evaluate_while_loop(Environment& env, Stmt stmt);
   Value evaluate_for_loop(Environment& env, Stmt stmt);
   Value evaluate_unless_stmt(Environment& env, Stmt stmt);

   Value evaluate_expr(Environment& env, Stmt expr);
   Value evaluate_ternary_expr(Environment& env, Stmt expr);
   Value evaluate_binary_expr(Environment& env, Stmt expr);
   Value evaluate_unary_expr(Environment& env, Stmt expr);
   Value evaluate_assignment(Environment& env, Stmt expr);
   Value evaluate_call_expr(Environment& env, Stmt expr);
   Value evaluate_primary_expr(Environment& env, Stmt expr);

public:
   Value evaluate(Program& program, Environment& env);
   Value call_function(Environment& env, Value func, const std::vector<Value>& args, int line);
};

#endif
