#ifndef VALUES_HPP
#define VALUES_HPP

#include <memory>
#include <string>

enum class ValueType : char {
   identifier, number, character, string, null
};

constexpr std::string_view value_type_str[] {
   "Identifier", "Number", "Character", "String", "Null"   
};

struct ValueLiteral;
using Value = std::unique_ptr<ValueLiteral>;

struct ValueLiteral {
   ValueType type;

   ValueLiteral(ValueType type);
   virtual ~ValueLiteral() = default;
   virtual void print() const = 0;
   virtual std::string as_string() const = 0;

   virtual Value copy() const = 0;
   virtual Value negate() const = 0;
   virtual Value increment() = 0;
   virtual Value decrement() = 0;
   virtual Value add(Value& other) const = 0;
   virtual Value subtract(Value& other) const = 0;
   virtual Value multiply(Value& other) const = 0;
   virtual Value divide(Value& other) const = 0;
   virtual Value remainder(Value& other) const = 0;
   virtual Value exponentiate(Value& other) const = 0;
};

struct IdentifierValue : public ValueLiteral {
   std::string identifier;

   IdentifierValue(const std::string& identifier);
   void print() const override;
   std::string as_string() const override;

   Value copy() const override;
   Value negate() const override;
   Value increment() override;
   Value decrement() override;
   Value add(Value& other) const override;
   Value subtract(Value& other) const override;
   Value multiply(Value& other) const override;
   Value divide(Value& other) const override;
   Value remainder(Value& other) const override;
   Value exponentiate(Value& other) const override;
};

struct NumberValue : public ValueLiteral {
   long double number;

   NumberValue(long double number);
   void print() const override;
   std::string as_string() const override;

   Value copy() const override;
   Value negate() const override;
   Value increment() override;
   Value decrement() override;
   Value add(Value& other) const override;
   Value subtract(Value& other) const override;
   Value multiply(Value& other) const override;
   Value divide(Value& other) const override;
   Value remainder(Value& other) const override;
   Value exponentiate(Value& other) const override;
};

struct CharValue : public ValueLiteral {
   char ch;

   CharValue(char ch);
   void print() const override;
   std::string as_string() const override;

   Value copy() const override;
   Value negate() const override;
   Value increment() override;
   Value decrement() override;
   Value add(Value& other) const override;
   Value subtract(Value& other) const override;
   Value multiply(Value& other) const override;
   Value divide(Value& other) const override;
   Value remainder(Value& other) const override;
   Value exponentiate(Value& other) const override;
};

struct StringValue : public ValueLiteral {
   std::string string;

   StringValue(const std::string& string);
   void print() const override;
   std::string as_string() const override;

   Value copy() const override;
   Value negate() const override;
   Value increment() override;
   Value decrement() override;
   Value add(Value& other) const override;
   Value subtract(Value& other) const override;
   Value multiply(Value& other) const override;
   Value divide(Value& other) const override;
   Value remainder(Value& other) const override;
   Value exponentiate(Value& other) const override;
};

struct NullValue : public ValueLiteral {
   NullValue();
   void print() const override;
   std::string as_string() const override;

   Value copy() const override;
   Value negate() const override;
   Value increment() override;
   Value decrement() override;
   Value add(Value& other) const override;
   Value subtract(Value& other) const override;
   Value multiply(Value& other) const override;
   Value divide(Value& other) const override;
   Value remainder(Value& other) const override;
   Value exponentiate(Value& other) const override;
};

#endif
