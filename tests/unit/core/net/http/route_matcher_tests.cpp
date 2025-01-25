#include "../../../../../include/prettyurl/core/net/http/route_matcher.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

using namespace prettyurl;

SCENARIO("simple route matcher tests") {
  core::net::http::route_matcher rm { "/api/v1/{short_url}" };

  core::net::http::route_matcher::vars_map m;

  REQUIRE(rm.match("/api/v1/Q1D", m));

  REQUIRE(m.contains("short_url"));
  REQUIRE(m.at("short_url") == "Q1D");
}

SCENARIO("several params in route tests") {
  core::net::http::route_matcher rm { "/api/v1/{par1}/bla/{par2}"};

  core::net::http::route_matcher::vars_map m;

  REQUIRE(rm.match("/api/v1/QUi34/bla/89as", m));

  REQUIRE(m.contains("par1"));
  REQUIRE(m.contains("par2"));

  REQUIRE(m.at("par1") == "QUi34");
  REQUIRE(m.at("par2") == "89as"); 
}

SCENARIO("param braces mismatch tests") {
  REQUIRE_THROWS_WITH(core::net::http::route_matcher{ "/api/v1/{short_url" }, "unmatched '{' in route path");
}