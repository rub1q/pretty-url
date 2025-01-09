#pragma once

#include <string>
#include <regex>

namespace prettyurl::core::net::http {

class route_matcher {
public:
  explicit route_matcher(std::string_view pattern);

  [[nodiscard]] bool match(std::string_view route);
  [[nodiscard]] std::string regex_str() const noexcept;

private:
  route_matcher() = default;

private:
  std::string regex_str_;

  std::regex regex_;
};

} // namespace prettyurl::core::net::http