#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "values.hpp"
#include <vector>

namespace fun {
   Value print(const std::vector<Value>& args, int line);
   Value println(const std::vector<Value>& args, int line);
   Value printf(const std::vector<Value>& args, int line);
   Value printfln(const std::vector<Value>& args, int line);
   Value format(const std::vector<Value>& args, int line);

   Value raise(const std::vector<Value>& args, int line);
   Value assert(const std::vector<Value>& args, int line);
   Value throw_(const std::vector<Value>& args, int line);
   Value exit(const std::vector<Value>& args, int line);

   Value input(const std::vector<Value>& args, int line);
   Value inputnum(const std::vector<Value>& args, int line);
   Value inputch(const std::vector<Value>& args, int line);

   Value string(const std::vector<Value>& args, int line);
   Value number(const std::vector<Value>& args, int line);
   Value char_(const std::vector<Value>& args, int line);
   Value bool_(const std::vector<Value>& args, int line);
}

#endif
