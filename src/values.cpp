#include "values.hpp"

#include "fmt.hpp"
#include <cmath>
#include <iostream>

// Utility functions

bool any(ValueType t1, ValueType t2, ValueType type) {
   return t1 == type || t2 == type;
}

bool one(ValueType t1, ValueType t2, ValueType type) {
   return (t1 == type || t2 == type) && t1 != t2;
}

// Value functions

ValueLiteral::ValueLiteral(ValueType type, int line)
   : type(type), line(line) {}

void ValueLiteral::print() const {
   std::cout << as_string();
}

Value ValueLiteral::negate() const {
   auto t1 = type;
   if (t1 == ValueType::number) {
      return NumberValue::make(-as_number(), line);
   } else if (t1 == ValueType::character) {
      return CharValue::make(-as_char(), line);
   } else if (t1 == ValueType::null) {
      return NullValue::make(line);
   } else {
      fmt::raise(line, "Invalid unary operation: - '{}'.", value_type_str[int(t1)]);
   }
}

Value ValueLiteral::increment() const {
   auto t1 = type;
   if (t1 == ValueType::number) {
      return NumberValue::make(as_number() + 1, line);
   } else if (t1 == ValueType::character) {
      return CharValue::make(as_char() + 1, line);
   } else if (t1 == ValueType::null) {
      return NullValue::make(line);
   } else {
      fmt::raise(line, "Invalid unary operation: '{}' ++.", value_type_str[int(t1)]);
   }
}

Value ValueLiteral::decrement() const {
   auto t1 = type;
   if (t1 == ValueType::number) {
      return NumberValue::make(as_number() - 1, line);
   } else if (t1 == ValueType::character) {
      return CharValue::make(as_char() - 1, line);
   } else if (t1 == ValueType::null) {
      return NullValue::make(line);
   } else {
      fmt::raise(line, "Invalid unary operation: '{}' --.", value_type_str[int(t1)]);
   }
}

Value ValueLiteral::add(Value& other) const {
   auto t1 = type, t2 = other->type;
   if (any(t1, t2, ValueType::null)) {
      return NullValue::make(line);
   } else if (any(t1, t2, ValueType::string)) {
      return StringValue::make(as_string() + other->as_string(), line);
   } else if (!any(t1, t2, ValueType::identifier)) {
      if (t1 == ValueType::number) {
         return NumberValue::make(as_number() + other->as_number(), line);
      } else if (t1 == ValueType::character) {
         return CharValue::make(as_number() + other->as_number(), line);
      } else {
         return BoolValue::make(as_number() + other->as_number(), line);
      }
   } else {
      fmt::raise(line, "Invalid binary operation: '{}' + '{}'.", value_type_str[int(t1)], value_type_str[int(t2)]);
   }
}

Value ValueLiteral::subtract(Value& other) const {
   auto t1 = type, t2 = other->type;
   fmt::raise_if(line, any(t1, t2, ValueType::string) || any(t1, t2, ValueType::identifier), "Invalid binary operation: '{}' - '{}'.", value_type_str[int(t1)], value_type_str[int(t2)]);

   if (any(t1, t2, ValueType::null)) {
      return NullValue::make(line);
   } else if (t1 == ValueType::number) {
      return NumberValue::make(as_number() - other->as_number(), line);
   } else if (t1 == ValueType::character) {
      return CharValue::make(as_number() - other->as_number(), line);
   } else {
      return BoolValue::make(as_number() - other->as_number(), line);
   }
}

Value ValueLiteral::multiply(Value& other) const {
   auto t1 = type, t2 = other->type;
   if (any(t1, t2, ValueType::null)) {
      return NullValue::make(line);
   } else if (one(t1, t2, ValueType::string) && (one(t1, t2, ValueType::number) || one(t1, t2, ValueType::character) || one(t1, t2, ValueType::boolean))) {
      auto temp = (t1 == ValueType::string ? as_string() : other->as_string());
      std::string result;

      for (int i = 0; i < (t1 == ValueType::string ? other->as_number() : as_number()); ++i) {
         result += temp;
      }
      return StringValue::make(result, line);
   } else if (!any(t1, t2, ValueType::identifier) && !any(t1, t2, ValueType::string)) {
      if (t1 == ValueType::number) {
         return NumberValue::make(as_number() * other->as_number(), line);
      } else if (t1 == ValueType::character) {
         return CharValue::make(as_number() * other->as_number(), line);
      } else {
         return BoolValue::make(as_number() * other->as_number(), line);
      }
   } else {
      fmt::raise(line, "Invalid binary operation: '{}' * '{}'.", value_type_str[int(t1)], value_type_str[int(t2)]);
   }
}

