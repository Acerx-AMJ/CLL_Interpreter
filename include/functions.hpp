#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "values.hpp"
#include <vector>

namespace fun {
   Value print(std::vector<Value>& args, Environment* env, int line);
   Value println(std::vector<Value>& args, Environment* env, int line);
   Value printf(std::vector<Value>& args, Environment* env, int line);
   Value printfln(std::vector<Value>& args, Environment* env, int line);
   Value format(std::vector<Value>& args, Environment* env, int line);

   Value raise(std::vector<Value>& args, Environment* env, int line);
   Value assert(std::vector<Value>& args, Environment* env, int line);
   Value throw_(std::vector<Value>& args, Environment* env, int line);
   Value exit(std::vector<Value>& args, Environment* env, int line);

   Value input(std::vector<Value>& args, Environment* env, int line);
   Value inputnum(std::vector<Value>& args, Environment* env, int line);
   Value inputch(std::vector<Value>& args, Environment* env, int line);

   Value string(std::vector<Value>& args, Environment* env, int line);
   Value number(std::vector<Value>& args, Environment* env, int line);
   Value char_(std::vector<Value>& args, Environment* env, int line);
   Value bool_(std::vector<Value>& args, Environment* env, int line);
}

#endif
