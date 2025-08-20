#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std::string_literals;

enum class Type : char {
   eof, keyword, identifier, number, character, string,
   increment, decrement, assign,
   plus_eq, minus_eq, multiply_eq, divide_eq, remainder_eq, exponentiate_eq,
   plus, minus, multiply, divide, remainder, exponentiate,
   log_and, log_or, log_not, divisible, binary_cond, quesion, colon, equals, really_equals, not_equals, really_not_equals, greater, greater_equal, smaller, smaller_equal,
   l_paren, r_paren, l_brace, r_brace, comma
};

constexpr std::string_view type_str[] {
   "EOF", "Keyword", "Identifier", "Number", "Character", "String",
   "++", "--", "=",
   "+=", "-=", "*=", "/=", "%=", "**=",
   "+", "-", "*", "/", "%", "**",
   "&&", "||", "!", "%%", "??", "?", ":", "==", "===", "!=", "!==", ">", ">=", "<", "<=",
   "(", ")", "{", "}", ","
};

struct Token {
   Type type {};
   std::string lexeme;
   int line = 0;
};

static constexpr int max_op_size = 3;
static std::unordered_map<std::string_view, Type> operators {
   {"++", Type::increment}, {"--", Type::decrement}, {"=", Type::assign},
   {"+=", Type::plus_eq}, {"-=", Type::minus_eq}, {"*=", Type::multiply_eq}, {"/=", Type::divide_eq}, {"%=", Type::remainder_eq}, {"**=", Type::exponentiate_eq},
   {"+", Type::plus}, {"-", Type::minus}, {"*", Type::multiply}, {"/", Type::divide}, {"%", Type::remainder}, {"**", Type::exponentiate},
   {"&&", Type::log_and}, {"||", Type::log_or}, {"!", Type::log_not}, {"%%", Type::divisible}, {"??", Type::binary_cond}, {"?", Type::quesion}, {":", Type::colon}, {"==", Type::equals}, {"===", Type::really_equals}, {"!=", Type::not_equals}, {"!==", Type::really_not_equals}, {">", Type::greater}, {">=", Type::greater_equal}, {"<", Type::smaller}, {"<=", Type::smaller_equal},
   {"(", Type::l_paren}, {")", Type::r_paren}, {"{", Type::l_brace}, {"}", Type::r_brace}, {",", Type::comma}
};

static std::unordered_set<std::string> keywords {
   "let"s, "con"s, "delete"s, "exists"s, "if"s, "elif"s, "else"s, "do"s
};

#endif
