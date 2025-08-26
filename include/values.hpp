#ifndef VALUES_HPP
#define VALUES_HPP

#include "ast.hpp"
#include <functional>
#include <memory>
#include <string>

enum class ValueType : char {
   identifier, number, character, string, boolean, array, native_fn, fn, null
};

constexpr std::string_view value_type_str[] {
   "Identifier", "Number", "Character", "String", "Boolean", "Array", "NativeFunction", "Function", "Null"   
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
   int line = 0;

   ValueLiteral(ValueType type, int line);
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

   IdentValue(const std::string& identifier, int line);
   static Value make(const std::string& identifier, int line) {
      return std::make_unique<IdentValue>(identifier, line);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

struct NumberValue : public ValueLiteral {
   long double number;

   NumberValue(long double number, int line);
   static Value make(long double number, int line) {
      return std::make_unique<NumberValue>(number, line);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

struct CharValue : public ValueLiteral {
   char ch;

   CharValue(char ch, int line);
   static Value make(char ch, int line) {
      return std::make_unique<CharValue>(ch, line);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

struct StringValue : public ValueLiteral {
   std::string string;

   StringValue(const std::string& string, int line);
   static Value make(const std::string& string, int line) {
      return std::make_unique<StringValue>(string, line);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

struct BoolValue : public ValueLiteral {
   bool value;

   BoolValue(bool value, int line);
   static Value make(bool value, int line) {
      return std::make_unique<BoolValue>(value, line);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

struct Array : public ValueLiteral {
   std::vector<Value> array;

   Array(std::vector<Value> array, int line);
   static Value make(std::vector<Value> array, int line) {
      return std::make_unique<Array>(std::move(array), line);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

struct NativeFn : public ValueLiteral {
   using Func = std::function<Value(const std::vector<Value>&, int)>;
   Func call;
   std::string identifier;

   NativeFn(const Func& call, const std::string& identifier, int line);
   static Value make(const Func& call, const std::string& identifier, int line) {
      return std::make_unique<NativeFn>(call, identifier, line);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

class Environment;

struct Function : public ValueLiteral {
   std::string identifier;
   std::vector<std::string> parameters;
   std::vector<Value> parameter_def;
   std::string returns;
   Value return_def;
   Environment* env;
   Stmt body;
   int def_args;

   Function(const std::string& identifier, const std::vector<std::string>& parameters, std::vector<Value> parameter_def, const std::string& returns, Value return_def, Environment* env, Stmt body, int def_args, int line);
   static Value make(const std::string& identifier, const std::vector<std::string>& parameters, std::vector<Value> parameter_def, const std::string& returns, Value return_def, Environment* env, Stmt body, int def_args, int line) {
      return std::make_unique<Function>(identifier, parameters, std::move(parameter_def), returns, std::move(return_def), env, std::move(body), def_args, line);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

struct NullValue : public ValueLiteral {
   NullValue(int line = -1);
   static Value make(int line = -1) {
      return std::make_unique<NullValue>(line);
   }

   std::string as_string() const override;
   long double as_number() const override;
   char as_char() const override;
   bool as_bool() const override;
   Value copy() const override;
};

#endif
