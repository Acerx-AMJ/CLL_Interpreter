#include "properties.hpp"

#include "environment.hpp"
#include "fmt.hpp"

namespace prop {
   // Property map

   using Key = std::pair<std::string, char>;
   using Val = std::pair<Value, bool>;
   constexpr char array_v = char(ValueType::array);

   struct PairHash {
      std::size_t operator()(const Key& pair) const {
         return std::hash<std::string>{}(pair.first) ^ (std::hash<char>{}(pair.second) << 1);
      }
   };

   static std::unordered_map<Key, Val, PairHash> properties {};

   // Property functions

   void init() {
      properties.insert({Key{"push", array_v}, Val{NativeFn::make(array_push, "push", err::nline), true}});
      properties.insert({Key{"pop", array_v}, Val{NativeFn::make(array_pop, "pop", err::nline), false}});
      properties.insert({Key{"size", array_v}, Val{NativeFn::make(array_size, "size", err::nline), false}});
      properties.insert({Key{"empty", array_v}, Val{NativeFn::make(array_empty, "empty", err::nline), false}});
      properties.insert({Key{"at", array_v}, Val{NativeFn::make(array_at, "at", err::nline), false}});
      properties.insert({Key{"find", array_v}, Val{NativeFn::make(array_find, "find", err::nline), false}});
      properties.insert({Key{"find_all", array_v}, Val{NativeFn::make(array_find_all, "find_all", err::nline), false}});
      properties.insert({Key{"contains", array_v}, Val{NativeFn::make(array_contains, "contains", err::nline), false}});
      properties.insert({Key{"in_bounds", array_v}, Val{NativeFn::make(array_in_bounds, "in_bounds", err::nline), false}});
      properties.insert({Key{"first", array_v}, Val{NativeFn::make(array_first, "first", err::nline), false}});
      properties.insert({Key{"last", array_v}, Val{NativeFn::make(array_last, "last", err::nline), false}});
      properties.insert({Key{"clear", array_v}, Val{NativeFn::make(array_clear, "clear", err::nline), true}});
      properties.insert({Key{"fill", array_v}, Val{NativeFn::make(array_fill, "fill", err::nline), true}});
      properties.insert({Key{"join", array_v}, Val{NativeFn::make(array_join, "join", err::nline), false}});
   }

   bool exists(const std::string& name, ValueType type) {
      return properties.find(Key{name, char(type)}) != properties.end();
   }

   bool overrides(const std::string& name, ValueType type) {
      return properties[Key{name, char(type)}].second;
   }

   Value get(const std::string& name, ValueType type) {
      return properties[Key{name, char(type)}].first->copy();
   }

   // Property functions
   // 0 - identifier or null, 1 - left value, 2... - rest of the arguments

   // Array functions

   Value array_push(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() < 3, "'Array.push': Expected at least a single argument.");
      auto& array = get_value<Array>(args.at(1));

      for (int i = 2; i < args.size(); ++i) {
         array.array.push_back(std::move(args.at(i)));
      }

