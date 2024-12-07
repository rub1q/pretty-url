#pragma once

#include <optional>
#include <string>

namespace prettyurl::core::repositories {

struct url_repository {
  virtual ~url_repository() = default;

  [[nodiscard]] virtual std::optional<std::string> get_long_url(std::string_view short_url) = 0;
  
  virtual void add_short_url(std::string_view short_url, std::string_view long_url) = 0;
};

} // namespace prettyurl::core::repositories