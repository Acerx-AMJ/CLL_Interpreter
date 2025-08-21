#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"
#include "tokens.hpp"
#include <vector>

class Parser {
   std::vector<Token>& tokens;
   Program program;
   size_t index = 0;

   // Statement functions
   Stmt parse_stmt();
   Stmt parse_var_decl();
   Stmt parse_del_stmt();
   Stmt parse_exists_stmt();
   Stmt parse_if_else_stmt();
   Stmt parse_if_clause();
   Stmt parse_while_loop();
   Stmt parse_for_loop();
   Stmt parse_block();
   Stmt parse_return_stmt();
   Stmt parse_unless_stmt(Stmt stmt);

   // Expression functions
   Stmt parse_expr();
   Stmt parse_ternary_expr();
   Stmt parse_value_or_expr();
   Stmt parse_assignment_expr();

   Stmt parse_logical_or_expr();
   Stmt parse_logical_and_expr();
   Stmt parse_equality_expr();
   Stmt parse_relational_expr();

   Stmt parse_additive_expr();
   Stmt parse_multiplicative_expr();
   Stmt parse_exponentiative_expr();
   Stmt parse_unary_expr();
   Stmt parse_reverse_unary_expr();

   Stmt parse_call_expr();
   Stmt parse_args_list();
   Stmt parse_primary_expr();

   // Utility functions
   void advance();
   bool is(Type type) const;
   Token& current();
   int line() const;

public:
   Parser(std::vector<Token>& tokens);
   Program& parse();
};

#endif