      if (args.at(0)->type == ValueType::identifier) {
         auto identifier = get_value<IdentValue>(args.at(0));
         env->assign_variable(identifier.identifier, array.copy(), line);
      }
      return array.copy();
   }

   Value array_pop(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'Array.pop': Expected no arguments.");
      auto& array = get_value<Array>(args.at(1));

      fmt::raise_if(line, array.array.empty(), "'Array.pop': Expected array to not be empty.");
      Value last = std::move(array.array.back());
      array.array.pop_back();

      if (args.at(0)->type == ValueType::identifier) {
         auto identifier = get_value<IdentValue>(args.at(0));
         env->assign_variable(identifier.identifier, array.copy(), line);
      }
      return std::move(last);
   }

   Value array_size(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'Array.size': Expected no arguments.");
      auto& array = get_value<Array>(args.at(1));
      return NumberValue::make(array.array.size(), array.line);
   }

   Value array_empty(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'Array.empty': Expected no arguments.");
      auto& array = get_value<Array>(args.at(1));
      return BoolValue::make(array.array.empty(), array.line);
   }

   Value array_at(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'Array.at': Expected a single argument.");
      auto& array = get_value<Array>(args.at(1));
      auto index = args.at(2)->as_number();

      fmt::raise_if(line, index >= array.array.size() || index < 0, "'Array.at': Index out of bounds. Array size is {}, while index is {}.", array.array.size(), index);
      return array.array.at(index)->copy();
   }

   Value array_find(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'Array.find': Expected a single argument.");
      auto& array = get_value<Array>(args.at(1));
      auto& target = args.at(2);

      for (int i = 0; i < array.array.size(); ++i) {
         auto& element = array.array.at(i);
         if (target->type == element->type && target->equal(element)) {
            return NumberValue::make(i, line);
         }
      }
      return NullValue::make(line);
   }

   Value array_find_all(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'Array.find_all': Expected a single argument.");
      auto& array = get_value<Array>(args.at(1));
      auto& target = args.at(2);

      auto index_array = std::make_unique<Array>(std::vector<Value>{}, line);

      for (int i = 0; i < array.array.size(); ++i) {
         auto& element = array.array.at(i);
         if (target->type == element->type && target->equal(element)) {
            index_array->array.push_back(NumberValue::make(i, line));
         }
      }
      return (index_array->array.empty() ? NullValue::make(line) : std::move(index_array));
   }

   Value array_contains(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'Array.contains': Expected a single argument.");
      auto& array = get_value<Array>(args.at(1));
      auto& target = args.at(2);

      for (int i = 0; i < array.array.size(); ++i) {
         auto& element = array.array.at(i);
         if (target->type == element->type && target->equal(element)) {
            return BoolValue::make(true, line);
         }
      }
      return BoolValue::make(false, line);
   }

   Value array_in_bounds(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'Array.in_bounds': Expected a single argument.");
      auto& array = get_value<Array>(args.at(1));
      auto index = args.at(2)->as_number();
      return BoolValue::make(index < array.array.size() && index >= 0, array.line);
   }

   Value array_first(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'Array.first': Expected no arguments.");
      auto& array = get_value<Array>(args.at(1));
      fmt::raise_if(line, array.array.empty(), "'Array.first': Expected array to not be empty.");
      return array.array.front()->copy();
   }

   Value array_last(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'Array.last': Expected no arguments.");
      auto& array = get_value<Array>(args.at(1));
      fmt::raise_if(line, array.array.empty(), "'Array.last': Expected array to not be empty.");
      return array.array.back()->copy();
   }

   Value array_clear(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'Array.clear': Expected no arguments");
      auto array = std::make_unique<Array>(std::vector<Value>{}, args.at(1)->line);

      if (args.at(0)->type == ValueType::identifier) {
         auto identifier = get_value<IdentValue>(args.at(0));
         env->assign_variable(identifier.identifier, array->copy(), line);
      }
      return std::move(array);
   }

   Value array_fill(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'Array.fill': Expected 1 or 2 arguments.");
      auto& array = get_value<Array>(args.at(1));

      auto size = args.at(2)->as_number();
      fmt::raise_if(line, size < 0, "'Array.fill': Expected first argument to be non-negative.");

      auto value = (args.size() == 3 ? NullValue::make() : std::move(args.at(3)));
      array.array.clear();

      for (int i = 0; i < size; ++i) {
         array.array.push_back(value->copy());
      }

      if (args.at(0)->type == ValueType::identifier) {
         auto identifier = get_value<IdentValue>(args.at(0));
         env->assign_variable(identifier.identifier, array.copy(), line);
      }
      return array.copy();
   }

   Value array_join(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2 && args.size() != 3, "'Array.join': Expected no arguments or a single argument.");
      auto& array = get_value<Array>(args.at(1));
      auto separator = (args.size() == 2 ? ""s : args.at(2)->as_string());

      std::string result;
      for (int i = 0; i < array.array.size(); ++i) {
         result += array.array.at(i)->as_string();
         if (i + 1 < array.array.size()) {
            result += separator;
         }
      }
      return StringValue::make(std::move(result), line);
   }
}
