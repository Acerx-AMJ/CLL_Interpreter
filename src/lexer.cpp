#include "lexer.hpp"
#include "fmt.hpp"

Lexer::Lexer(const std::string& code)
   : code(code) {}

std::vector<Token>& Lexer::lex() {
   tokens.clear();
   line = 1;

   for (auto it = code.cbegin(); it != code.cend(); ++it) {
      if (isspace(*it)) {
         line += (*it == '\n');
         continue;
      }

      // Following two if-statements handle skipping comments
      if (*it == '/' && it + 1 != code.cend() && *(it + 1) == '/') {
         for (; it != code.cend() && *it != '\n'; ++it);
         ++line;

         if (it == code.cend()) {
            return tokens;
         }
      } else if (*it == '/' && it + 1 != code.cend() && *(it + 1) == '*') {
         int original_line = line;
         for (; it + 1 != code.cend() && (*it != '*' || *(it + 1) != '/'); ++it) {
            if (*it == '\n') {
               ++line;
            }
         }
         ++it;
         fmt::raise_if(it == code.cend(), "Unterminated block comment at line {}.", original_line);
      } else if (isdigit(*it)) {
         std::string number;
         bool dot = false, last_dash = false, prefix = false, scientific = false;
         bool bin = false, hex = false, oct = false;

         if (*it == '0' && it + 1 != code.cend()) {
            ++it;
            bin = (*it == 'b' || *it == 'B');
            hex = (*it == 'x' || *it == 'X');
            oct = (*it == 'o' || *it == 'O');
            prefix = bin || hex || oct;
            it += prefix;
         }

         for (; it != code.cend(); ++it) {
            if (isdigit(*it) || (hex && ((*it <= 'f' && *it >= 'a') || (*it <= 'F' && *it >= 'A')))) {
               last_dash = false;
               number += *it;
            } else if (*it == '.') {
               if (!dot) {
                  dot = true;
                  number += '.';
               } else break;
            } else if (*it == 'e' || *it == 'E') {
               fmt::raise_if(scientific, "Expected scientific number '{}' to only contain one 'e' at line {}.", number, line);
               fmt::raise_if(prefix, "Expected prefixed number '{}' to not be scientific at line {}.", number, line);
               scientific = true;
               number += *it;
            } else if (scientific && (*it == '-' || *it == '+') && (*(it - 1) == 'e' || *(it - 1) == 'E')) {
               number += *it;
            } else if (*it != '_') break;

            if (*it == '_' || ((*it == '-' || *it == '+') && (*(it - 1) != 'e' && *(it - 1) != 'E')) || *it == '.' || *it == 'e') {
               fmt::raise_if(last_dash, "Expected number '{}' to not have two or more consecutive '_', 'e' or '.' at line {}.", number, line);
               last_dash = true;
            }
         }
         fmt::raise_if(last_dash, "Expected number '{}' to not end with '_', 'e' or '.' at line {}.", number, line);
         fmt::raise_if(number.empty() && prefix, "Expected number to not only contain the prefix at line {}.", line);

         try {
            if (scientific) {
               number = std::to_string(std::stod(number));
            } else if (prefix) {
               number = std::to_string(std::stoi(number, nullptr, (bin ? 2 : (oct ? 8 : 16))));
            }
         } catch (...) {
            fmt::raise("Prefixed number '{}' out of range at line {}.", number, line);
         }

         tokens.push_back({Type::number, (number.empty() ? "0" : number), line});
         --it;
      } else if (isalpha(*it) || *it == '_') {
         std::string string;

         for (; it != code.cend(); ++it) {
            if (!isalnum(*it) && *it != '_') {
               break;
            }
            string += *it;
         }

         if (keywords.find(string) != keywords.end()) {
            tokens.push_back({Type::keyword, string, line});
         } else {
            tokens.push_back({Type::identifier, string, line});
         }
         --it;
      } else {
         std::string op;
         for (int i = 0; i < max_op_size && it + i != code.cend(); ++i) {
            op += *(it + i);
         }
         auto original_size = op.size();

         for (int i = 0; i < original_size; ++i) {
            if (auto t = operators.find(op); t != operators.end()) {
               tokens.push_back({t->second, op, line});
               break;
            }
            op.pop_back();
         }
         fmt::raise_if(op.empty(), "Unexpected character: '{}' at line {}.", *it, line);
         
         it += op.size() - 1;
         if (it == code.cend()) {
            break;
         }
      }
   }
   tokens.push_back({Type::eof, "EOF", line});
   return tokens;
}
