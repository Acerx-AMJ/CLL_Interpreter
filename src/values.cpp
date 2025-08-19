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

ValueLiteral::ValueLiteral(ValueType type)
   : type(type) {}

void ValueLiteral::print() const {
   std::cout << as_string();
}

Value ValueLiteral::negate() const {
   auto t1 = type;
   if (t1 == ValueType::number) {
      return NumberValue::make(-as_number());
   } else if (t1 == ValueType::character) {
      return CharValue::make(-as_char());
   } else if (t1 == ValueType::null) {
      return NullValue::make();
   } else {
      fmt::raise("Invalid unary operation: - '{}'.", value_type_str[int(t1)]);
   }
}

Value ValueLiteral::increment() const {
   auto t1 = type;
   if (t1 == ValueType::number) {
      return NumberValue::make(as_number() + 1);
   } else if (t1 == ValueType::character) {
      return CharValue::make(as_char() + 1);
   } else if (t1 == ValueType::null) {
      return NullValue::make();
   } else {
      fmt::raise("Invalid unary operation: '{}' ++.", value_type_str[int(t1)]);
   }
}

Value ValueLiteral::decrement() const {
   auto t1 = type;
   if (t1 == ValueType::number) {
      return NumberValue::make(as_number() - 1);
   } else if (t1 == ValueType::character) {
      return CharValue::make(as_char() - 1);
   } else if (t1 == ValueType::null) {
      return NullValue::make();
   } else {
      fmt::raise("Invalid unary operation: '{}' --.", value_type_str[int(t1)]);
   }
}

Value ValueLiteral::add(Value& other) const {
   auto t1 = type, t2 = other->type;
   if (any(t1, t2, ValueType::null)) {
      return NullValue::make();
   } else if (any(t1, t2, ValueType::string)) {
      return StringValue::make(as_string() + other->as_string());
   } else if (!any(t1, t2, ValueType::identifier)) {
      if (t1 == ValueType::number) {
         return NumberValue::make(as_number() + other->as_number());
      } else if (t1 == ValueType::character) {
         return CharValue::make(as_number() + other->as_number());
      } else {
         return BoolValue::make(as_number() + other->as_number());
      }
   } else {
      fmt::raise("Invalid binary operation: '{}' + '{}'.", value_type_str[int(t1)], value_type_str[int(t2)]);
   }
}

Value ValueLiteral::subtract(Value& other) const {
   auto t1 = type, t2 = other->type;
   fmt::raise_if(any(t1, t2, ValueType::string) || any(t1, t2, ValueType::identifier), "Invalid binary operation: '{}' - '{}'.", value_type_str[int(t1)], value_type_str[int(t2)]);

   if (any(t1, t2, ValueType::null)) {
      return NullValue::make();
   } else if (t1 == ValueType::number) {
      return NumberValue::make(as_number() - other->as_number());
   } else if (t1 == ValueType::character) {
      return CharValue::make(as_number() - other->as_number());
   } else {
      return BoolValue::make(as_number() - other->as_number());
   }
}

Value ValueLiteral::multiply(Value& other) const {
   auto t1 = type, t2 = other->type;
   if (any(t1, t2, ValueType::null)) {
      return NullValue::make();
   } else if (one(t1, t2, ValueType::string) && (one(t1, t2, ValueType::number) || one(t1, t2, ValueType::character) || one(t1, t2, ValueType::boolean))) {
      auto temp = (t1 == ValueType::string ? as_string() : other->as_string());
      std::string result;

      for (int i = 0; i < (t1 == ValueType::string ? other->as_number() : as_number()); ++i) {
         result += temp;
      }
      return StringValue::make(result);
   } else if (!any(t1, t2, ValueType::identifier) && !any(t1, t2, ValueType::string)) {
      if (t1 == ValueType::number) {
         return NumberValue::make(as_number() * other->as_number());
      } else if (t1 == ValueType::character) {
         return CharValue::make(as_number() * other->as_number());
      } else {
         return BoolValue::make(as_number() * other->as_number());
      }
   } else {
      fmt::raise("Invalid binary operation: '{}' * '{}'.", value_type_str[int(t1)], value_type_str[int(t2)]);
   }
}