Value ValueLiteral::divide(Value& other) const {
   auto t1 = type, t2 = other->type;
   fmt::raise_if(line, any(t1, t2, ValueType::string) || any(t1, t2, ValueType::identifier), "Invalid binary operation: '{}' / '{}'.", value_type_str[int(t1)], value_type_str[int(t2)]);

   if (any(t1, t2, ValueType::null)) {
      return NullValue::make(line);
   }
   
   fmt::raise_if(line, other->as_number() == 0, "Division by zero error: {} / 0.", as_number());
   if (t1 == ValueType::number) {
      return NumberValue::make(as_number() / other->as_number(), line);
   } else if (t1 == ValueType::character) {
      return CharValue::make(as_number() / other->as_number(), line);
   } else {
      return BoolValue::make(as_number() / other->as_number(), line);
   }
}

Value ValueLiteral::remainder(Value& other) const {
   auto t1 = type, t2 = other->type;
   fmt::raise_if(line, any(t1, t2, ValueType::string) || any(t1, t2, ValueType::identifier), "Invalid binary operation: '{}' %/%% '{}'.", value_type_str[int(t1)], value_type_str[int(t2)]);

   if (any(t1, t2, ValueType::null)) {
      return NullValue::make(line);
   }
   
   fmt::raise_if(line, other->as_number() == 0, "Division by zero error: {} %/%% 0.", as_number());
   if (t1 == ValueType::number) {
      return NumberValue::make(std::remainder(as_number(), other->as_number()), line);
   } else if (t1 == ValueType::character) {
      return CharValue::make(std::remainder(as_number(), other->as_number()), line);
   } else {
      return BoolValue::make(std::remainder(as_number(), other->as_number()), line);
   }
}

Value ValueLiteral::exponentiate(Value& other) const {
   auto t1 = type, t2 = other->type;
   fmt::raise_if(line, any(t1, t2, ValueType::string) || any(t1, t2, ValueType::identifier), "Invalid binary operation: '{}' ** '{}'.", value_type_str[int(t1)], value_type_str[int(t2)]);

   if (any(t1, t2, ValueType::null)) {
      return NullValue::make(line);
   } else if (t1 == ValueType::number) {
      return NumberValue::make(std::pow(as_number(), other->as_number()), line);
   } else if (t1 == ValueType::character) {
      return CharValue::make(std::pow(as_number(), other->as_number()), line);
   } else {
      return BoolValue::make(std::pow(as_number(), other->as_number()), line);
   }
}

bool ValueLiteral::equal(Value& other) const {
   auto t1 = type, t2 = other->type;
   if (any(t1, t2, ValueType::null)) {
      return t1 == t2;
   } else if (any(t1, t2, ValueType::boolean)) {
      return as_bool() == other->as_bool();
   } else if (!any(t1, t2, ValueType::string) && !any(t1, t2, ValueType::identifier)) {
      return as_number() == other->as_number();
   } else {
      return as_string() == other->as_string();
   }
}

bool ValueLiteral::greater(Value& other, const std::string& op) const {
   auto t1 = type, t2 = other->type;
   fmt::raise_if(line, any(t1, t2, ValueType::null) || any(t1, t2, ValueType::identifier), "Invalid binary operation: '{}' {} '{}'.", value_type_str[int(t1)], op, value_type_str[int(t2)]);

   if (any(t1, t2, ValueType::string)) {
      std::string s1 = as_string(), s2 = other->as_string();
      for (int i = 0; i < std::min(s1.size(), s2.size()); ++i) {
         char c1 = std::tolower(s1.at(i)), c2 = std::tolower(s2.at(i));
         if (c1 > c2) {
            return true;
         } else if (c1 < c2) {
            return false;
         }
      }
      return false;
   } else {
      return as_number() > other->as_number();
   }
}

// Identifier value

IdentValue::IdentValue(const std::string& identifier, int line)
   : identifier(identifier), ValueLiteral(ValueType::identifier, line) {}

std::string IdentValue::as_string() const {
   return std::string("[") + identifier + "]";
}

long double IdentValue::as_number() const {
   fmt::raise(line, "Cannot convert 'Identifier' to 'Number'.");
}

