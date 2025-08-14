#ifndef FILE_HPP
#define FILE_HPP

#include <string>

namespace file {
   bool exists(const std::string& file);
   std::string read(const std::string& file);   
}

#endif