Value ValueLiteral::divide(Value& other) const {
   auto t1 = type, t2 = other->type;
   fmt::raise_if(any(t1, t2, ValueType::string) || any(t1, t2, ValueType::identifier), "Invalid binary operation: '{}' / '{}'.", value_type_str[int(t1)], value_type_str[int(t2)]);

   if (any(t1, t2, ValueType::null)) {
      return NullValue::make();
   }
   
   fmt::raise_if(other->as_number() == 0, "Tried to divide by zero: '{}' / 0.", as_number());
   if (t1 == ValueType::number) {
      return NumberValue::make(as_number() / other->as_number());
   } else if (t1 == ValueType::character) {
      return CharValue::make(as_number() / other->as_number());
   } else {
      return BoolValue::make(as_number() / other->as_number());
   }
}

Value ValueLiteral::remainder(Value& other) const {
   auto t1 = type, t2 = other->type;
   fmt::raise_if(any(t1, t2, ValueType::string) || any(t1, t2, ValueType::identifier), "Invalid binary operation: '{}' %/%% '{}'.", value_type_str[int(t1)], value_type_str[int(t2)]);

   if (any(t1, t2, ValueType::null)) {
      return NullValue::make();
   }
   
   fmt::raise_if(other->as_number() == 0, "Tried to divide by zero: '{}' %/%% 0.", as_number());
   if (t1 == ValueType::number) {
      return NumberValue::make(std::remainder(as_number(), other->as_number()));
   } else if (t1 == ValueType::character) {
      return CharValue::make(std::remainder(as_number(), other->as_number()));
   } else {
      return BoolValue::make(std::remainder(as_number(), other->as_number()));
   }
}

Value ValueLiteral::exponentiate(Value& other) const {
   auto t1 = type, t2 = other->type;
   fmt::raise_if(any(t1, t2, ValueType::string) || any(t1, t2, ValueType::identifier), "Invalid binary operation: '{}' ** '{}'.", value_type_str[int(t1)], value_type_str[int(t2)]);

   if (any(t1, t2, ValueType::null)) {
      return NullValue::make();
   } else if (t1 == ValueType::number) {
      return NumberValue::make(std::pow(as_number(), other->as_number()));
   } else if (t1 == ValueType::character) {
      return CharValue::make(std::pow(as_number(), other->as_number()));
   } else {
      return BoolValue::make(std::pow(as_number(), other->as_number()));
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
   fmt::raise_if(any(t1, t2, ValueType::null) || any(t1, t2, ValueType::identifier), "Invalid binary operation: '{}' {} '{}'.", value_type_str[int(t1)], op, value_type_str[int(t2)]);

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

IdentValue::IdentValue(const std::string& identifier)
   : identifier(identifier), ValueLiteral(ValueType::identifier) {}

std::string IdentValue::as_string() const {
   return std::string("[") + identifier + "]";
}

long double IdentValue::as_number() const {
   fmt::raise("Cannot convert 'Identifier' to 'Number'.");
}

char IdentValue::as_char() const {
   fmt::raise("Cannot convert 'Identifier' to 'Character'.");
}

bool IdentValue::as_bool() const {
   fmt::raise("Cannot convert 'Identifier' to 'Boolean'.");
}

Value IdentValue::copy() const {
   return IdentValue::make(identifier);
}

// Number value

NumberValue::NumberValue(long double number)
   : number(number), ValueLiteral(ValueType::number) {}

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
   return NumberValue::make(number);
}

// Char value

CharValue::CharValue(char ch)
   : ch(ch), ValueLiteral(ValueType::character) {}

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
   return CharValue::make(ch);
}

// String value

StringValue::StringValue(const std::string& string)
   : string(string), ValueLiteral(ValueType::string) {}

std::string StringValue::as_string() const {
   return string;
}

long double StringValue::as_number() const {
   try {
      return std::stold(string);
   } catch (...) {
      fmt::raise("Could not convert string '{}' to a number.", string);
   }
}

char StringValue::as_char() const {
   fmt::raise_if(string.size() > 1, "Could not convert string '{}' to a character as it has too many characters.", string);
   return (string.empty() ? 0 : string.at(0));
}

bool StringValue::as_bool() const {
   return !string.empty();
}

Value StringValue::copy() const {
   return StringValue::make(string);
}

// Boolean value

BoolValue::BoolValue(bool value)
   : value((value ? true : false)), ValueLiteral(ValueType::boolean) {}

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
   return BoolValue::make(value);
}

// Null value

NullValue::NullValue()
   : ValueLiteral(ValueType::null) {}

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
   return NullValue::make();
}
