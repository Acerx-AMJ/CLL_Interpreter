#include "properties.hpp"

// Includes

#include "environment.hpp"
#include "fmt.hpp"
#include "interpreter.hpp"

// Properties

namespace prop {
   // Property map

   using Key = std::pair<std::string, char>;
   using Val = std::pair<Value, bool>;

   struct PairHash {
      std::size_t operator()(const Key& pair) const {
         return std::hash<std::string>{}(pair.first) ^ (std::hash<char>{}(pair.second) << 1);
      }
   };

   static std::unordered_map<Key, Val, PairHash> properties {};

   // Property functions

   void create(ValueType type, const std::string& name, const std::function<Value(std::vector<Value>&, Environment*, int)>& function, bool overrides) {
      properties.insert({Key{name, char(type)}, Val{NativeFn::make(function, name, err::nline), overrides}});
   }

   void init() {
      // Types

      constexpr auto array_v = ValueType::array;
      constexpr auto string_v = ValueType::string;

      // Array properties

      create(array_v, "push"s, array_push, true);
      create(array_v, "insert"s, array_insert, true);
      create(array_v, "erase"s, array_erase, true);
      create(array_v, "pop"s, array_pop, false);
      create(array_v, "size"s, array_size, false);
      create(array_v, "empty"s, array_empty, false);
      create(array_v, "get"s, array_get, false);
      create(array_v, "set"s, array_set, true);
      create(array_v, "find"s, array_find, false);
      create(array_v, "find_all"s, array_find_all, false);
      create(array_v, "contains"s, array_contains, false);
      create(array_v, "count"s, array_count, false);
      create(array_v, "in"s, array_in, false);
      create(array_v, "first"s, array_first, false);
      create(array_v, "last"s, array_last, false);
      create(array_v, "clear"s, array_clear, true);
      create(array_v, "fill"s, array_fill, true);
      create(array_v, "join"s, array_join, false);
      create(array_v, "reverse"s, array_reverse, false);
      create(array_v, "slice"s, array_slice, false);
      create(array_v, "foreach"s, array_foreach, true);

      // String properties

      create(string_v, "push"s, string_push, true);
      create(string_v, "insert"s, string_insert, true);
      create(string_v, "erase"s, string_erase, true);
      create(string_v, "pop"s, string_pop, false);
      create(string_v, "size"s, string_size, false);
      create(string_v, "empty"s, string_empty, false);
      create(string_v, "get"s, string_get, false);
      create(string_v, "set"s, string_set, true);
      create(string_v, "find"s, string_find, false);
      create(string_v, "find_all"s, string_find_all, false);
      create(string_v, "contains"s, string_contains, false);
      create(string_v, "count"s, string_count, false);
      create(string_v, "in"s, string_in, false);
      create(string_v, "first"s, string_first, false);
      create(string_v, "last"s, string_last, false);
      create(string_v, "clear"s, string_clear, true);
      create(string_v, "fill"s, string_fill, true);
      create(string_v, "reverse"s, string_reverse, false);
      create(string_v, "slice"s, string_slice, false);
      create(string_v, "upper"s, string_upper, false);
      create(string_v, "lower"s, string_lower, false);
      create(string_v, "swapcase"s, string_swapcase, false);
      create(string_v, "trim"s, string_trim, false);
      create(string_v, "normalize"s, string_normalize, false);
      create(string_v, "pad_start"s, string_pad_start, false);
      create(string_v, "pad_end"s, string_pad_end, false);
      create(string_v, "repeat"s, string_repeat, false);
      create(string_v, "replace"s, string_replace, false);
      create(string_v, "replace_all"s, string_replace_all, false);
      create(string_v, "split"s, string_split, false);
      create(string_v, "starts_with"s, string_starts_with, false);
      create(string_v, "ends_with"s, string_ends_with, false);
      create(string_v, "foreach"s, string_foreach, true);
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

      save(args, array.copy(), env, line);
      return array.copy();
   }

   Value array_insert(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() < 4, "'Array.insert': Expected at least 2 arguments.");
      auto& array = get_value<Array>(args.at(1));
      auto index = args.at(2)->as_number();

