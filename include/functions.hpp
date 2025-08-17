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
}

#endif
