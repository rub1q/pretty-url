#include "../../../../../include/prettyurl/infra/net/http/router.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace prettyurl;

SCENARIO("build new router") {
  infra::net::http::router router;

  router.add_route("/route1", core::net::http::emethod::get, [](auto&& req) {
    infra::net::http::response resp { core::net::http::estatus::ok };
    return resp;
  });

  const auto route = router.get("/route1");

  REQUIRE(route.has_value());
  REQUIRE(route.value().get().is_allowed_method(core::net::http::emethod::get));

  REQUIRE_FALSE(router.get("/abc").has_value());

  infra::net::http::request req;

  req.method(core::net::http::emethod::get);
  req.target("/route1");

  WHEN("route found") {
    THEN("return ok") {
      REQUIRE(router(std::move(req)).status_code() == core::net::http::estatus::ok);
    }
  }

  req.target("/abc");

  WHEN("route not found") {
    THEN("return not found") {
      REQUIRE(router(std::move(req)).status_code() == core::net::http::estatus::not_found);
    }
  }

  req.target("/route1");
  req.method(core::net::http::emethod::post);

  WHEN("method not allowed") {
    THEN("return method not allowed") {
      REQUIRE(router(std::move(req)).status_code() == core::net::http::estatus::method_not_allowed);
    }
  }
}