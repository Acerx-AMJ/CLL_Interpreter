#include "functions.hpp"
#include "fmt.hpp"
#include <iostream>

namespace fun {
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
      auto null = printf(args);
      std::cout << '\n';
      return std::move(null);
   }

   Value format(const std::vector<Value>& args) {
      fmt::raise_if(args.empty() || args.at(0)->type != ValueType::string, "'printf': Expected at least one argument and expected the first argument to be a string.");
      std::string base = static_cast<StringValue&>(*args.at(0).get()).string;
      std::vector<std::string> arguments;

      for (int i = 1; i < args.size(); ++i) {
         arguments.push_back(args.at(i)->as_string());
      }
      return std::make_unique<StringValue>(fmt::format_v(base.c_str(), arguments));
   }
}
