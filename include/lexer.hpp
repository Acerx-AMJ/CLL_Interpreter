#ifndef LEXER_HPP
#define LEXER_HPP

#include "tokens.hpp"
#include <vector>

class Lexer {
   const std::string& code;
   std::vector<Token> tokens;
   int index = 0, line = 1;

   char current();
   char peek();
   char advance();
   char get_escape_code(char escape);

public:
   Lexer(const std::string& code);
   std::vector<Token>& lex();
};

#endif
