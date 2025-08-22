#include "environment.hpp"

#include "fmt.hpp"
#include "functions.hpp"

Environment::Environment(Environment* parent)
   : parent(parent) {}

Environment::Environment()
   : parent(nullptr)
{
   declare_variable("null"s, NullValue::make(err::nline), true, err::nline);
   declare_variable("true"s, BoolValue::make(true, err::nline), true, err::nline);
   declare_variable("false"s, BoolValue::make(false, err::nline), true, err::nline);

   declare_variable("print"s, NativeFn::make(fun::print, "print"s, err::nline), true, err::nline);
   declare_variable("println"s, NativeFn::make(fun::println, "println"s, err::nline), true, err::nline);
   declare_variable("printf"s, NativeFn::make(fun::printf, "printf"s, err::nline), true, err::nline);
   declare_variable("printfln"s, NativeFn::make(fun::printfln, "printfln"s, err::nline), true, err::nline);
   declare_variable("format"s, NativeFn::make(fun::format, "format"s, err::nline), true, err::nline);

   declare_variable("raise"s, NativeFn::make(fun::raise, "raise"s, err::nline), true, err::nline);
   declare_variable("assert"s, NativeFn::make(fun::assert, "assert"s, err::nline), true, err::nline);
   declare_variable("throw"s, NativeFn::make(fun::throw_, "throw"s, err::nline), true, err::nline);
   declare_variable("exit"s, NativeFn::make(fun::exit, "exit"s, err::nline), true, err::nline);

   declare_variable("input"s, NativeFn::make(fun::input, "input"s, err::nline), true, err::nline);
   declare_variable("inputnum"s, NativeFn::make(fun::inputnum, "inputnum"s, err::nline), true, err::nline);
   declare_variable("inputch"s, NativeFn::make(fun::inputch, "inputch"s, err::nline), true, err::nline);

   declare_variable("string"s, NativeFn::make(fun::string, "string"s, err::nline), true, err::nline);
   declare_variable("number"s, NativeFn::make(fun::number, "number"s, err::nline), true, err::nline);
   declare_variable("char"s, NativeFn::make(fun::char_, "char"s, err::nline), true, err::nline);
   declare_variable("bool"s, NativeFn::make(fun::bool_, "bool"s, err::nline), true, err::nline);
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
