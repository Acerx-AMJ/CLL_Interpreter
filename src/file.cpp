#include "file.hpp"

// Includes

#include "fmt.hpp"
#include <filesystem>
#include <fstream>

// File

namespace file {
   bool exists(const std::string& file) {
      try { return std::filesystem::is_regular_file(file); }
      catch (...) { return false; }
   }

   std::string read(const std::string& file) {
      std::ifstream fbuf (file);
      fmt::raise_if(err::nline, !fbuf, "Could not read file '{}'.", file);

      std::string output, temp;
      while (std::getline(fbuf, temp)) {
         output += temp + '\n';
      }
      return output;
   }
}
