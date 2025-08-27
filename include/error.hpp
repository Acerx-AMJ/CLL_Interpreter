#ifndef ERROR_HPP
#define ERROR_HPP

// Includes

#include <string>

// Errors

namespace err {
   constexpr int nline = -1;

   void set_program_code(std::string& code);
   [[noreturn]] void raise(const std::string& msg, int line, int code = -1);
   [[noreturn]] void exit(int code = 0);
}

#endif
