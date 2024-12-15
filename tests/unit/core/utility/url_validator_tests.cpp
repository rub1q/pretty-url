#include "../../../../include/prettyurl/core/utility/url_validator.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace prettyurl;

SCENARIO("url validator tests") {
  REQUIRE(core::utility::url_validator::is_valid("https://google.com"));
  REQUIRE(core::utility::url_validator::is_valid("http://amazon.com"));
  REQUIRE(core::utility::url_validator::is_valid("uber.com"));
  REQUIRE(core::utility::url_validator::is_valid("https://www.youtube.com/feed/subscriptions"));

  REQUIRE_FALSE(core::utility::url_validator::is_valid("1234"));
  REQUIRE_FALSE(core::utility::url_validator::is_valid("abcdef"));
  REQUIRE_FALSE(core::utility::url_validator::is_valid("!google.com"));
  REQUIRE_FALSE(core::utility::url_validator::is_valid("https://<>!**.com"));
}