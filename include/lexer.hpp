#ifndef LEXER_HPP
#define LEXER_HPP

// Includes

#include "tokens.hpp"
#include <vector>

// Lexer

class Lexer {
   const std::string& code;
   std::vector<Token> tokens;
   int index = 0, line = 1;

   // Helper functions

   char current();
   char peek();
   char advance();
   char get_escape_code(char escape);

public:
   // Lex functions

   Lexer(const std::string& code);
   std::vector<Token>& lex();
};

#endif
