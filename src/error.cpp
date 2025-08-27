#include "error.hpp"

// Includes

#include <iostream>
#include <sstream>

// Static variables

static std::string temp = "";
static std::string& program_code = temp;

// Errors

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
      int previous_line = 0, next_line = 0;
      int i = 1;
      
      while (std::getline(ss, temp)) {
         if (i < line && !temp.empty()) {
            previous = temp;
            previous_line = i;
         } else if (i == line) {
            current = temp;
         } else if (i > line && next.empty()) {
            next = temp;
            next_line = i;
         }
         ++i;
      }

      if (!previous.empty()) {
         printf("  %-5d %s\n", previous_line, previous.c_str());
         for (int i = previous_line; i < line - 1; ++i) {
            printf("  %-5d\n", i + 1);
         }
      }
      if (!current.empty()) {
         printf("  %-5d %s\n", line, current.c_str());
         printf("  %-5s \e[91m%s\e[0m\n", std::string(std::to_string(line).size(), ' ').c_str(), std::string(current.size(), '^').c_str());
      }
      if (!next.empty()) {
         for (int i = line; i < next_line - 1; ++i) {
            printf("  %-5d\n", i + 1);
         }
         printf("  %-5d %s\n", next_line, next.c_str());
      }

      err::exit(code);
   }

   void exit(int code) {
      std::cout << (code == 0 ? "\n" : "\n\e[91m") << "Program exited with exit code " << code << ".\e[0m\n";
      std::exit(code);
   }
}
