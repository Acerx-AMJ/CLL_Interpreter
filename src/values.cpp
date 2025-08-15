#include "values.hpp"
#include "fmt.hpp"
#include <cmath>
#include <iostream>

ValueLiteral::ValueLiteral(ValueType type)
   : type(type) {}

// Identifier value

IdentifierValue::IdentifierValue(const std::string& identifier)
   : identifier(identifier), ValueLiteral(ValueType::identifier) {}

void IdentifierValue::print() const {
   std::cout << "[" << identifier << "]";
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
   std::cout << number;
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
   if (other->type == ValueType::number) {
      auto other_number = static_cast<NumberValue&>(*other.get()).number;
      return std::make_unique<NumberValue>(number + other_number);
   } else {
      return std::make_unique<NullValue>();
   }
}

Value NumberValue::subtract(Value& other) const {
   if (other->type == ValueType::number) {
      auto other_number = static_cast<NumberValue&>(*other.get()).number;
      return std::make_unique<NumberValue>(number - other_number);
   } else {
      return std::make_unique<NullValue>();
   }
}

Value NumberValue::multiply(Value& other) const {
   if (other->type == ValueType::number) {
      auto other_number = static_cast<NumberValue&>(*other.get()).number;
      return std::make_unique<NumberValue>(number * other_number);
   } else {
      return std::make_unique<NullValue>();
   }
}

Value NumberValue::divide(Value& other) const {
   if (other->type == ValueType::number) {
      auto other_number = static_cast<NumberValue&>(*other.get()).number;
      fmt::raise_if(other_number == 0, "Tried to divide {} by zero.", number);
      return std::make_unique<NumberValue>(number / other_number);
   } else {
      return std::make_unique<NullValue>();
   }
}

Value NumberValue::remainder(Value& other) const {
   if (other->type == ValueType::number) {
      auto other_number = static_cast<NumberValue&>(*other.get()).number;
      fmt::raise_if(other_number == 0, "Tried to divide {} by zero.", number);
      return std::make_unique<NumberValue>(static_cast<long long>(number) % static_cast<long long>(other_number));
   } else {
      return std::make_unique<NullValue>();
   }
}

Value NumberValue::exponentiate(Value& other) const {
   if (other->type == ValueType::number) {
      auto other_number = static_cast<NumberValue&>(*other.get()).number;
      return std::make_unique<NumberValue>(std::pow(number, other_number));
   } else {
      return std::make_unique<NullValue>();
   }
}

// Null value

NullValue::NullValue()
   : ValueLiteral(ValueType::null) {}

void NullValue::print() const {
   std::cout << "NULL";
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
