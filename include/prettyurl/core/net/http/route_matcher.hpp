#pragma once

#include <string>
#include <regex>
#include <unordered_map>
#include <vector>

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

  std::vector<std::string> vars_;
};

} // namespace prettyurl::core::net::http