#include "error.hpp"

#include <iostream>
#include <sstream>

static std::string temp = "";
static std::string& program_code = temp;

namespace err {
   void set_program_code(std::string& code) {
      program_code = code;
   }

   void raise(const std::string& msg, int line, int code) {
      std::cout << "Program exited due to the following error:\n";
      std::cout << " \e[91m" << msg << "\e[0m\n";

      if (line == err::nline) {
         err::exit(code);
      }

      std::stringstream ss (program_code);
      std::string previous, current, next, temp;
      for (int i = 1; i <= line + 1; ++i) {
         if (!std::getline(ss, temp)) {
            break;
         }

         if (i == line - 1) {
            previous = temp;
         } else if (i == line) {
            current = temp;
         } else if (i == line + 1) {
            next = temp;
         }
      }

      if (!previous.empty()) {
         printf("  %-5d '%s'\n", line - 1, previous.c_str());
      }
      if (!current.empty()) {
         printf("  %-5d '%s'\n", line, current.c_str());
         printf("  %-5s  \e[91m%s\e[0m\n", std::string(std::to_string(line).size(), ' ').c_str(), std::string(current.size(), '^').c_str());
      }
      if (!next.empty()) {
         printf("  %-5d '%s'\n", line + 1, next.c_str());
      }

      err::exit(code);
   }

   void exit(int code) {
      std::cout << "\n\e[91mExited with exit code " << code << ".\e[0m\n";
      std::exit(code);
   }
}
