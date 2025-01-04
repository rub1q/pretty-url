#pragma once

#include <filesystem>
#include <optional>

namespace prettyurl::app::cli {

struct cli_params final {
  std::filesystem::path config_path;
};

class cli_parser final {
public:
  cli_parser(const cli_parser&) = delete;
  cli_parser(cli_parser&&) = delete;

  cli_parser& operator=(const cli_parser&) = delete;
  cli_parser& operator=(cli_parser&&) = delete;

  [[nodiscard]] static const cli_parser& instance() noexcept;
  [[nodiscard]] std::optional<cli_params> parse(const int argc, char* argv[]) const;

private:
  cli_parser() = default;
};

} // namespace prettyurl::app::cli