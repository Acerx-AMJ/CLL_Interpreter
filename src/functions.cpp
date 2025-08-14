#include "functions.hpp"
#include <iostream>

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
