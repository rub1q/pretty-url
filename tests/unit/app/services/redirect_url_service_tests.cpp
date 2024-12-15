#include "../../../../include/prettyurl/app/services/redirect_url_service.hpp"
#include "../../../mocks/in_memory_url_repository_impl.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace prettyurl;

SCENARIO("redirect url service tests") {
  auto repo = std::make_shared<in_memory_url_repository_impl>();

  repo->add_short_url("asnb1a7", "google.com");

  app::services::redirect_url_service service { std::move(repo) };

  REQUIRE(service.get_source_url("asnb1a7").has_value());
  REQUIRE(service.get_source_url("asnb1a7").value() == "google.com");
  
  REQUIRE_FALSE(service.get_source_url("abcdefg").has_value());
}