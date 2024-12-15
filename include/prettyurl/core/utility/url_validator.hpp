#pragma once

#include <string>

namespace prettyurl::core::utility {

class url_validator {
public:
  [[nodiscard]] static bool is_valid(std::string_view url);
};

} // namespace prettyurl::core::utility