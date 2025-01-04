#include "prettyurl/app/cli/cli_parser.hpp"

#include <iostream>
#include <boost/program_options.hpp>

namespace prettyurl::app::cli {

namespace po = boost::program_options;

const cli_parser& cli_parser::instance() noexcept {
  static cli_parser p {};
  return p;
}

std::optional<cli_params> cli_parser::parse(const int argc, char* argv[]) const {
  po::options_description desc("Allowed options:\n");

  desc.add_options()
    ("help,h", "show help")
    ("config, c", po::value<std::string>(), "config file");

  po::variables_map vm;

  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.contains("help")) {
      std::cout << desc << '\n';
      return std::nullopt;
    }

    if (!vm.contains("config")) {
      throw std::runtime_error("config file path is absent");
    }

    cli_params params;
    params.config_path = vm["config"].as<std::string>();

    return params;
  } catch (const po::error& e) {
    throw std::runtime_error("failed parsing cli (" + std::string(e.what()) + ")");
  }
}

} // namespace prettyurl::app::cli