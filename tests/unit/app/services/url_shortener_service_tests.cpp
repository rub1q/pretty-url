#include "../../../../include/prettyurl/app/services/url_shortener_service.hpp"
#include "../../../../include/prettyurl/core/encoding/base62_encoder.hpp"
#include "../../../../include/prettyurl/infra/db/auto_inc_id_generator.hpp"

#include "../../../mocks/in_memory_url_repository_impl.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace prettyurl;

SCENARIO("url shortener service tests") {
  auto repo = std::make_shared<in_memory_url_repository_impl>();

  repo->add_short_url("asnb1a7", "google.com");

  auto encoder = std::make_unique<core::encoding::base62_encoder>();
  auto id_generator = std::make_unique<infra::db::auto_inc_id_generator>(repo);

  app::services::url_shortener_service service { std::move(repo), std::move(encoder), std::move(id_generator) };

  REQUIRE(service.shorten("google.com") == "asnb1a7");
  REQUIRE(service.shorten("amazon.com") == "2");

  REQUIRE(service.is_valid_url("google.com"));
  REQUIRE(service.is_valid_url("https://www.postgresql.org/docs/current/sql-createindex.html"));
  REQUIRE(service.is_valid_url("https://stackoverflow.com"));

  REQUIRE_FALSE(service.is_valid_url("j1ik2j1113"));
}