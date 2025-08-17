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
   } else if (other->type == ValueType::character) {
      auto ch = static_cast<CharValue&>(*other.get()).ch;
      return std::make_unique<NumberValue>(number + ch);
   } else if (other->type == ValueType::string) {
      auto str = static_cast<StringValue&>(*other.get()).string;
      return std::make_unique<StringValue>(std::to_string(number) + str);
   }
   return std::make_unique<NullValue>();
}

Value NumberValue::subtract(Value& other) const {
   if (other->type == ValueType::number) {
      auto other_number = static_cast<NumberValue&>(*other.get()).number;
      return std::make_unique<NumberValue>(number - other_number);
   } else if (other->type == ValueType::character) {
      auto ch = static_cast<CharValue&>(*other.get()).ch;
      return std::make_unique<NumberValue>(number - ch);
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Number' - 'String'.");
   }
   return std::make_unique<NullValue>();
}

Value NumberValue::multiply(Value& other) const {
   if (other->type == ValueType::number) {
      auto other_number = static_cast<NumberValue&>(*other.get()).number;
      return std::make_unique<NumberValue>(number * other_number);
   } else if (other->type == ValueType::character) {
      auto ch = static_cast<CharValue&>(*other.get()).ch;
      return std::make_unique<NumberValue>(number * ch);
   } else if (other->type == ValueType::string) {
      auto str = static_cast<StringValue&>(*other.get()).string;
      std::string final;

      for (int i = 0; i < number; ++i) {
         final += str;
      }
      return std::make_unique<StringValue>(final);
   }
   return std::make_unique<NullValue>();
}

Value NumberValue::divide(Value& other) const {
   if (other->type == ValueType::number) {
      auto other_number = static_cast<NumberValue&>(*other.get()).number;
      fmt::raise_if(other_number == 0, "Tried to divide {} by zero.", number);
      return std::make_unique<NumberValue>(number / other_number);
   } else if (other->type == ValueType::character) {
      auto ch = static_cast<CharValue&>(*other.get()).ch;
      fmt::raise_if(ch == 0, "Tried to divide {} by zero.", number);
      return std::make_unique<NumberValue>(number / ch);
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Number' / 'String'.");
   }
   return std::make_unique<NullValue>();
}

Value NumberValue::remainder(Value& other) const {
   if (other->type == ValueType::number) {
      auto other_number = static_cast<long long>(static_cast<NumberValue&>(*other.get()).number);
      fmt::raise_if(other_number == 0, "Tried to divide {} by zero.", number);
      return std::make_unique<NumberValue>(static_cast<long long>(number) % other_number);
   } else if (other->type == ValueType::character) {
      auto ch = static_cast<CharValue&>(*other.get()).ch;
      fmt::raise_if(ch == 0, "Tried to divide {} by zero.", number);
      return std::make_unique<NumberValue>(static_cast<long long>(number) % ch);
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Number' % 'String'.");
   }
   return std::make_unique<NullValue>();
}

