#ifndef LEXER_HPP
#define LEXER_HPP

#include "tokens.hpp"
#include <vector>

class Lexer {
   const std::string& code;
   std::vector<Token> tokens;
   int line = 1;

public:
   Lexer(const std::string& code);
   std::vector<Token>& lex();
};

#endif
