#ifndef FMT_HPP
#define FMT_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fmt {
   using namespace std::string_literals;
   
   // Convert any type to string
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

   // Format string
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
   void println_v(std::vector<T>& args = {}) {
      for (const auto& arg : args) {
         std::cout << std::boolalpha << arg << ' ';
      }
      std::cout << '\n';
   }

   // Exception
   class exception : public std::exception {
      std::string msg;

   public:
      explicit exception(const std::string& message) {
         msg = "Program exited due to the following error:\n\t"s + message;
      }
      
      virtual const char* what() const noexcept override {
         return msg.c_str();
      }
   };

   // Raise functions
   template<typename... Args>
   [[noreturn]] void raise(const char* error, const Args&... args) {
      throw fmt::exception(fmt::format(error, args...));
   }

   template<typename... Args>
   void raise_if(bool cond, const char* error, const Args&... args) {
      if (cond) {
         fmt::raise(error, args...);
      }
   }
}

#endif
