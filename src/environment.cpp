#include "environment.hpp"

#include "fmt.hpp"
#include "functions.hpp"

Environment::Environment(Environment* parent)
   : parent(parent) {}

Environment::Environment()
   : parent(nullptr)
{
   declare_variable("null", std::make_unique<NullValue>(err::nline), true, err::nline);
   declare_variable("true", std::make_unique<BoolValue>(true, err::nline), true, err::nline);
   declare_variable("false", std::make_unique<BoolValue>(false, err::nline), true, err::nline);

   declare_function("print", fun::print, err::nline);
   declare_function("println", fun::println, err::nline);
   declare_function("printf", fun::printf, err::nline);
   declare_function("printfln", fun::printfln, err::nline);
   declare_function("format", fun::format, err::nline);

   declare_function("raise", fun::raise, err::nline);
   declare_function("assert", fun::assert, err::nline);
   declare_function("throw", fun::throw_, err::nline);
   declare_function("exit", fun::exit, err::nline);

   declare_function("input", fun::input, err::nline);
   declare_function("inputnum", fun::inputnum, err::nline);
   declare_function("inputch", fun::inputch, err::nline);

   declare_function("string", fun::string, err::nline);
   declare_function("number", fun::number, err::nline);
   declare_function("char", fun::char_, err::nline);
   declare_function("bool", fun::bool_, err::nline);
}

void Environment::declare_variable(const std::string& identifier, Value value, bool constant, int line) {
   fmt::raise_if(line, constants.find(identifier) != constants.end(), "Cannot shadow constant variable '{}'.", identifier);
   if (constant)
      constants.insert(identifier);
   
   variables[identifier] = std::move(value);
}

void Environment::assign_variable(const std::string& identifier, Value value, int line) {
   auto& env = resolve_variable(identifier, line);
   fmt::raise_if(line, env.constants.find(identifier) != env.constants.end(), "Cannot assign to constant '{}'.", identifier);
   env.variables[identifier] = std::move(value);
}

void Environment::delete_variable(const std::string& identifier, int line) {
   auto& env = resolve_variable(identifier, line);
   fmt::raise_if(line, env.constants.find(identifier) != env.constants.end(), "Cannot delete constant '{}'.", identifier);
   fmt::raise_if(line, env.variables.find(identifier) == env.variables.end(), "Cannot delete variable '{}' as it does not exist in the given scope.", identifier);
   env.variables.erase(identifier);
}

bool Environment::variable_exists(const std::string& identifier) {
   if (variables.find(identifier) != variables.end())
      return true;
   return parent && parent->variable_exists(identifier);
}

Value Environment::get_variable(const std::string& identifier, int line) {
   auto& env = resolve_variable(identifier, line);
   return env.variables.at(identifier)->copy();
}

Environment& Environment::resolve_variable(const std::string& identifier, int line) {
   if (variables.find(identifier) != variables.end())
      return *this;
   
   fmt::raise_if(line, !parent, "Variable '{}' does not exist in the given scope.", identifier);
   return parent->resolve_variable(identifier, line);
}

void Environment::declare_function(const std::string& identifier, const std::function<Value(const std::vector<Value>&, int)>& function, int line) {
   fmt::raise_if(line, functions.find(identifier) != functions.end(), "Function '{}' is already defined in the current scope.", identifier);
   functions.insert({identifier, function});
}

Value Environment::call_function(const std::string& identifier, const std::vector<Value>& args, int line) {
   auto& env = resolve_function(identifier, line);
   return env.functions.at(identifier)(args, line);
}

Environment& Environment::resolve_function(const std::string& identifier, int line) {
   if (functions.find(identifier) != functions.end())
      return *this;
   
   fmt::raise_if(line, !parent, "Function '{}' does not exist in the given scope.", identifier);
   return parent->resolve_function(identifier, line);
}
