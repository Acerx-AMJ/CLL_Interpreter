#include "values.hpp"
#include "fmt.hpp"
#include <cmath>
#include <iostream>

// Utility functions

bool is_numeric(ValueType type) {
   return type == ValueType::number || type == ValueType::character || type == ValueType::boolean;
}

ValueLiteral::ValueLiteral(ValueType type)
   : type(type) {}

// Identifier value. Identifiers should never be converted or be operated on

IdentValue::IdentValue(const std::string& identifier)
   : identifier(identifier), ValueLiteral(ValueType::identifier) {}

void IdentValue::print() const {
   std::cout << "[" << identifier << "]";
}

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

Value IdentValue::negate() const { return copy(); }
Value IdentValue::increment() { return copy(); }
Value IdentValue::decrement() { return copy(); }
Value IdentValue::add(Value& other) const { return copy(); }
Value IdentValue::subtract(Value& other) const { return copy(); }
Value IdentValue::multiply(Value& other) const { return copy(); }
Value IdentValue::divide(Value& other) const { return copy(); }
Value IdentValue::remainder(Value& other) const { return copy(); }
Value IdentValue::exponentiate(Value& other) const { return copy(); }

// Number value

NumberValue::NumberValue(long double number)
   : number(number), ValueLiteral(ValueType::number) {}

void NumberValue::print() const {
   std::cout << as_string();
}

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

Value NumberValue::negate() const {
   return NumberValue::make(-number);
}

Value NumberValue::increment() {
   ++number;
   return copy();
}

Value NumberValue::decrement() {
   --number;
   return copy();
}

Value NumberValue::add(Value& other) const {
   if (is_numeric(other->type)) {
      return NumberValue::make(number + other->as_number());
   } else if (other->type == ValueType::string) {
      return StringValue::make(std::to_string(number) + other->as_string());
   }
   return NullValue::make();
}

Value NumberValue::subtract(Value& other) const {
   if (is_numeric(other->type)) {
      return NumberValue::make(number - other->as_number());
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Number' - 'String'.");
   }
   return NullValue::make();
}

Value NumberValue::multiply(Value& other) const {
   if (is_numeric(other->type)) {
      return NumberValue::make(number * other->as_number());
   } else if (other->type == ValueType::string) {
      auto str = other->as_string();
      std::string final;

      for (int i = 0; i < number; ++i) {
         final += str;
      }
      return StringValue::make(final);
   }
   return NullValue::make();
}

Value NumberValue::divide(Value& other) const {
   if (is_numeric(other->type)) {
      fmt::raise_if(other->as_number() == 0, "Tried to divide {} by zero.", number);
      return NumberValue::make(number / other->as_number());
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Number' / 'String'.");
   }
   return NullValue::make();
}

Value NumberValue::remainder(Value& other) const {
   if (is_numeric(other->type)) {
      auto other_num = static_cast<long long>(other->as_number());
      fmt::raise_if(other_num == 0, "Tried to divide {} by zero.", number);
      return NumberValue::make(static_cast<long long>(number) % other_num);
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Number' % 'String'.");
   }
   return NullValue::make();
}

Value NumberValue::exponentiate(Value& other) const {
   if (is_numeric(other->type)) {
      return NumberValue::make(std::pow(number, other->as_number()));
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Number' ** 'String'.");
   }
   return NullValue::make();
}

// Char value

CharValue::CharValue(char ch)
   : ch(ch), ValueLiteral(ValueType::character) {}

void CharValue::print() const {
   std::cout << ch;
}

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

Value CharValue::negate() const {
   return CharValue::make(-ch);
}

Value CharValue::increment() {
   ++ch;
   return copy();
}

Value CharValue::decrement() {
   --ch;
   return copy();
}

Value CharValue::add(Value& other) const {
   if (is_numeric(other->type)) {
      return CharValue::make(ch + other->as_number());
   } else if (other->type == ValueType::string) {
      return StringValue::make(as_string() + other->as_string());
   }
   return NullValue::make();
}

Value CharValue::subtract(Value& other) const {
   if (is_numeric(other->type)) {
      return CharValue::make(ch - other->as_number());
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Character' - 'String'.");
   }
   return NullValue::make();
}

Value CharValue::multiply(Value& other) const {
   if (is_numeric(other->type)) {
      return CharValue::make(ch * other->as_number());
   } else if (other->type == ValueType::string) {
      auto str = other->as_string();
      std::string final;

      for (int i = 0; i < ch; ++i) {
         final += str;
      }
      return StringValue::make(final);
   }
   return NullValue::make();
}

Value CharValue::divide(Value& other) const {
   if (is_numeric(other->type)) {
      fmt::raise_if(other->as_number() == 0, "Tried to divide {} by zero.", int(ch));
      return CharValue::make(ch / other->as_number());
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Character' / 'String'.");
   }
   return NullValue::make();
}

