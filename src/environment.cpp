#include "environment.hpp"
#include "fmt.hpp"

Environment::Environment(std::optional<std::unique_ptr<Environment>>& parent)
   : parent(std::move(parent)) {}

Environment::Environment()
   : parent(std::nullopt) {}

void Environment::declare_variable(const std::string& identifier, Value value, bool constant) {
   fmt::raise_if(constants.find(identifier) != constants.end(), "Cannot shadow constant variable '{}'.", identifier);
   if (constant)
      constants.insert(identifier);
   
   variables[identifier] = std::move(value);
}

void Environment::assign_variable(const std::string& identifier, Value value) {
   auto& env = resolve_variable(identifier);
   fmt::raise_if(env.constants.find(identifier) != env.constants.end(), "Cannot assign to constant '{}'.", identifier);
   env.variables[identifier] = std::move(value);
}

void Environment::delete_variable(const std::string& identifier) {
   auto& env = resolve_variable(identifier);
   fmt::raise_if(env.constants.find(identifier) != env.constants.end(), "Cannot delete constant '{}'.", identifier);
   fmt::raise_if(env.variables.find(identifier) == env.variables.end(), "Cannot delete variable '{}' as it does not exist in the given scope.", identifier);
   env.variables.erase(identifier);
}

Value Environment::get_variable(const std::string& identifier) {
   auto& env = resolve_variable(identifier);
   return env.variables.at(identifier)->copy();
}

Environment& Environment::resolve_variable(const std::string& identifier) {
   if (variables.find(identifier) != variables.end())
      return *this;
   
   fmt::raise_if(!parent.has_value(), "Variable '{}' does not exist in the given scope.", identifier);
   return parent->get()->resolve_variable(identifier);
}

void Environment::declare_function(const std::string& identifier, const std::function<Value(const std::vector<Value>&)>& function) {
   fmt::raise_if(functions.find(identifier) != functions.end(), "Function '{}' is already defined in the current scope.", identifier);
   functions.insert({identifier, function});
}

Value Environment::call_function(const std::string& identifier, const std::vector<Value>& args) {
   auto& env = resolve_function(identifier);
   return env.functions.at(identifier)(args);
}

Environment& Environment::resolve_function(const std::string& identifier) {
   if (functions.find(identifier) != functions.end())
      return *this;
   
   fmt::raise_if(!parent.has_value(), "Function '{}' does not exist in the given scope.", identifier);
   return parent->get()->resolve_function(identifier);
}
