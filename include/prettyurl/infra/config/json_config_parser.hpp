#pragma once

#include "prettyurl/core/config/app_config.hpp"

#include <filesystem>

namespace prettyurl::infra::config {

class json_config_parser final {
public:
  json_config_parser(const json_config_parser&) = delete;
  json_config_parser(json_config_parser&&) = delete;

  json_config_parser& operator=(const json_config_parser&) = delete;
  json_config_parser& operator=(json_config_parser&&) = delete;

  [[nodiscard]] static const json_config_parser& instance() noexcept;

  void parse(const std::filesystem::path& path, core::config::app_config& cfg) const;

private:
  json_config_parser() = default;
};

} // namespace prettyurl::infra::config