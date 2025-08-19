#include "file.hpp"
#include "fmt.hpp"
#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char* argv[]) {
   fmt::raise_if(argc != 2, "Expected 2 arguments, got {} instead.", argc);
   std::string code = argv[1];

   if (file::exists(code)) {
      code = file::read(code);
   }
   Lexer lexer (code);
   auto& tokens = lexer.lex();

   Parser parser (tokens);
   auto& program = parser.parse();

   Environment global;
   Interpreter interpreter (program, global);
   interpreter.evaluate();
   return 0;
}
