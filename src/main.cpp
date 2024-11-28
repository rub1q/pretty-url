#include "prettyurl/app.hpp"

#include <stdexcept>
#include <iostream>

int main(int argc, char* argv[]) {
  try {
    prettyurl::application::run();
  } catch (const std::exception& e) {
    std::cerr << "an error occured while running app (" << e.what() << ")\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "an unknown error occured while running app\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}