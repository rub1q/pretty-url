#include "../../../../include/prettyurl/core/encoding/base62_encoder.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace prettyurl;

SCENARIO("base62 encoder tests") {
  auto encoder = core::encoding::base62_encoder();
  
  REQUIRE(encoder.encode(0u) == "0");
  REQUIRE(encoder.encode(1u) == "1");
  REQUIRE(encoder.encode(62u) == "10");
  REQUIRE(encoder.encode(12345u) == "3D7");
  REQUIRE(encoder.encode(987654321u) == "14q60P");

  REQUIRE(encoder.decode("0") == 0u);
  REQUIRE(encoder.decode("1") == 1u);
  REQUIRE(encoder.decode("10") == 62u);
  REQUIRE(encoder.decode("3D7") == 12345u);
  REQUIRE(encoder.decode("14q60P") == 987654321u);
}