      fmt::raise_if(line, index < 0 || index > array.array.size(), "'Array.insert': Expected first argument to be in range 0-{} including, instead it is {}.", array.array.size(), index);
      for (int i = 0; i < args.size() - 3; ++i) {
         array.array.insert(array.array.begin() + index + i, std::move(args.at(i + 3)));
      }

      save(args, array.copy(), env, line);
      return array.copy();
   }

   Value array_erase(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'Array.erase': Expected 1 or 2 arguments.");
      auto& array = get_value<Array>(args.at(1));
      auto begin = args.at(2)->as_number();
      auto end = (args.size() == 3 ? begin : args.at(3)->as_number());

      fmt::raise_if(line, begin < 0 || begin > array.array.size(), "'Array.erase': Expected first argument to be in range 0-{} including, instead it is {}.", array.array.size(), begin);
      fmt::raise_if(line, end < 0 || end > array.array.size(), "'Array.erase': Expected second argument to be in range 0-{} including, instead it is {}.", array.array.size(), end);
      if (begin > end) {
         std::swap(begin, end);
      }      
      array.array.erase(array.array.begin() + begin, array.array.begin() + end + 1);

      save(args, array.copy(), env, line);
      return array.copy();
   }

   Value array_pop(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'Array.pop': Expected no arguments.");
      auto& array = get_value<Array>(args.at(1));

      fmt::raise_if(line, array.array.empty(), "'Array.pop': Expected array to not be empty.");
      Value last = std::move(array.array.back());
      array.array.pop_back();

      save(args, array.copy(), env, line);
      return std::move(last);
   }

   Value array_size(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'Array.size': Expected no arguments.");
      auto& array = get_value<Array>(args.at(1));
      return NumberValue::make(array.array.size(), line);
   }

   Value array_empty(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'Array.empty': Expected no arguments.");
      auto& array = get_value<Array>(args.at(1));
      return BoolValue::make(array.array.empty(), line);
   }

   Value array_get(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'Array.get': Expected a single argument.");
      auto& array = get_value<Array>(args.at(1));
      auto index = args.at(2)->as_number();

      fmt::raise_if(line, index >= array.array.size() || index < 0, "'Array.get': Index out of bounds. Array size is {}, while index is {}.", array.array.size(), index);
      return array.array.at(index)->copy();
   }

   Value array_set(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'Array.set': Expected 1 or 2 arguments.");
      auto& array = get_value<Array>(args.at(1));

      auto index = args.at(2)->as_number();
      fmt::raise_if(line, index >= array.array.size() || index < 0, "'Array.set': Index out of bounds. Array size is {}, while index is {}.", array.array.size(), index);
      auto value = (args.size() == 3 ? NullValue::make(line) : std::move(args.at(3)));

      array.array.at(index) = std::move(value);
      save(args, array.copy(), env, line);
      return array.copy();
   }

   Value array_find(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'Array.find': Expected 1 or 2 arguments.");
      auto& array = get_value<Array>(args.at(1));
      auto& target = args.at(2);

      auto start = (args.size() == 3 ? 0 : args.at(3)->as_number());
      fmt::raise_if(line, start < 0 || start > array.array.size(), "'Array.find': Expected second argument to be in range 0-{} including, instead it is {}.", array.array.size(), start);

      for (int i = start; i < array.array.size(); ++i) {
         if (target->equal(array.array.at(i))) {
            return NumberValue::make(i, line);
         }
      }
      return NullValue::make(line);
   }

   Value array_find_all(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'Array.find_all': Expected 1 or 2 arguments.");
      auto& array = get_value<Array>(args.at(1));
      auto& target = args.at(2);

      auto index_array = std::make_unique<Array>(std::vector<Value>{}, line);
      auto start = (args.size() == 3 ? 0 : args.at(3)->as_number());
      fmt::raise_if(line, start < 0 || start > array.array.size(), "'Array.find_all': Expected second argument to be in range 0-{} including, instead it is {}.", array.array.size(), start);

      for (int i = start; i < array.array.size(); ++i) {
         if (target->equal(array.array.at(i))) {
            index_array->array.push_back(NumberValue::make(i, line));
         }
      }
      return std::move(index_array);
   }

   Value array_contains(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'Array.contains': Expected 1 or 2 arguments.");
      auto& array = get_value<Array>(args.at(1));
      auto& target = args.at(2);

      auto start = (args.size() == 3 ? 0 : args.at(3)->as_number());
      fmt::raise_if(line, start < 0 || start > array.array.size(), "'Array.contains': Expected second argument to be in range 0-{} including, instead it is {}.", array.array.size(), start);

      for (int i = start; i < array.array.size(); ++i) {
         if (target->equal(array.array.at(i))) {
            return BoolValue::make(true, line);
         }
      }
      return BoolValue::make(false, line);
   }

   Value array_count(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'Array.count': Expected 1 or 2 arguments.");
      auto& array = get_value<Array>(args.at(1));
      auto& target = args.at(2);

      int count = 0;
      auto start = (args.size() == 3 ? 0 : args.at(3)->as_number());
      fmt::raise_if(line, start < 0 || start > array.array.size(), "'Array.count': Expected second argument to be in range 0-{} including, instead it is {}.", array.array.size(), start);

      for (int i = start; i < array.array.size(); ++i) {
         count += (target->equal(array.array.at(i)));
      }
      return NumberValue::make(count, line);
   }

   Value array_in(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'Array.in': Expected a single argument.");
      auto& array = get_value<Array>(args.at(1));
      auto index = args.at(2)->as_number();
      return BoolValue::make(index < array.array.size() && index >= 0, line);
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
      save(args, array->copy(), env, line);
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
      save(args, array.copy(), env, line);
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

   Value array_reverse(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'Array.reverse': Expected no arguments.");
      auto& array = get_value<Array>(args.at(1));

      auto new_array = std::make_unique<Array>(std::vector<Value>{}, line);
      for (int i = array.array.size() - 1; i >= 0; --i) {
         new_array->array.push_back(array.array.at(i)->copy());
      }
      return std::move(new_array);
   }

   Value array_slice(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'Array.slice': Expected 1 or 2 arguments.");
      auto& array = get_value<Array>(args.at(1));
      auto begin = args.at(2)->as_number();
      auto end = (args.size() == 3 ? array.array.size() - 1 : args.at(3)->as_number());

      fmt::raise_if(line, begin < 0 || begin >= array.array.size(), "'Array.slice': Expected first argument to be in range 0-{} including, instead it is {}.", array.array.size() - 1, begin);
      fmt::raise_if(line, end < 0 || end >= array.array.size(), "'Array.slice': Expected second argument to be in range 0-{} including, instead it is {}.", array.array.size() - 1, end);

      bool bigger = (begin > end);
      auto new_array = std::make_unique<Array>(std::vector<Value>{}, line);

      for (int i = begin; (bigger ? i >= end : i <= end); (bigger ? --i : ++i)) {
         new_array->array.push_back(array.array.at(i)->copy());
      }
      return std::move(new_array);
   }

   Value array_foreach(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'Array.foreach': Expected 1 argument.");
      auto& array = get_value<Array>(args.at(1));

      fmt::raise_if(line, args.at(2)->type != ValueType::fn && args.at(2)->type != ValueType::native_fn, "'Array.foreach': Expected first argument to be callable, instead it is {}.", value_type_str[int(args.at(2)->type)]);
      Interpreter interpreter;

      for (int i = 0; i < array.size(); ++i) {
         std::vector<Value> arguments;
         arguments.push_back(array.array.at(i)->copy());
         
         auto result = interpreter.call_function(*env, args.at(2)->copy(), arguments, line);
         array.array.at(i) = std::move(result);
      }
      
      save(args, array.copy(), env, line);
      return array.copy();
   }

   // String functions

   Value string_push(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() < 3, "'String.push': Expected at least a single argument.");
      auto& string = get_value<StringValue>(args.at(1));

      for (int i = 2; i < args.size(); ++i) {
         string.string.push_back(args.at(i)->as_char());
      }
      save(args, string.copy(), env, line);
      return string.copy();
   }

   Value string_insert(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() < 4, "'String.insert': Expected at least 2 arguments.");
      auto& string = get_value<StringValue>(args.at(1));
      auto index = args.at(2)->as_number();

      fmt::raise_if(line, index < 0 || index > string.string.size(), "'String.insert': Expected first argument to be in range 0-{} including, instead it is {}.", string.string.size(), index);
      std::vector<char> chars;

      for (int i = 3; i < args.size(); ++i) {
         const auto& arg = args.at(i);
         if (arg->type == ValueType::string) {
            for (const auto& ch : get_value<StringValue>(arg).string) {
               chars.push_back(ch);
            }
         } else {
            chars.push_back(arg->as_char());
         }
      }
      
      for (int i = 0; i < chars.size(); ++i) {
         string.string.insert(string.string.begin() + index + i, chars.at(i));
      }

      save(args, string.copy(), env, line);
      return string.copy();
   }

   Value string_erase(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'String.erase': Expected 1 or 2 arguments.");
      auto& string = get_value<StringValue>(args.at(1));
      auto begin = args.at(2)->as_number();
      auto end = (args.size() == 3 ? begin : args.at(3)->as_number());

      fmt::raise_if(line, begin < 0 || begin > string.string.size(), "'String.erase': Expected first argument to be in range 0-{} including, instead it is {}.", string.string.size(), begin);
      fmt::raise_if(line, end < 0 || end > string.string.size(), "'String.erase': Expected second argument to be in range 0-{} including, instead it is {}.", string.string.size(), end);
      if (begin > end) {
         std::swap(begin, end);
      }
      string.string.erase(string.string.begin() + begin, string.string.begin() + end + 1);

      save(args, string.copy(), env, line);
      return string.copy();
   }

   Value string_pop(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'String.pop': Expected no arguments.");
      auto& string = get_value<StringValue>(args.at(1));

      fmt::raise_if(line, string.string.empty(), "'String.pop': Expected string to not be empty.");
      char last = std::move(string.string.back());
      string.string.pop_back();

      save(args, string.copy(), env, line);
      return CharValue::make(last, line);
   }

   Value string_size(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'String.size': Expected no arguments.");
      auto& string = get_value<StringValue>(args.at(1));
      return NumberValue::make(string.string.size(), line);
   }

   Value string_empty(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'String.empty': Expected no arguments.");
      auto& string = get_value<StringValue>(args.at(1));
      return BoolValue::make(string.string.empty(), line);
   }

   Value string_get(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'String.get': Expected a single argument.");
      auto& string = get_value<StringValue>(args.at(1));
      auto index = args.at(2)->as_number();

      fmt::raise_if(line, index >= string.string.size() || index < 0, "'String.get': Index out of bounds. String size is {}, while index is {}.", string.string.size(), index);
      return CharValue::make(string.string.at(index), line);
   }

   Value string_set(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'String.set': Expected 1 or 2 arguments.");
      auto& string = get_value<StringValue>(args.at(1));

      auto index = args.at(2)->as_number();
      fmt::raise_if(line, index >= string.string.size() || index < 0, "'String.set': Index out of bounds. String size is {}, while index is {}.", string.string.size(), index);
      auto value = (args.size() == 3 ? CharValue::make(char{}, line) : std::move(args.at(3)));

      string.string.at(index) = value->as_char();
      save(args, string.copy(), env, line);
      return string.copy();
   }

   Value string_find(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'String.find': Expected 1 or 2 arguments.");
      auto& string = get_value<StringValue>(args.at(1));
      auto target = args.at(2)->as_string();

      auto start = (args.size() == 3 ? 0 : args.at(3)->as_number());
      fmt::raise_if(line, start < 0 || start > string.string.size(), "'String.find': Expected second argument to be in range 0-{}, instead it is {}.", string.string.size(), start);

      auto find = string.string.find(target, start);
      return (find == std::string::npos ? NullValue::make(line) : NumberValue::make(find, line));
   }

   Value string_find_all(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'String.find_all': Expected 1 or 2 arguments.");
      auto& string = get_value<StringValue>(args.at(1));
      auto target = args.at(2)->as_string();

      auto start = (args.size() == 3 ? 0 : args.at(3)->as_number());
      fmt::raise_if(line, start < 0 || start > string.string.size(), "'String.find_all': Expected second argument to be in range 0-{}, instead it is {}.", string.string.size(), start);

      auto index_array = std::make_unique<Array>(std::vector<Value>{}, line);
      auto find = string.string.find(target, start);

      while (find != std::string::npos) {
         index_array->array.push_back(NumberValue::make(find, line));
         find = string.string.find(target, find + target.size());
      }
      return std::move(index_array);
   }

   Value string_contains(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'String.contains': Expected 1 or 2 arguments.");
      auto& string = get_value<StringValue>(args.at(1));
      auto target = args.at(2)->as_string();

      auto start = (args.size() == 3 ? 0 : args.at(3)->as_number());
      fmt::raise_if(line, start < 0 || start > string.string.size(), "'String.contains': Expected second argument to be in range 0-{}, instead it is {}.", string.string.size(), start);

      return BoolValue::make(string.string.find(target, start) != std::string::npos, line);
   }

   Value string_count(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'String.count': Expected 1 or 2 arguments.");
      auto& string = get_value<StringValue>(args.at(1));
      auto target = args.at(2)->as_string();

      auto start = (args.size() == 3 ? 0 : args.at(3)->as_number());
      fmt::raise_if(line, start < 0 || start > string.string.size(), "'String.count': Expected second argument to be in range 0-{}, instead it is {}.", string.string.size(), start);

      auto find = string.string.find(target, start);
      int count = 0;

      while (find != std::string::npos) {
         ++count;
         find = string.string.find(target, find + target.size());
      }
      return NumberValue::make(count, line);
   }

   Value string_in(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'String.in': Expected a single argument.");
      auto& string = get_value<StringValue>(args.at(1));
      auto index = args.at(2)->as_number();
      return BoolValue::make(index < string.string.size() && index >= 0, line);
   }

   Value string_first(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "String.first: Expected no arguments.");
      auto& string = get_value<StringValue>(args.at(1));
      fmt::raise_if(line, string.string.empty(), "'String.first': Expected string to not be empty.");
      return CharValue::make(string.string.front(), line);
   }

   Value string_last(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "String.last: Expected no arguments.");
      auto& string = get_value<StringValue>(args.at(1));
      fmt::raise_if(line, string.string.empty(), "'String.first': Expected string to not be empty.");
      return CharValue::make(string.string.back(), line);
   }

   Value string_clear(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'String.clear': Expected no arguments.");
      auto string = std::make_unique<StringValue>("", args.at(1)->line);
      save(args, string->copy(), env, line);
      return std::move(string);
   }

   Value string_fill(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'String.fill': Expected 1 or 2 arguments.");
      auto& string = get_value<StringValue>(args.at(1));

      auto size = args.at(2)->as_number();
      fmt::raise_if(line, size < 0, "'String.fill': Expected first argument to be non-negative.");

      auto value = (args.size() == 3 ? " " : args.at(3)->as_string());
      string.string.clear();
      
      for (int i = 0; i < size; ++i) {
         string.string += value;
      }
      save(args, string.copy(), env, line);
      return string.copy();
   }

   Value string_reverse(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'String.reverse': Expected no arguments.");
      auto& string = get_value<StringValue>(args.at(1));

      auto new_string = std::make_unique<StringValue>("", line);
      for (int i = string.string.size() - 1; i >= 0; --i) {
         new_string->string.push_back(string.string.at(i));
      }
      return std::move(new_string);
   }

   Value string_slice(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'String.slice': Expected 1 or 2 arguments.");
      auto& string = get_value<StringValue>(args.at(1));
      auto begin = args.at(2)->as_number();
      auto end = (args.size() == 3 ? string.string.size() - 1 : args.at(3)->as_number());

      fmt::raise_if(line, begin < 0 || begin >=  string.string.size(), "'String.slice': Expected first argument to be in range 0-{} including, instead it is {}.", string.string.size() - 1, begin);
      fmt::raise_if(line, end < 0 || end >= string.string.size(), "'String.slice': Expected second argument to be in range 0-{} including, instead it is {}.", string.string.size() - 1, end);

      bool bigger = (begin > end);
      auto new_string = std::make_unique<StringValue>("", line);

      for (int i = begin; (bigger ? i >= end : i <= end); (bigger ? --i : ++i)) {
         new_string->string.push_back(string.string.at(i));
      }
      return std::move(new_string);
   }

   Value string_upper(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'String.upper': Expected no arguments.");
      auto& string = get_value<StringValue>(args.at(1));

      auto new_string = std::make_unique<StringValue>("", line);
      for (int i = 0; i < string.string.size(); i++) {
         new_string->string.push_back(toupper(string.string.at(i)));
      }
      return std::move(new_string);
   }

   Value string_lower(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'String.lower': Expected no arguments.");
      auto& string = get_value<StringValue>(args.at(1));

      auto new_string = std::make_unique<StringValue>("", line);
      for (int i = 0; i < string.string.size(); i++) {
         new_string->string.push_back(tolower(string.string.at(i)));
      }
      return std::move(new_string);
   }

   Value string_swapcase(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'String.swapcase': Expected no arguments.");
      auto& string = get_value<StringValue>(args.at(1));

      auto new_string = std::make_unique<StringValue>("", line);
      for (int i = 0; i < string.string.size(); i++) {
         auto ch = string.string.at(i);
         new_string->string.push_back((isupper(ch) ? tolower(ch) : toupper(ch)));
      }
      return std::move(new_string);
   }

   Value string_trim(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'String.trim': Expected no arguments.");
      auto& string = get_value<StringValue>(args.at(1));

      auto new_string = string.copy();
      auto& nstr = get_value<StringValue>(new_string);
      nstr.string.erase(nstr.string.begin(), std::find_if(nstr.string.begin(), nstr.string.end(), [](int c){ return !isspace(c); }));
      nstr.string.erase(std::find_if(nstr.string.rbegin(), nstr.string.rend(), [](int c){ return !isspace(c); }).base(), nstr.string.end());
      return std::move(new_string);
   }

   Value string_normalize(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'String.normalize': Expected no arguments.");
      auto& string = get_value<StringValue>(args.at(1));
      auto new_string = std::make_unique<StringValue>("", line);
      bool last_space = true;

      for (const auto& ch : string.string) {
         bool is_space = isspace(ch);

         if (last_space && is_space) {
            continue;
         } else if (last_space && !is_space) {
            last_space = false;
         } else if (is_space) {
            last_space = true;
         }
         new_string->string.push_back(ch);
      }

      if (!new_string->string.empty() && isspace(new_string->string.back())) {
         new_string->string.pop_back();
      }
      return std::move(new_string);
   }

   Value string_pad_start(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'String.pad_start': Expected 1 or 2 arguments.");
      auto& string = get_value<StringValue>(args.at(1));

      auto new_string = string.copy();
      auto& nstr = get_value<StringValue>(new_string);
      auto size = args.at(2)->as_number();
      auto ch = (args.size() == 3 ? " " : args.at(3)->as_string());

      while (nstr.string.size() < size) {
         nstr.string = ch + nstr.string;
      }
      return std::move(new_string);
   }

   Value string_pad_end(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3 && args.size() != 4, "'String.pad_end': Expected 1 or 2 arguments.");
      auto& string = get_value<StringValue>(args.at(1));

      auto new_string = string.copy();
      auto& nstr = get_value<StringValue>(new_string);
      auto size = args.at(2)->as_number();
      auto ch = (args.size() == 3 ? " " : args.at(3)->as_string());

      while (nstr.string.size() < size) {
         nstr.string += ch;
      }
      return std::move(new_string);
   }

   Value string_repeat(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'String.repeat': Expected 1 argument.");
      auto& string = get_value<StringValue>(args.at(1));
      auto new_string = std::make_unique<StringValue>("", line);
      auto count = args.at(2)->as_number();

      for (int i = 0; i < count; ++i) {
         new_string->string += string.string;
      }
      return std::move(new_string);
   }

   Value string_replace(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 4 && args.size() != 5, "'String.replace': Expected 2 or 3 arguments.");
      auto& string = get_value<StringValue>(args.at(1));

      auto target = args.at(2)->as_string();
      auto replace = args.at(3)->as_string();
      auto new_string = std::make_unique<StringValue>(string.string, line);

      auto start = (args.size() == 4 ? 0 : args.at(4)->as_number());
      fmt::raise_if(line, start < 0 || start >= string.string.size(), "Expected first argument to be in range 0-{} including, instead it is {}.", string.string.size() - 1, start);

      if (auto find = new_string->string.find(target, start); find != std::string::npos) {
         new_string->string = new_string->string.replace(find, target.size(), replace);
      }
      return std::move(new_string);
   }

   Value string_replace_all(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 4 && args.size() != 5, "'String.replace_all': Expected 2 or 3 arguments.");
      auto& string = get_value<StringValue>(args.at(1));

      auto target = args.at(2)->as_string();
      auto replace = args.at(3)->as_string();
      auto new_string = std::make_unique<StringValue>(string.string, line);

      auto start = (args.size() == 4 ? 0 : args.at(4)->as_number());
      fmt::raise_if(line, start < 0 || start >= string.string.size(), "Expected first argument to be in range 0-{} including, instead it is {}.", string.string.size() - 1, start);

      auto find = new_string->string.find(target, start);
      while (find != std::string::npos) {
         new_string->string = new_string->string.replace(find, target.size(), replace);
         find = new_string->string.find(target, find + target.size());
      }
      return std::move(new_string);
   }

   Value string_split(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'String.split': Expected 1 argument.");
      auto& string = get_value<StringValue>(args.at(1));
      auto target = args.at(2)->as_string();

      auto array = std::make_unique<Array>(std::vector<Value>{}, line);
      auto find = string.string.find(target, 0), last = 0ul;

      while (find != std::string::npos) {
         array->array.push_back(StringValue::make(string.string.substr(last, find - last), line));
         last = find + target.size();
         find = string.string.find(target, find + target.size());
      }
      array->array.push_back(StringValue::make(string.string.substr(last, find - last), line));
      return std::move(array);
   }

   Value string_starts_with(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'String.starts_with': Expected 1 argument.");
      auto& string = get_value<StringValue>(args.at(1));
      auto prefix = args.at(2)->as_string();
      return BoolValue::make(string.string.rfind(prefix, 0) == 0, line);
   }

   Value string_ends_with(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'String.ends_with': Expected 1 argument.");
      auto& string = get_value<StringValue>(args.at(1));
      auto suffix = args.at(2)->as_string();
      return BoolValue::make(string.string.rfind(suffix) == string.string.size() - suffix.size(), line);
   }

   Value string_foreach(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 3, "'String.foreach': Expected 1 argument.");
      auto& string = get_value<StringValue>(args.at(1));

      fmt::raise_if(line, args.at(2)->type != ValueType::fn && args.at(2)->type != ValueType::native_fn, "'String.foreach': Expected first argument to be callable, instead it is '{}'.", value_type_str[int(args.at(2)->type)]);
      Interpreter interpreter;

      for (int i = 0; i < string.size(); ++i) {
         std::vector<Value> arguments;
         arguments.push_back(CharValue::make(string.string.at(i), line));

         auto result = interpreter.call_function(*env, args.at(2)->copy(), arguments, line);
         string.string.at(i) = result->as_char();
      }

      save(args, string.copy(), env, line);
      return string.copy();
   }

   // Helper functions

   void save(std::vector<Value>& args, Value value, Environment* env, int line) {
      if (args.at(0)->type == ValueType::identifier) {
         auto identifier = get_value<IdentValue>(args.at(0));
         env->assign_variable(identifier.identifier, std::move(value), line);
      }
   }
}
