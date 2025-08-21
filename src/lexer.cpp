#include "lexer.hpp"
#include "fmt.hpp"

Lexer::Lexer(const std::string& code)
   : code(code) {}

std::vector<Token>& Lexer::lex() {
   for (char ch = current(); index < code.size(); ch = advance()) {
      if (isspace(ch)) {
         line += (ch == '\n');
         continue;
      }

      if (ch == '/' && peek() == '/') {
         for (; index < code.size() && ch != '\n'; ch = advance());
         ++line;
      } else if (ch == '/' && peek() == '*') {
         int original_line = line;
         for (; index < code.size() && (ch != '*' || peek() != '/'); ch = advance()) {
            line += (ch == '\n');
         }
         advance();
         fmt::raise_if(original_line, index >= code.size(), "Unterminated block comment.");
      } else if (isdigit(ch)) {
         std::string number;
         bool dot = false, last_dash = false, prefix = false, scientific = false;
         bool bin = false, hex = false, oct = false;

         if (ch == '0' && index + 1 < code.size()) {
            ch = advance();
            bin = (ch == 'b' || ch == 'B');
            hex = (ch == 'x' || ch == 'X');
            oct = (ch == 'o' || ch == 'O');
            prefix = bin || hex || oct;

            if (prefix) {
               ch = advance();
            }
         }

         for (; index < code.size(); ch = advance()) {
            if (isdigit(ch) || (hex && ((tolower(ch) <= 'f' && tolower(ch) >= 'a')))) {
               last_dash = false;
               number += ch;
            } else if (ch == '.') {
               if (!dot) {
                  dot = true;
                  number += '.';
               } else break;
            } else if (ch == 'e' || ch == 'E') {
               fmt::raise_if(line, scientific, "Expected scientific number '{}' to only contain one 'e'.", number);
               fmt::raise_if(line, prefix, "Expected prefixed number '{}' to not be scientific.", number);
               scientific = true;
               number += ch;
            } else if (scientific && (ch == '-' || ch == '+') && (code.at(index - 1) == 'e' || code.at(index - 1) == 'E')) {
               number += ch;
            } else if (ch != '_') break;

            if (ch == '_' || ((ch == '-' || ch == '+') && (code.at(index - 1) != 'e' && code.at(index - 1) != 'E')) || ch == '.' || ch == 'e') {
               fmt::raise_if(line, last_dash, "Expected number '{}' to not have two or more consecutive '_', 'e' or '.'.", number);
               last_dash = true;
            }
         }
         fmt::raise_if(line, last_dash, "Expected number '{}' to not end with '_', 'e' or '.'.", number);
         fmt::raise_if(line, number.empty() && prefix, "Expected number to not only contain the prefix.");

         try {
            if (scientific) {
               number = std::to_string(std::stod(number));
            } else if (prefix) {
               number = std::to_string(std::stoi(number, nullptr, (bin ? 2 : (oct ? 8 : 16))));
            }
         } catch (...) {
            fmt::raise(line, "Prefixed number '{}' out of range.", number);
         }

         tokens.push_back({Type::number, (number.empty() ? "0" : number), line});
         --index;
      } else if (isalpha(ch) || ch == '_') {
         std::string string;

         for (; index < code.size(); ch = advance()) {
            if (!isalnum(ch) && ch != '_') {
               break;
            }
            line += (ch == '\n');
            string += ch;
         }

         if (auto it = keyword_operators.find(string); it != keyword_operators.end()) {
            tokens.push_back({it->second, string, line});
         } else if (keywords.find(string) != keywords.end()) {
            tokens.push_back({Type::keyword, string, line});
         } else {
            tokens.push_back({Type::identifier, string, line});
         }
         --index;
      } else if (ch == '\'') {
         char character = advance();

         if (character == '\\') {
            character = get_escape_code(advance());
         }
         ch = advance();
         fmt::raise_if(line, ch != '\'', "Expected character to be one character long/unterminated character.");
         tokens.push_back({Type::character, std::string(1, character), line});
      } else if (ch == '"') {
         std::string string;
         int original_line = line;

         for (ch = advance(); index < code.size() && ch != '"'; ch = advance()) {
            if (ch == '\\') {
               ch = get_escape_code(advance());
            }
            string += ch;
         }
         fmt::raise_if(original_line, ch != '"', "Unterminated string.");
         tokens.push_back({Type::string, string, line});
      } else {
         std::string op;
         for (int i = 0; i < max_op_size && index + i < code.size(); ++i) {
            op += code.at(index + i);
         }
         auto original_size = op.size();

         for (int i = 0; i < original_size; ++i) {
            if (auto t = operators.find(op); t != operators.end()) {
               tokens.push_back({t->second, op, line});
               break;
            }
            op.pop_back();
         }
         fmt::raise_if(line, op.empty(), "Unexpected character: '{}'.", ch);  
         index += op.size() - 1;
      }
   }
   tokens.push_back({Type::eof, "EOF", line});
   return tokens;
}

// Utility functions

char Lexer::current() {
   return (index >= code.size() ? 0 : code.at(index));
}

char Lexer::peek() {
   return (index + 1 >= code.size() ? 0 : code.at(index + 1));
}

char Lexer::advance() {
   ++index;
   return current();
}

char Lexer::get_escape_code(char escape) {
   static std::unordered_map<char, char> code_map {
      {'a', '\a'}, {'b', '\b'}, {'t', '\t'}, {'n', '\n'}, {'v', '\v'}, {'f', '\f'},
      {'r', '\r'}, {'e', '\e'}, {'\\', '\\'}, {'\'', '\''}, {'"', '"'}
   };
   fmt::raise_if(line, code_map.find(escape) == code_map.end(), "Unknown escape code '\\{}'.", escape);
   return code_map[escape];
}
