#include "functions.hpp"

// Includes

#include "fmt.hpp"
#include <iostream>
#include <limits>

// Functions

namespace fun {
   // Print/Format functions

   Value print(std::vector<Value>& args, Environment* env, int line) {
      for (int i = 0; i < args.size(); ++i) {
         args.at(i)->print();
         if (i + 1 < args.size())
            std::cout << ' ';
      }
      return NullValue::make(line);
   }

   Value println(std::vector<Value>& args, Environment* env, int line) {
      auto null = print(args, env, line);
      std::cout << '\n';
      return std::move(null);
   }

   Value printf(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.empty() || args.at(0)->type != ValueType::string, "'printf': Expected at least one argument and expected the first argument to be a string.");
      std::string base = get_value<StringValue>(args.at(0)).string;
      std::vector<std::string> arguments;

      for (int i = 1; i < args.size(); ++i) {
         arguments.push_back(args.at(i)->as_string());
      }
      fmt::printf_v(base.c_str(), arguments);
      return NullValue::make(line);
   }

   Value printfln(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.empty() || args.at(0)->type != ValueType::string, "'printfln': Expected at least one argument and expected the first argument to be a string.");
      std::string base = get_value<StringValue>(args.at(0)).string;
      std::vector<std::string> arguments;

      for (int i = 1; i < args.size(); ++i) {
         arguments.push_back(args.at(i)->as_string());
      }
      fmt::printfln_v(base.c_str(), arguments);
      return NullValue::make(line);
   }

   Value format(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.empty() || args.at(0)->type != ValueType::string, "'format': Expected at least one argument and expected the first argument to be a string.");
      std::string base = get_value<StringValue>(args.at(0)).string;
      std::vector<std::string> arguments;

      for (int i = 1; i < args.size(); ++i) {
         arguments.push_back(args.at(i)->as_string());
      }
      return StringValue::make(fmt::format_v(base.c_str(), arguments), line);
   }

   // Error/exit functions

   Value raise(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.empty() || args.at(0)->type != ValueType::string, "'raise': Expected at least one argument and expected the first argument to be a string.");
      std::string base = get_value<StringValue>(args.at(0)).string;
      std::vector<std::string> arguments;

      for (int i = 1; i < args.size(); ++i) {
         arguments.push_back(args.at(i)->as_string());
      }
      fmt::raise_v(line, base.c_str(), arguments);
   }

   Value assert(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() != 2, "'assert': Expected two arguments.");
      if (!args.at(0)->as_bool()) {
         err::raise(args.at(1)->as_string(), line);
      }
      return NullValue::make(line);
   }

   Value throw_(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() > 2, "'throw': Expected at most two arguments.");
      err::raise((args.empty() ? "Error thrown with no further description." : args.at(0)->as_string()), err::nline, (args.size() < 2 ? err::nline : args.at(1)->as_number()));
   }

   Value exit(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() > 1, "'exit': Expected no arguments or a single argument.");
      err::exit((args.empty() ? 0 : args.at(0)->as_number()));
   }

   // Input functions

   Value input(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() > 1, "'input': Expected no arguments or a single argument.");
      if (!args.empty()) {
         args.at(0)->print();
      }
      std::string user_input;
      std::getline(std::cin, user_input);
      return StringValue::make(user_input, line);
   }

   Value inputnum(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() > 1, "'inputnum': Expected no arguments or a single argument.");
      if (!args.empty()) {
         args.at(0)->print();
      }
      long double user_input = 0;
      std::cin >> user_input;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return NumberValue::make(user_input, line);
   }

   Value inputch(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() > 1, "'inputch': Expected no arguments or a single argument.");
      if (!args.empty()) {
         args.at(0)->print();
      }
      char user_input = 0;
      std::cin >> std::noskipws >> user_input;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return CharValue::make(user_input, line);
   }

   // Type conversion functions

   Value string(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() > 1, "'string': Expected no arguments or a single argument.");
      return StringValue::make((args.empty() ? "" : args.at(0)->as_string()), line);
   }

   Value number(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() > 1, "'number': Expected no arguments or a single argument.");
      return NumberValue::make((args.empty() ? 0 : args.at(0)->as_number()), line);
   }

   Value char_(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() > 1, "'char': Expected no arguments or a single argument.");
      return CharValue::make((args.empty() ? 0 : args.at(0)->as_char()), line);
   }

   Value bool_(std::vector<Value>& args, Environment* env, int line) {
      fmt::raise_if(line, args.size() > 1, "'bool': Expected no arguments or a single argument.");
      return BoolValue::make((args.empty() ? false : args.at(0)->as_bool()), line);
   }
}
