#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "values.hpp"
#include <functional>
#include <unordered_map>
#include <unordered_set>

class Environment {
   Environment* parent;
   std::unordered_map<std::string, std::function<Value(const std::vector<Value>&)>> functions;
   std::unordered_map<std::string, Value> variables;
   std::unordered_set<std::string> constants;

public:
   Environment(Environment* parent);
   Environment();

   void declare_variable(const std::string& identifier, Value value, bool constant);
   void assign_variable(const std::string& identifier, Value value);
   void delete_variable(const std::string& identifier);
   Value get_variable(const std::string& identifier);
   Environment& resolve_variable(const std::string& identifier);

   void declare_function(const std::string& identifier, const std::function<Value(const std::vector<Value>&)>& function);
   Value call_function(const std::string& identifier, const std::vector<Value>& args);
   Environment& resolve_function(const std::string& identifier);
};

#endif
