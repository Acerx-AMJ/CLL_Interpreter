#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

// Includes

#include "values.hpp"
#include <vector>

// Functions

namespace fun {
   // Print/format functions

   Value print(std::vector<Value>& args, Environment* env, int line);
   Value println(std::vector<Value>& args, Environment* env, int line);
   Value printf(std::vector<Value>& args, Environment* env, int line);
   Value printfln(std::vector<Value>& args, Environment* env, int line);
   Value format(std::vector<Value>& args, Environment* env, int line);

   // Error/exit functions

   Value raise(std::vector<Value>& args, Environment* env, int line);
   Value assert(std::vector<Value>& args, Environment* env, int line);
   Value throw_(std::vector<Value>& args, Environment* env, int line);
   Value exit(std::vector<Value>& args, Environment* env, int line);

   // Input functions

   Value input(std::vector<Value>& args, Environment* env, int line);
   Value inputnum(std::vector<Value>& args, Environment* env, int line);
   Value inputch(std::vector<Value>& args, Environment* env, int line);

   // Type conversion functions

   Value string(std::vector<Value>& args, Environment* env, int line);
   Value number(std::vector<Value>& args, Environment* env, int line);
   Value char_(std::vector<Value>& args, Environment* env, int line);
   Value bool_(std::vector<Value>& args, Environment* env, int line);
}

#endif
