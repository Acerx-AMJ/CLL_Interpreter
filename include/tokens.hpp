#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std::string_literals;

enum class Type : char {
   eof, keyword, identifier, number, equals, plus, minus, multiply, divide, remainder,
   l_paren, r_paren, comma
};

constexpr std::string_view type_str[] {
   "EOF", "Keyword", "Identifier", "Number", "=", "+", "-", "*", "/", "%", "(", ")", ","
};

struct Token {
   Type type {};
   std::string lexeme;
   int line = 0;
};

static std::unordered_map<char, Type> operators {
   {'=', Type::equals}, {'+', Type::plus}, {'-', Type::minus}, {'*', Type::multiply}, {'/', Type::divide}, {'%', Type::remainder},
   {'(', Type::l_paren}, {')', Type::r_paren}, {',', Type::comma}
};

static std::unordered_set<std::string> keywords {
   "let"s, "con"s
};

#endif
