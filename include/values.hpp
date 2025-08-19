#ifndef VALUES_HPP
#define VALUES_HPP

#include <memory>
#include <string>

enum class ValueType : char {
   identifier, number, character, string, boolean, null
};

constexpr std::string_view value_type_str[] {
   "Identifier", "Number", "Character", "String", "Boolean", "Null"   
};

struct ValueLiteral;
using Value = std::unique_ptr<ValueLiteral>;

template<class T>
const T& get_value(const Value& value) {
   return static_cast<const T&>(*value.get());
}

template<class T>
T& get_value(Value& value) {
   return static_cast<T&>(*value.get());
}

struct ValueLiteral {
   ValueType type;

   ValueLiteral(ValueType type);
   virtual ~ValueLiteral() = default;

   void print() const;
   virtual std::string as_string() const = 0;
   virtual long double as_number() const = 0;
   virtual char as_char() const = 0;
   virtual bool as_bool() const = 0;
   virtual Value copy() const = 0;

   Value negate() const;
   Value increment() const;
   Value decrement() const;
   Value add(Value& other) const;
   Value subtract(Value& other) const;
   Value multiply(Value& other) const;
   Value divide(Value& other) const;
   Value remainder(Value& other) const;
   Value exponentiate(Value& other) const;
   bool equal(Value& other) const;
   bool greater(Value& other, const std::string& op) const;
};

struct IdentValue : public ValueLiteral {
   std::string identifier;

   IdentValue(const std::string& identifier);
   static Value make(const std::string& identifier) {
      return std::make_unique<IdentValue>(identifier);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

struct NumberValue : public ValueLiteral {
   long double number;

   NumberValue(long double number);
   static Value make(long double number) {
      return std::make_unique<NumberValue>(number);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

struct CharValue : public ValueLiteral {
   char ch;

   CharValue(char ch);
   static Value make(char ch) {
      return std::make_unique<CharValue>(ch);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

struct StringValue : public ValueLiteral {
   std::string string;

   StringValue(const std::string& string);
   static Value make(const std::string& string) {
      return std::make_unique<StringValue>(string);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

struct BoolValue : public ValueLiteral {
   bool value;

   BoolValue(bool value);
   static Value make(bool value) {
      return std::make_unique<BoolValue>(value);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

struct NullValue : public ValueLiteral {
   NullValue();
   static Value make() {
      return std::make_unique<NullValue>();
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

#endif
