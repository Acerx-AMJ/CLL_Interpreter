#include "functions.hpp"
#include "fmt.hpp"
#include <iostream>
#include <limits>

namespace fun {
   // Print/Format functions

   Value print(const std::vector<Value>& args) {
      for (int i = 0; i < args.size(); ++i) {
         args.at(i)->print();
         if (i + 1 < args.size())
            std::cout << ' ';
      }
      return std::make_unique<NullValue>();
   }

   Value println(const std::vector<Value>& args) {
      auto null = print(args);
      std::cout << '\n';
      return std::move(null);
   }

   Value printf(const std::vector<Value>& args) {
      fmt::raise_if(args.empty() || args.at(0)->type != ValueType::string, "'printf': Expected at least one argument and expected the first argument to be a string.");
      std::string base = static_cast<StringValue&>(*args.at(0).get()).string;
      std::vector<std::string> arguments;

      for (int i = 1; i < args.size(); ++i) {
         arguments.push_back(args.at(i)->as_string());
      }
      fmt::printf_v(base.c_str(), arguments);
      return std::make_unique<NullValue>();
   }

   Value printfln(const std::vector<Value>& args) {
      fmt::raise_if(args.empty() || args.at(0)->type != ValueType::string, "'printfln': Expected at least one argument and expected the first argument to be a string.");
      std::string base = static_cast<StringValue&>(*args.at(0).get()).string;
      std::vector<std::string> arguments;

      for (int i = 1; i < args.size(); ++i) {
         arguments.push_back(args.at(i)->as_string());
      }
      fmt::printfln_v(base.c_str(), arguments);
      return std::make_unique<NullValue>();
   }

   Value format(const std::vector<Value>& args) {
      fmt::raise_if(args.empty() || args.at(0)->type != ValueType::string, "'format': Expected at least one argument and expected the first argument to be a string.");
      std::string base = static_cast<StringValue&>(*args.at(0).get()).string;
      std::vector<std::string> arguments;

      for (int i = 1; i < args.size(); ++i) {
         arguments.push_back(args.at(i)->as_string());
      }
      return std::make_unique<StringValue>(fmt::format_v(base.c_str(), arguments));
   }

   // Input commands

   Value input(const std::vector<Value>& args) {
      fmt::raise_if(args.size() > 1, "'input': Expected no arguments or a single argument.");
      if (!args.empty()) {
         args.at(0)->print();
      }
      std::string user_input;
      std::getline(std::cin, user_input);
      return std::make_unique<StringValue>(user_input);
   }

   Value inputnum(const std::vector<Value>& args) {
      fmt::raise_if(args.size() > 1, "'inputnum': Expected no arguments or a single argument.");
      if (!args.empty()) {
         args.at(0)->print();
      }
      long double user_input = 0;
      std::cin >> user_input;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return std::make_unique<NumberValue>(user_input);
   }

   Value inputch(const std::vector<Value>& args) {
      fmt::raise_if(args.size() > 1, "'inputch': Expected no arguments or a single argument.");
      if (!args.empty()) {
         args.at(0)->print();
      }
      char user_input = 0;
      std::cin >> std::noskipws >> user_input;
      return std::make_unique<CharValue>(user_input);
   }

   // Convert functions

   Value string(const std::vector<Value>& args) {
      fmt::raise_if(args.size() > 1, "'string': Expected no arguments or a single argument.");
      return std::make_unique<StringValue>((args.empty() ? "" : args.at(0)->as_string()));
   }

   Value number(const std::vector<Value>& args) {
      fmt::raise_if(args.size() > 1, "'number': Expected no arguments or a single argument.");
      return std::make_unique<NumberValue>((args.empty() ? 0 : args.at(0)->as_number()));
   }

   Value char_(const std::vector<Value>& args) {
      fmt::raise_if(args.size() > 1, "'char': Expected no arguments or a single argument.");
      return std::make_unique<CharValue>((args.empty() ? 0 : args.at(0)->as_char()));
   }
}
