#include "prettyurl/app.hpp"
#include "prettyurl/app/cli/cli_parser.hpp"
#include "prettyurl/infra/config/json_config_parser.hpp"

#include <stdexcept>
#include <iostream>

int main(int argc, char* argv[]) {
  try {
    const auto params = prettyurl::app::cli::cli_parser::instance().parse(argc, argv);

    if (!params.has_value()) {
      throw std::runtime_error("invalid cli params");
    }
    
    prettyurl::core::config::app_config app_cfg;  
    prettyurl::infra::config::json_config_parser::instance().parse(params.value().config_path, app_cfg);
    
    prettyurl::application::run(app_cfg);
  } catch (const std::exception& e) {
    std::cerr << "an error occured while running app (" << e.what() << ")\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "an unknown error occured while running app\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}