Value NumberValue::exponentiate(Value& other) const {
   if (other->type == ValueType::number) {
      auto other_number = static_cast<NumberValue&>(*other.get()).number;
      return std::make_unique<NumberValue>(std::pow(number, other_number));
   } else if (other->type == ValueType::character) {
      auto ch = static_cast<CharValue&>(*other.get()).ch;
      return std::make_unique<NumberValue>(std::pow(number, ch));
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
   if (other->type == ValueType::number) {
      auto number = static_cast<NumberValue&>(*other.get()).number;
      return std::make_unique<CharValue>(ch + number);
   } else if (other->type == ValueType::character) {
      auto other_ch = static_cast<CharValue&>(*other.get()).ch;
      return std::make_unique<CharValue>(ch + other_ch);
   } else if (other->type == ValueType::string) {
      auto str = static_cast<StringValue&>(*other.get()).string;
      return std::make_unique<StringValue>(std::string(1, ch) + str);
   }
   return std::make_unique<NullValue>();
}

Value CharValue::subtract(Value& other) const {
   if (other->type == ValueType::number) {
      auto number = static_cast<NumberValue&>(*other.get()).number;
      return std::make_unique<CharValue>(ch - number);
   } else if (other->type == ValueType::character) {
      auto other_ch = static_cast<CharValue&>(*other.get()).ch;
      return std::make_unique<CharValue>(ch - other_ch);
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Char' - 'String'.");
   }
   return std::make_unique<NullValue>();
}

Value CharValue::multiply(Value& other) const {
   if (other->type == ValueType::number) {
      auto number = static_cast<NumberValue&>(*other.get()).number;
      return std::make_unique<CharValue>(ch * number);
   } else if (other->type == ValueType::character) {
      auto other_ch = static_cast<CharValue&>(*other.get()).ch;
      return std::make_unique<CharValue>(ch * other_ch);
   } else if (other->type == ValueType::string) {
      auto str = static_cast<StringValue&>(*other.get()).string;
      std::string final;

      for (int i = 0; i < ch; ++i) {
         final += str;
      }
      return std::make_unique<StringValue>(final);
   }
   return std::make_unique<NullValue>();
}

Value CharValue::divide(Value& other) const {
   if (other->type == ValueType::number) {
      auto number = static_cast<NumberValue&>(*other.get()).number;
      fmt::raise_if(number == 0, "Tried to divide {} by zero.", ch);
      return std::make_unique<CharValue>(ch / number);
   } else if (other->type == ValueType::character) {
      auto other_ch = static_cast<CharValue&>(*other.get()).ch;
      fmt::raise_if(other_ch == 0, "Tried to divide {} by zero.", ch);
      return std::make_unique<CharValue>(ch / other_ch);
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Char' / 'String'.");
   }
   return std::make_unique<NullValue>();
}

Value CharValue::remainder(Value& other) const {
   if (other->type == ValueType::number) {
      auto number = static_cast<long long>(static_cast<NumberValue&>(*other.get()).number);
      fmt::raise_if(number == 0, "Tried to divide {} by zero.", ch);
      return std::make_unique<CharValue>(ch % number);
   } else if (other->type == ValueType::character) {
      auto other_ch = static_cast<CharValue&>(*other.get()).ch;
      fmt::raise_if(other_ch == 0, "Tried to divide {} by zero.", ch);
      return std::make_unique<CharValue>(ch % other_ch);
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Char' % 'String'.");
   }
   return std::make_unique<NullValue>();
}

Value CharValue::exponentiate(Value& other) const {
   if (other->type == ValueType::number) {
      auto number = static_cast<NumberValue&>(*other.get()).number;
      return std::make_unique<CharValue>(std::pow(ch, number));
   } else if (other->type == ValueType::character) {
      auto other_ch = static_cast<CharValue&>(*other.get()).ch;
      return std::make_unique<CharValue>(std::pow(ch, other_ch));
   } else if (other->type == ValueType::string) {
      fmt::raise("Invalid binary operation: 'Char' ** 'String'.");
   }
   return std::make_unique<NullValue>();
}

// String value

StringValue::StringValue(const std::string& string)
   : string(string), ValueLiteral(ValueType::string) {}

void StringValue::print() const {
   std::cout << string;
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
   if (other->type == ValueType::number) {
      auto number = static_cast<NumberValue&>(*other.get()).number;
      return std::make_unique<StringValue>(string + std::to_string(number));
   } else if (other->type == ValueType::character) {
      auto ch = static_cast<CharValue&>(*other.get()).ch;
      return std::make_unique<StringValue>(string + ch);
   } else if (other->type == ValueType::string) {
      auto other_str = static_cast<StringValue&>(*other.get()).string;
      return std::make_unique<StringValue>(string + other_str);
   }
   return std::make_unique<NullValue>();
}

Value StringValue::subtract(Value& other) const {
   fmt::raise("Invalid binary operation: 'String' - 'Any'.");
}

Value StringValue::multiply(Value& other) const {
   if (other->type == ValueType::number) {
      auto number = static_cast<NumberValue&>(*other.get()).number;
      std::string final;

      for (int i = 0; i < number; ++i) {
         final += string;
      }
      return std::make_unique<StringValue>(final);
   } else if (other->type == ValueType::character) {
      auto ch = static_cast<CharValue&>(*other.get()).ch;
      std::string final;

      for (int i = 0; i < ch; ++i) {
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
