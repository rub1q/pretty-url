#include "prettyurl/core/utility/url_validator.hpp"

#include <regex>

namespace prettyurl::core::utility {

bool url_validator::is_valid(std::string_view url) {
  static const std::regex url_regex { R"(^(https?:\/\/)?([a-zA-Z0-9.-]+)(\.[a-zA-Z]{2,})(:[0-9]{1,5})?(\/.*)?$)" };
  return std::regex_match(url.data(), url_regex);
}

} // namespace prettyurl::core::utility