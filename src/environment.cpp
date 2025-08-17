#include "environment.hpp"

#include "fmt.hpp"
#include "functions.hpp"

Environment::Environment(Environment* parent)
   : parent(parent) {}

Environment::Environment()
   : parent(nullptr)
{
   declare_variable("null", std::make_unique<NullValue>(), true);

   declare_function("print", fun::print);
   declare_function("println", fun::println);
   declare_function("printf", fun::printf);
   declare_function("printfln", fun::printfln);
   declare_function("format", fun::format);
}

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
   
   fmt::raise_if(!parent, "Variable '{}' does not exist in the given scope.", identifier);
   return parent->resolve_variable(identifier);
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
   
   fmt::raise_if(!parent, "Function '{}' does not exist in the given scope.", identifier);
   return parent->resolve_function(identifier);
}
