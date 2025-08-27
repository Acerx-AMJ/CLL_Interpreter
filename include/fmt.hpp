#ifndef FMT_HPP
#define FMT_HPP

// Includes

#include "error.hpp"
#include <iostream>
#include <sstream>
#include <vector>

// Format

namespace fmt {
   using namespace std::string_literals;
   
   // To string functions

   template<typename T>
   std::string to_string(const T& value) {
      std::stringstream s;
      s << value;
      return s.str();
   }

   template<>
   inline std::string to_string(const bool& value) {
      return (value ? "true"s : "false"s);
   }

   // Format functions

   template<typename... Args>
   std::string format(const char* base, const Args&... args) {
      std::string result = base;

      size_t pos = 0;
      (( pos = result.find("{}", pos),
         pos != std::string::npos
            ? result = result.replace(pos, 2, fmt::to_string(args))
            : result
      ), ...);
      return result;
   }

   template<typename T>
   std::string format_v(const char* base, const std::vector<T>& args = {}) {
      std::string result = base;
      size_t pos = 0;

      for (const auto& arg : args) {
         pos = result.find("{}", pos);
         result = (pos != std::string::npos ? result.replace(pos, 2, fmt::to_string(arg)) : result);
      }
      return result;
   }

   // Print functions

   template<typename... Args>
   void printf(const char* base, const Args&... args) {
      std::cout << fmt::format(base, args...);
   }

   template<typename... Args>
   void printfln(const char* base, const Args&... args) {
      std::cout << fmt::format(base, args...) << '\n';
   }

   template<typename... Args>
   void print(const Args&... args) {
      ((std::cout << std::boolalpha << args << ' '), ...);
   }

   template<typename... Args>
   void println(const Args&... args) {
      ((std::cout << std::boolalpha << args << ' '), ...);
      std::cout << '\n';
   }

   // Format print functions

   template<typename T>
   void printf_v(const char* base, const std::vector<T>& args = {}) {
      std::cout << fmt::format_v(base, args);
   }

   template<typename T>
   void printfln_v(const char* base, const std::vector<T>& args = {}) {
      std::cout << fmt::format_v(base, args) << '\n';
   }

   template<typename T>
   void print_v(const std::vector<T>& args = {}) {
      for (const auto& arg : args) {
         std::cout << std::boolalpha << arg << ' ';
      }
   }

   template<typename T>
   void println_v(const std::vector<T>& args = {}) {
      for (const auto& arg : args) {
         std::cout << std::boolalpha << arg << ' ';
      }
      std::cout << '\n';
   }

   // Raise functions

   template<typename... Args>
   [[noreturn]] void raise(int line, const char* error, const Args&... args) {
      err::raise(fmt::format(error, args...), line);
   }

   template<typename... Args>
   void raise_if(int line, bool cond, const char* error, const Args&... args) {
      if (cond) {
         fmt::raise(line, error, args...);
      }
   }

   template<typename T>
   [[noreturn]] void raise_v(int line, const char* error, const std::vector<T>& args = {}) {
      err::raise(fmt::format_v(error, args), line);
   }

   template<typename T>
   void raise_if_v(int line, bool cond, const char* error, const std::vector<T>& args = {}) {
      if (cond) {
         fmt::raise_v(line, error, args);
      }
   }
}

#endif
