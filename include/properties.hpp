#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

// Includes

#include "values.hpp"

// Properties

namespace prop {
   // Property functions

   void init();
   bool exists(const std::string& name, ValueType type);
   bool overrides(const std::string& name, ValueType type);
   Value get(const std::string& name, ValueType type);

   // Array functions

   Value array_push         (std::vector<Value>& args, Environment* env, int line);
   Value array_insert       (std::vector<Value>& args, Environment* env, int line);
   Value array_erase        (std::vector<Value>& args, Environment* env, int line);
   Value array_pop          (std::vector<Value>& args, Environment* env, int line);
   Value array_size         (std::vector<Value>& args, Environment* env, int line);
   Value array_empty        (std::vector<Value>& args, Environment* env, int line);
   Value array_get          (std::vector<Value>& args, Environment* env, int line);
   Value array_set          (std::vector<Value>& args, Environment* env, int line);
   Value array_find         (std::vector<Value>& args, Environment* env, int line);
   Value array_find_all     (std::vector<Value>& args, Environment* env, int line);
   Value array_contains     (std::vector<Value>& args, Environment* env, int line);
   Value array_count        (std::vector<Value>& args, Environment* env, int line);
   Value array_in           (std::vector<Value>& args, Environment* env, int line);
   Value array_first        (std::vector<Value>& args, Environment* env, int line);
   Value array_last         (std::vector<Value>& args, Environment* env, int line);
   Value array_clear        (std::vector<Value>& args, Environment* env, int line);
   Value array_fill         (std::vector<Value>& args, Environment* env, int line);
   Value array_join         (std::vector<Value>& args, Environment* env, int line);
   Value array_reverse      (std::vector<Value>& args, Environment* env, int line);
   Value array_slice        (std::vector<Value>& args, Environment* env, int line);

   // String functions

   Value string_push        (std::vector<Value>& args, Environment* env, int line);
   Value string_insert      (std::vector<Value>& args, Environment* env, int line);
   Value string_erase       (std::vector<Value>& args, Environment* env, int line);
   Value string_pop         (std::vector<Value>& args, Environment* env, int line);
   Value string_size        (std::vector<Value>& args, Environment* env, int line);
   Value string_empty       (std::vector<Value>& args, Environment* env, int line);
   Value string_get         (std::vector<Value>& args, Environment* env, int line);
   Value string_set         (std::vector<Value>& args, Environment* env, int line);
   Value string_find        (std::vector<Value>& args, Environment* env, int line);
   Value string_find_all    (std::vector<Value>& args, Environment* env, int line);
   Value string_contains    (std::vector<Value>& args, Environment* env, int line);
   Value string_count       (std::vector<Value>& args, Environment* env, int line);
   Value string_in          (std::vector<Value>& args, Environment* env, int line);
   Value string_first       (std::vector<Value>& args, Environment* env, int line);
   Value string_last        (std::vector<Value>& args, Environment* env, int line);
   Value string_clear       (std::vector<Value>& args, Environment* env, int line);
   Value string_fill        (std::vector<Value>& args, Environment* env, int line);
   Value string_reverse     (std::vector<Value>& args, Environment* env, int line);
   Value string_slice       (std::vector<Value>& args, Environment* env, int line);
   Value string_upper       (std::vector<Value>& args, Environment* env, int line);
   Value string_lower       (std::vector<Value>& args, Environment* env, int line);
   Value string_swapcase    (std::vector<Value>& args, Environment* env, int line);
   Value string_trim        (std::vector<Value>& args, Environment* env, int line);
   Value string_pad_start   (std::vector<Value>& args, Environment* env, int line);
   Value string_pad_end     (std::vector<Value>& args, Environment* env, int line);
   Value string_repeat      (std::vector<Value>& args, Environment* env, int line);
   Value string_replace     (std::vector<Value>& args, Environment* env, int line);
   Value string_replace_all (std::vector<Value>& args, Environment* env, int line);
   Value string_split       (std::vector<Value>& args, Environment* env, int line);
   Value string_starts_with (std::vector<Value>& args, Environment* env, int line);
   Value string_ends_with   (std::vector<Value>& args, Environment* env, int line);

   // Helper functions

   void save(std::vector<Value>& args, Value value, Environment* env, int line);
}

#endif
