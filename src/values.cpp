#include "values.hpp"
#include "fmt.hpp"
#include <cmath>
#include <iostream>

ValueLiteral::ValueLiteral(ValueType type)
   : type(type) {}

// Identifier value. Identifiers should never be converted or be operated on

IdentifierValue::IdentifierValue(const std::string& identifier)
   : identifier(identifier), ValueLiteral(ValueType::identifier) {}

void IdentifierValue::print() const {
   std::cout << "[" << identifier << "]";
}

std::string IdentifierValue::as_string() const {
   return std::string("[") + identifier + "]";
}

long double IdentifierValue::as_number() const {
   fmt::raise("Cannot convert 'Identifier' to 'Number'.");
}

char IdentifierValue::as_char() const {
   fmt::raise("Cannot convert 'Identifier' to 'Character'.");
}

Value IdentifierValue::copy() const {
   return std::make_unique<IdentifierValue>(identifier);
}

Value IdentifierValue::negate() const { return copy(); }
Value IdentifierValue::increment() { return copy(); }
Value IdentifierValue::decrement() { return copy(); }
Value IdentifierValue::add(Value& other) const { return copy(); }
Value IdentifierValue::subtract(Value& other) const { return copy(); }
Value IdentifierValue::multiply(Value& other) const { return copy(); }
Value IdentifierValue::divide(Value& other) const { return copy(); }
Value IdentifierValue::remainder(Value& other) const { return copy(); }
Value IdentifierValue::exponentiate(Value& other) const { return copy(); }

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

Value NumberValue::copy() const {
   return std::make_unique<NumberValue>(number);
}

Value NumberValue::negate() const {
   return std::make_unique<NumberValue>(-number);
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
   if (other->type == ValueType::number || other->type == ValueType::character) {
      return std::make_unique<NumberValue>(number + other->as_number());
   } else if (other->type == ValueType::string) {
      return std::make_unique<StringValue>(std::to_string(number) + other->as_string());
   }
   return std::make_unique<NullValue>();
}

Value NumberValue::subtract(Value& other) const {
   if (other->type == ValueType::number || other->type == ValueType::character) {
      return std::make_unique<NumberValue>(number - other->as_number());
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Number' - 'String'.");
   }
   return std::make_unique<NullValue>();
}

Value NumberValue::multiply(Value& other) const {
   if (other->type == ValueType::number || other->type == ValueType::character) {
      return std::make_unique<NumberValue>(number * other->as_number());
   } else if (other->type == ValueType::string) {
      auto str = other->as_string();
      std::string final;

      for (int i = 0; i < number; ++i) {
         final += str;
      }
      return std::make_unique<StringValue>(final);
   }
   return std::make_unique<NullValue>();
}

Value NumberValue::divide(Value& other) const {
   if (other->type == ValueType::number || other->type == ValueType::character) {
      fmt::raise_if(other->as_number() == 0, "Tried to divide {} by zero.", number);
      return std::make_unique<NumberValue>(number / other->as_number());
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Number' / 'String'.");
   }
   return std::make_unique<NullValue>();
}

Value NumberValue::remainder(Value& other) const {
   if (other->type == ValueType::number || other->type == ValueType::character) {
      auto other_num = static_cast<long long>(other->as_number());
      fmt::raise_if(other_num == 0, "Tried to divide {} by zero.", number);
      return std::make_unique<NumberValue>(static_cast<long long>(number) % other_num);
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Number' % 'String'.");
   }
   return std::make_unique<NullValue>();
}

Value NumberValue::exponentiate(Value& other) const {
   if (other->type == ValueType::number || other->type == ValueType::character) {
      return std::make_unique<NumberValue>(std::pow(number, other->as_number()));
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Number' ** 'String'.");
   }
   return std::make_unique<NullValue>();
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

Value CharValue::copy() const {
   return std::make_unique<CharValue>(ch);
}

Value CharValue::negate() const {
   return std::make_unique<CharValue>(-ch);
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
   if (other->type == ValueType::number || other->type == ValueType::character) {
      return std::make_unique<CharValue>(ch + other->as_number());
   } else if (other->type == ValueType::string) {
      return std::make_unique<StringValue>(as_string() + other->as_string());
   }
   return std::make_unique<NullValue>();
}

Value CharValue::subtract(Value& other) const {
   if (other->type == ValueType::number || other->type == ValueType::character) {
      return std::make_unique<CharValue>(ch - other->as_number());
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Character' - 'String'.");
   }
   return std::make_unique<NullValue>();
}

Value CharValue::multiply(Value& other) const {
   if (other->type == ValueType::number || other->type == ValueType::character) {
      return std::make_unique<CharValue>(ch * other->as_number());
   } else if (other->type == ValueType::string) {
      auto str = other->as_string();
      std::string final;

      for (int i = 0; i < ch; ++i) {
         final += str;
      }
      return std::make_unique<StringValue>(final);
   }
   return std::make_unique<NullValue>();
}

Value CharValue::divide(Value& other) const {
   if (other->type == ValueType::number || other->type == ValueType::character) {
      fmt::raise_if(other->as_number() == 0, "Tried to divide {} by zero.", ch);
      return std::make_unique<CharValue>(ch / other->as_number());
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Character' / 'String'.");
   }
   return std::make_unique<NullValue>();
}

Value CharValue::remainder(Value& other) const {
   if (other->type == ValueType::number || other->type == ValueType::character) {
      auto other_num = static_cast<long long>(other->as_number());
      fmt::raise_if(other_num == 0, "Tried to divide {} by zero.", ch);
      return std::make_unique<CharValue>(ch % other_num);
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Character' % 'String'.");
   }
   return std::make_unique<NullValue>();
}

Value CharValue::exponentiate(Value& other) const {
   if (other->type == ValueType::number || other->type == ValueType::character) {
      return std::make_unique<CharValue>(std::pow(ch, other->as_number()));
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Character' ** 'String'.");
   }
   return std::make_unique<NullValue>();
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

Value StringValue::copy() const {
   return std::make_unique<StringValue>(string);
}

Value StringValue::negate() const {
   fmt::raise("Invalid unary operation: - 'String'.");
}

Value StringValue::increment() {
   fmt::raise("Invalid unary operation: ++ 'String'.");
}

Value StringValue::decrement() {
   fmt::raise("Invalid unary operation: -- 'String'.");
}

Value StringValue::add(Value& other) const {
   if (other->type == ValueType::null) {
      return std::make_unique<NullValue>();
   }
   return std::make_unique<StringValue>(string + other->as_string());
}

Value StringValue::subtract(Value& other) const {
   fmt::raise("Invalid binary operation: 'String' - 'Any'.");
}

Value StringValue::multiply(Value& other) const {
   if (other->type == ValueType::number || other->type == ValueType::character) {
      std::string final;

      for (int i = 0; i < other->as_number(); ++i) {
         final += string;
      }
      return std::make_unique<StringValue>(final);
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'String' * 'String'.");
   }
   return std::make_unique<NullValue>();
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

// Null value

NullValue::NullValue()
   : ValueLiteral(ValueType::null) {}

void NullValue::print() const {
   std::cout << "NULL";
}

std::string NullValue::as_string() const {
   return "NULL";
}

long double NullValue::as_number() const {
   return 0;
}

char NullValue::as_char() const {
   return 0;
}
 
Value NullValue::copy() const {
   return std::make_unique<NullValue>();
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