char IdentValue::as_char() const {
   fmt::raise(line, "Cannot convert 'Identifier' to 'Character'.");
}

bool IdentValue::as_bool() const {
   fmt::raise(line, "Cannot convert 'Identifier' to 'Boolean'.");
}

Value IdentValue::copy() const {
   return IdentValue::make(identifier, line);
}

// Number value

NumberValue::NumberValue(long double number, int line)
   : number(number), ValueLiteral(ValueType::number, line) {}

std::string NumberValue::as_string() const {
   std::string str = std::to_string(number);
   return str.substr(0, str.size() - (number == floor(number) ? 7 : 4));
}

long double NumberValue::as_number() const {
   return number;
}

char NumberValue::as_char() const {
   return number;
}

bool NumberValue::as_bool() const {
   return number != 0.0;
}

Value NumberValue::copy() const {
   return NumberValue::make(number, line);
}

// Char value

CharValue::CharValue(char ch, int line)
   : ch(ch), ValueLiteral(ValueType::character, line) {}

std::string CharValue::as_string() const {
   return std::string(1, ch);
}

long double CharValue::as_number() const {
   return ch;
}

char CharValue::as_char() const {
   return ch;
}

bool CharValue::as_bool() const {
   return ch != 0;
}

Value CharValue::copy() const {
   return CharValue::make(ch, line);
}

// String value

StringValue::StringValue(const std::string& string, int line)
   : string(string), ValueLiteral(ValueType::string, line) {}

std::string StringValue::as_string() const {
   return string;
}

long double StringValue::as_number() const {
   try {
      return std::stold(string);
   } catch (...) {
      fmt::raise(line, "Could not convert string '{}' to a number. Number might be too large, too small, or invalid.", string);
   }
}

char StringValue::as_char() const {
   fmt::raise_if(line, string.size() > 1, "Could not convert string '{}' to a character as it has too many characters.", string);
   return (string.empty() ? 0 : string.at(0));
}

bool StringValue::as_bool() const {
   return !string.empty();
}

Value StringValue::copy() const {
   return StringValue::make(string, line);
}

// Boolean value

BoolValue::BoolValue(bool value, int line)
   : value((value ? true : false)), ValueLiteral(ValueType::boolean, line) {}

std::string BoolValue::as_string() const {
   return (value ? "true" : "false");
}

long double BoolValue::as_number() const {
   return value;
}

char BoolValue::as_char() const {
   return value;
}

bool BoolValue::as_bool() const {
   return value;
}

Value BoolValue::copy() const {
   return BoolValue::make(value, line);
}

// Native function

NativeFn::NativeFn(const Func& call, const std::string& identifier, int line)
   : call(call), identifier(identifier), ValueLiteral(ValueType::native_fn, line) {}

std::string NativeFn::as_string() const {
   return identifier;
}

long double NativeFn::as_number() const {
   fmt::raise(line, "Cannot convert 'NativeFunction' to 'Number'.");
}

char NativeFn::as_char() const {
   fmt::raise(line, "Cannot convert 'NativeFunction' to 'Character'.");
}

bool NativeFn::as_bool() const {
   fmt::raise(line, "Cannot convert 'NativeFunction' to 'Boolean'.");
}

Value NativeFn::copy() const {
   return NativeFn::make(call, identifier, line);
}

// Function

Function::Function(const std::string& identifier, const std::vector<std::string>& parameters, const std::string& returns, Environment* env, Stmt body, int line)
   : identifier(identifier), parameters(parameters), returns(returns), env(env), body(std::move(body)), ValueLiteral(ValueType::fn, line) {}

std::string Function::as_string() const {
   return identifier;
}

long double Function::as_number() const {
   fmt::raise(line, "Cannot convert 'Function' to 'Number'.");
}

char Function::as_char() const {
   fmt::raise(line, "Cannot convert 'Function' to 'Character'.");
}

bool Function::as_bool() const {
   fmt::raise(line, "Cannot convert 'Function' to 'Boolean'.");
}

Value Function::copy() const {
   return Function::make(identifier, parameters, returns, env, body->copy(), line);
}

// Null value

NullValue::NullValue(int line)
   : ValueLiteral(ValueType::null, line) {}

std::string NullValue::as_string() const {
   return "null";
}

long double NullValue::as_number() const {
   return 0;
}

char NullValue::as_char() const {
   return 0;
}

bool NullValue::as_bool() const {
   return false;
}
 
Value NullValue::copy() const {
   return NullValue::make(line);
}
