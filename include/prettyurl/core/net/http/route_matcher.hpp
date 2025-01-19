#pragma once

#include <string>
#include <regex>
#include <unordered_map>

namespace prettyurl::core::net::http {

class route_matcher final {
public:
  using vars_map = std::unordered_map<std::string, std::string>;

  explicit route_matcher(std::string_view pattern);

  [[nodiscard]] bool match(std::string_view path, vars_map& vars) const;

private:
  route_matcher() = default;

private:
  std::string regex_str_;
  std::regex regex_;

  std::unordered_map<std::string, std::size_t> vars_idx_;
};

} // namespace prettyurl::core::net::http