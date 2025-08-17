#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "values.hpp"
#include <vector>

namespace fun {
   Value print(const std::vector<Value>& args);
   Value println(const std::vector<Value>& args);
   Value printf(const std::vector<Value>& args);
   Value printfln(const std::vector<Value>& args);
   Value format(const std::vector<Value>& args);

   Value input(const std::vector<Value>& args);
   Value inputnum(const std::vector<Value>& args);
   Value inputch(const std::vector<Value>& args);

   Value string(const std::vector<Value>& args);
   Value number(const std::vector<Value>& args);
   Value char_(const std::vector<Value>& args);
}

#endif
