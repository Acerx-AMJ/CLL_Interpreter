#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include "values.hpp"

namespace prop {
   // Property functions

   void init();
   bool exists(const std::string& name, ValueType type);
   bool overrides(const std::string& name, ValueType type);
   Value get(const std::string& name, ValueType type);

   // Array functions

   Value array_push(std::vector<Value>& args, Environment* env, int line);
   Value array_pop(std::vector<Value>& args, Environment* env, int line);
   Value array_size(std::vector<Value>& args, Environment* env, int line);
   Value array_empty(std::vector<Value>& args, Environment* env, int line);
   Value array_at(std::vector<Value>& args, Environment* env, int line);
   Value array_find(std::vector<Value>& args, Environment* env, int line);
   Value array_find_all(std::vector<Value>& args, Environment* env, int line);
   Value array_contains(std::vector<Value>& args, Environment* env, int line);
   Value array_in_bounds(std::vector<Value>& args, Environment* env, int line);
   Value array_first(std::vector<Value>& args, Environment* env, int line);
   Value array_last(std::vector<Value>& args, Environment* env, int line);
   Value array_clear(std::vector<Value>& args, Environment* env, int line);
   Value array_fill(std::vector<Value>& args, Environment* env, int line);
   Value array_join(std::vector<Value>& args, Environment* env, int line);
}

#endif
