#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "values.hpp"
#include <unordered_map>
#include <unordered_set>

class Environment {
   Environment* parent;
   std::unordered_map<std::string, Value> variables;
   std::unordered_set<std::string> constants;

public:
   Environment(Environment* parent);
   Environment();

   void declare_variable(const std::string& identifier, Value value, bool constant, int line);
   void assign_variable(const std::string& identifier, Value value, int line);
   void delete_variable(const std::string& identifier, int line);
   bool variable_exists(const std::string& identifier);
   Value get_variable(const std::string& identifier, int line);
   Environment& resolve_variable(const std::string& identifier, int line);
};

#endif
