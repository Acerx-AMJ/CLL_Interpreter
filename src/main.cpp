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
   // for (const auto& t : tokens) {
   //    fmt::printfln("{}: '{}' at line {}.", type_str[int(t.type)], t.lexeme, t.line);
   // }
   // fmt::println();

   Parser parser (tokens);
   auto& program = parser.parse();
   // program.print();

   std::unique_ptr<Environment> env = Environment::create_global();
   Interpreter interpreter (program, env);
   interpreter.evaluate();
   return 0;
}
