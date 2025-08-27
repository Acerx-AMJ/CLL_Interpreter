#ifndef FILE_HPP
#define FILE_HPP

// Includes

#include <string>

// File

namespace file {
   bool exists(const std::string& file);
   std::string read(const std::string& file);   
}

#endif
