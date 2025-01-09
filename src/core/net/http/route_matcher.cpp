#include "prettyurl/core/net/http/route_matcher.hpp"

namespace prettyurl::core::net::http {

route_matcher::route_matcher(std::string_view pattern) {
  regex_str_ = "^";

  std::string::size_type open_brace_pos = 0u;
  std::string::size_type last_pos = open_brace_pos;
  
  while ((open_brace_pos = pattern.find('{', last_pos)) != std::string::npos) {
    const auto close_brace_pos = pattern.find('}', open_brace_pos + 1);
    
    if (close_brace_pos == std::string::npos) {
      throw std::runtime_error("unmatched '{' in route path");
    }

    regex_str_ += pattern.substr(last_pos, open_brace_pos - last_pos);
    regex_str_ += "([^/]+)";

    last_pos = close_brace_pos + 1;
  }

  if (last_pos != std::string::npos) {
    regex_str_ += pattern.substr(last_pos);
  }

  regex_str_ += "$";

  regex_ = regex_str_;
}

std::string route_matcher::regex_str() const noexcept {
  return regex_str_;
}

bool route_matcher::match(std::string_view route) {
  std::smatch route_match;
  std::string s { route };

  if (std::regex_match(s, route_match, regex_)) {
    return true;
  }

  return false;
}

} // namespace prettyurl::infra::net::http