Value CharValue::remainder(Value& other) const {
   if (is_numeric(other->type)) {
      auto other_num = static_cast<long long>(other->as_number());
      fmt::raise_if(other_num == 0, "Tried to divide {} by zero.", int(ch));
      return CharValue::make(ch % other_num);
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Character' % 'String'.");
   }
   return NullValue::make();
}

Value CharValue::exponentiate(Value& other) const {
   if (is_numeric(other->type)) {
      return CharValue::make(std::pow(ch, other->as_number()));
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Character' ** 'String'.");
   }
   return NullValue::make();
}

// String value

StringValue::StringValue(const std::string& string)
   : string(string), ValueLiteral(ValueType::string) {}

void StringValue::print() const {
   std::cout << string;
}

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

Value StringValue::negate() const {
   fmt::raise("Invalid unary operation: - 'String'.");
}

Value StringValue::increment() {
   fmt::raise("Invalid unary operation: 'String' ++.");
}

Value StringValue::decrement() {
   fmt::raise("Invalid unary operation: 'String' --.");
}

Value StringValue::add(Value& other) const {
   if (other->type == ValueType::null) {
      return NullValue::make();
   }
   return StringValue::make(string + other->as_string());
}

Value StringValue::subtract(Value& other) const {
   fmt::raise("Invalid binary operation: 'String' - 'Any'.");
}

Value StringValue::multiply(Value& other) const {
   if (is_numeric(other->type)) {
      std::string final;

      for (int i = 0; i < other->as_number(); ++i) {
         final += string;
      }
      return StringValue::make(final);
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'String' * 'String'.");
   }
   return NullValue::make();
}

Value StringValue::divide(Value& other) const {
   fmt::raise("Invalid binary operation: 'String' / 'Any'.");
}

Value StringValue::remainder(Value& other) const {
   fmt::raise("Invalid binary operation: 'String' % 'Any'.");
}

Value StringValue::exponentiate(Value& other) const {
   fmt::raise("Invalid binary operation: 'String' ** 'Any'.");
}

// Boolean value

BoolValue::BoolValue(bool value)
   : value(value), ValueLiteral(ValueType::boolean) {}

void BoolValue::print() const {
   std::cout << std::boolalpha << value;
}

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

Value BoolValue::negate() const {
   return BoolValue::make(-value);
}

Value BoolValue::increment() {
   value += 1;
   return copy();
}

Value BoolValue::decrement() {
   value -= 1;
   return copy();
}

Value BoolValue::add(Value& other) const {
   if (is_numeric(other->type)) {
      return BoolValue::make(value + other->as_number());
   } else if (other->type == ValueType::string) {
      return StringValue::make(as_string() + other->as_string());
   }
   return NullValue::make();
}

Value BoolValue::subtract(Value& other) const {
   if (is_numeric(other->type)) {
      return BoolValue::make(value - other->as_number());
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Boolean' - 'String'.");
   }
   return NullValue::make();
}

Value BoolValue::multiply(Value& other) const {
   if (is_numeric(other->type)) {
      return BoolValue::make(value * other->as_number());
   } else if (other->type == ValueType::string) {
      auto str = other->as_string();
      std::string final;

      for (int i = 0; i < value; ++i) {
         final += str;
      }
      return StringValue::make(final);
   }
   return NullValue::make();
}

Value BoolValue::divide(Value& other) const {
   if (is_numeric(other->type)) {
      fmt::raise_if(other->as_number() == 0, "Tried to divide {} by zero.", int(value));
      return BoolValue::make(value - other->as_number());
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Boolean' / 'String'.");
   }
   return NullValue::make();
}

Value BoolValue::remainder(Value& other) const {
   if (is_numeric(other->type)) {
      auto other_num = static_cast<long long>(other->as_number());
      fmt::raise_if(other_num == 0, "Tried to divide {} by zero.", int(value));
      return BoolValue::make(value % other_num);
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Boolean' % 'String'.");
   }
   return NullValue::make();
}

Value BoolValue::exponentiate(Value& other) const {
   if (is_numeric(other->type)) {
      return BoolValue::make(std::pow(value, other->as_number()));
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Boolean' ** 'String'.");
   }
   return NullValue::make();
}

// Null value

NullValue::NullValue()
   : ValueLiteral(ValueType::null) {}

void NullValue::print() const {
   std::cout << "null";
}

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

Value NullValue::negate() const { return copy(); }
Value NullValue::increment() { return copy(); }
Value NullValue::decrement() { return copy(); }
Value NullValue::add(Value& other) const { return copy(); }
Value NullValue::subtract(Value& other) const { return copy(); }
Value NullValue::multiply(Value& other) const { return copy(); }
Value NullValue::divide(Value& other) const { return copy(); }
Value NullValue::remainder(Value& other) const { return copy(); }
Value NullValue::exponentiate(Value& other) const { return copy(); }
