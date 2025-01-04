#pragma once

#include <filesystem>

namespace prettyurl::core::config {

struct logging_config final {
  std::filesystem::path path;
  std::string level { "debug" };
};

} // namespace prettyurl::core::config