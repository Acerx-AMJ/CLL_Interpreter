// Includes

#include "file.hpp"
#include "fmt.hpp"
#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "properties.hpp"

// Main program entry point

int main(int argc, char* argv[]) {
   fmt::raise_if(err::nline, argc < 2, "Expected at least 2 arguments, got {} instead.", argc);
   std::string code = argv[1];

   if (file::exists(code)) {
      code = file::read(code);
   }
   err::set_program_code(code);
   Lexer lexer (code);
   auto& tokens = lexer.lex();

   Parser parser (tokens);
   auto& program = parser.parse();

   prop::init();
   Environment global;
   Interpreter interpreter;
   interpreter.evaluate(program, global);

   // Evaluate main function if it exists
   if (global.variable_exists("main"s)) {
      auto main = global.get_variable("main"s, err::nline);
      if (main->type == ValueType::fn) {
         std::vector<Value> args;

         // Pass in arguments if they are provided and 'main' function takes them
         if (get_value<Function>(main).parameters.size() == 1) {
            auto array = std::make_unique<Array>(std::vector<Value>{}, err::nline);
            for (int i = 2; i < argc; ++i) {
               array->array.push_back(StringValue::make(argv[i], err::nline));
            }
            args.push_back(std::move(array));
         } else if (get_value<Function>(main).parameters.size() != 0) {
            fmt::raise(err::nline, "'main' function parameter count should be 0 or 1.");
         }

         interpreter.call_function(global, std::move(main), args, err::nline);
      }
   }
   return 0;
}
