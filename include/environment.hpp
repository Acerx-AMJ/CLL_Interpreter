#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "functions.hpp"
#include <functional>
#include <optional>
#include <unordered_map>
#include <unordered_set>

class Environment {
   std::optional<std::unique_ptr<Environment>> parent;
   std::unordered_map<std::string, std::function<Value(const std::vector<Value>&)>> functions;
   std::unordered_map<std::string, Value> variables;
   std::unordered_set<std::string> constants;

public:
   Environment(std::optional<std::unique_ptr<Environment>>& parent);
   Environment();

   static std::unique_ptr<Environment> create_global() {
      auto env = std::make_unique<Environment>();
      env->declare_variable("null", std::make_unique<NullValue>(), true);

      env->declare_function("print", print);
      env->declare_function("println", println);
      return env;
   }

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
