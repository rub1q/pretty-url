#include "../../../../include/prettyurl/core/db/query_result_set.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

using namespace prettyurl;

SCENARIO("query result set tests") {
  core::db::query_result_set result_set;

  result_set.set_columns({"id", "short_url", "long_url", "created_at", "expired_at"});

  result_set.add_row({"1", "h1JKn09", "amazon.com", std::nullopt, "2025-11-30 19:49:05.715 +0300"});
  result_set.add_row({"2", "opwC681", "google.com", "2024-11-30 19:49:05.715 +0300", "2025-11-30 19:49:05.715 +0300"});
  result_set.add_row({"3", "jnMm13x", "yahoo.com", "2024-11-30 19:49:05.715 +0300", "2025-11-30 19:49:05.715 +0300"});

  WHEN("column size less than column num") {
    THEN("throw exception") {
      REQUIRE_THROWS(result_set.add_row({"1", "2", "3", "4"}));
    }
  }

  REQUIRE_THROWS_WITH(result_set[999], "row index is out of range");

  auto row = result_set[0];

  REQUIRE(row.get<int>("id") == 1);
  REQUIRE(row["short_url"] == "h1JKn09");
  REQUIRE(row.get<std::string>("long_url") == "amazon.com");
  REQUIRE_FALSE(row.get<std::string>("created_at").has_value());
  REQUIRE(row.get<std::string>("expired_at") == "2025-11-30 19:49:05.715 +0300");

  REQUIRE_THROWS_WITH(row["empty"], "column (empty) does not exist");
  REQUIRE_THROWS_WITH(row[999], "column index is out of range");
}