#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"
#include "tokens.hpp"
#include <vector>

class Parser {
   std::vector<Token>& tokens;
   Program program;
   size_t index = 0;

   Stmt parse_stmt();
   Stmt parse_var_decl();

   Stmt parse_expr();
   Stmt parse_assignment_expr();
   Stmt parse_additive_expr();
   Stmt parse_multiplicative_expr();
   Stmt parse_unary_expr();
   Stmt parse_call_expr();
   Stmt parse_args_list();
   Stmt parse_primary_expr();

   void advance();
   bool is(Type type) const;
   Token& current();
   int line() const;

public:
   Parser(std::vector<Token>& tokens);
   Program& parse();
};

#endif
