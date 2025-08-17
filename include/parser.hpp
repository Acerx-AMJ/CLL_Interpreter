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

   // Expression functions
   Stmt parse_expr();
   Stmt parse_assignment_expr();
   
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
