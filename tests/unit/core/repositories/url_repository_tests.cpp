#include "../../../../include/prettyurl/core/repositories/url_repository.hpp"
#include "../../../mocks/in_memory_url_repository_impl.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace prettyurl;

SCENARIO("url repository tests") {
  auto repo = std::make_shared<in_memory_url_repository_impl>();

  repo->add_short_url("asnb1a7", "google.com");
  repo->add_short_url("jk1099C", "amazon.com");
  repo->add_short_url("Lknx1aZ", "yahoo.com");

  REQUIRE(repo->get_long_url("asnb1a7").has_value());
  REQUIRE(repo->get_long_url("jk1099C").has_value());
  REQUIRE(repo->get_long_url("Lknx1aZ").has_value());

  REQUIRE(repo->get_long_url("asnb1a7").value() == "google.com");
  REQUIRE(repo->get_long_url("jk1099C").value() == "amazon.com");
  REQUIRE(repo->get_long_url("Lknx1aZ").value() == "yahoo.com");

  WHEN("trying to add existing short url") {
    THEN("throw exception") {
      REQUIRE_THROWS(repo->add_short_url("asnb1a7", "uber.com"));
    }
  }
}