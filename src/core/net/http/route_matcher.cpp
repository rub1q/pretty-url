#include "prettyurl/core/net/http/route_matcher.hpp"

namespace prettyurl::core::net::http {

route_matcher::route_matcher(std::string_view pattern) {
  regex_str_ = "^";

  std::string::size_type open_brace_pos = 0u;
  std::string::size_type last_pos = open_brace_pos;

  if (pattern.find('{') != std::string::npos) {
    vars_.reserve(10);
  }

  while ((open_brace_pos = pattern.find('{', last_pos)) != std::string::npos) {
    const auto close_brace_pos = pattern.find('}', open_brace_pos + 1);
    
    if (close_brace_pos == std::string::npos) {
      throw std::runtime_error("unmatched '{' in route path");
    }
 
    regex_str_ += pattern.substr(last_pos, open_brace_pos - last_pos);
    regex_str_ += "([^/]+)";

    const auto var_name = pattern.substr(open_brace_pos + 1, close_brace_pos - open_brace_pos - 1);
    vars_.emplace_back(std::move(var_name));

    last_pos = close_brace_pos + 1;
  }

  vars_.shrink_to_fit();

  if (last_pos != std::string::npos) {
    regex_str_ += pattern.substr(last_pos);
  }

  regex_str_ += "$";

  regex_ = regex_str_;
}

bool route_matcher::match(std::string_view path, vars_map& vars) const {
  std::smatch route_match;
  std::string s { path };

  if (std::regex_match(s, route_match, regex_)) {
    vars.clear();
    
    for (std::size_t i = 0u; i < vars_.size(); i++) {
      if (i + 1 < route_match.size()) {
        vars[vars_[i]] = route_match[i + 1].str();
      }
    }

    return true;
  }

  return false;
}

} // namespace prettyurl::infra::net::http