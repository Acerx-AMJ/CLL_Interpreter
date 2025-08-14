#ifndef LEXER_HPP
#define LEXER_HPP

#include "tokens.hpp"
#include <vector>

class Lexer {
   std::vector<Token> tokens;
   int line = 1;

public:
   std::vector<Token>& lex(const std::string& code);
};

#endif
