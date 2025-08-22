#include "file.hpp"
#include "fmt.hpp"
#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char* argv[]) {
   fmt::raise_if(err::nline, argc != 2, "Expected 2 arguments, got {} instead.", argc);
   std::string code = argv[1];

   if (file::exists(code)) {
      code = file::read(code);
   }
   err::set_program_code(code);
   Lexer lexer (code);
   auto& tokens = lexer.lex();

   Parser parser (tokens);
   auto& program = parser.parse();

   Environment global;
   Interpreter interpreter;
   interpreter.evaluate(program, global);

   // Evaluate main function if it exists
   if (global.variable_exists("main"s)) {
      auto main = global.get_variable("main"s, err::nline);
      if (main->type == ValueType::fn) {
         interpreter.call_function(global, std::move(main), {}, err::nline);
      }
   }
   return 0;
}
