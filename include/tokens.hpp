#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std::string_literals;

enum class Type : char {
   eof, keyword, identifier, number, character, string,
   increment, decrement, equals,
   plus_equals, minus_equals, multiply_equals, divide_equals, remainder_equals, exponentiate_equals,
   plus, minus, multiply, divide, remainder, exponentiate,
   l_paren, r_paren, l_brace, r_brace, comma
};

constexpr std::string_view type_str[] {
   "EOF", "Keyword", "Identifier", "Number", "Character", "String",
   "++", "--", "=",
   "+=", "-=", "*=", "/=", "%=", "**=",
   "+", "-", "*", "/", "%", "**",
   "(", ")", "{", "}", ","
};

struct Token {
   Type type {};
   std::string lexeme;
   int line = 0;
};

static constexpr int max_op_size = 3;
static std::unordered_map<std::string_view, Type> operators {
   {"++", Type::increment}, {"--", Type::decrement}, {"=", Type::equals},
   {"+=", Type::plus_equals}, {"-=", Type::minus_equals}, {"*=", Type::multiply_equals}, {"/=", Type::divide_equals}, {"%=", Type::remainder_equals}, {"**=", Type::exponentiate_equals},
   {"+", Type::plus}, {"-", Type::minus}, {"*", Type::multiply}, {"/", Type::divide}, {"%", Type::remainder}, {"**", Type::exponentiate},
   {"(", Type::l_paren}, {")", Type::r_paren}, {"{", Type::l_brace}, {"}", Type::r_brace}, {",", Type::comma}
};

static std::unordered_set<std::string> keywords {
   "let"s, "con"s, "delete"s
};

#endif
