#include "../../../../../include/prettyurl/infra/net/http/route.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace prettyurl;

SCENARIO("route allowed methods test") {
  infra::net::http::route r(core::net::http::emethod::get, [](auto&& req) {
    infra::net::http::response resp { core::net::http::estatus::ok };
    return resp;
  });

  REQUIRE(r.is_allowed_method(core::net::http::emethod::head));
  REQUIRE(r.is_allowed_method(core::net::http::emethod::get));
  REQUIRE_FALSE(r.is_allowed_method(core::net::http::emethod::post));
  REQUIRE_FALSE(r.is_allowed_method(core::net::http::emethod::delete_));
  REQUIRE_FALSE(r.is_allowed_method(core::net::http::emethod::put));
  REQUIRE_FALSE(r.is_allowed_method(core::net::http::emethod::patch));
}

SCENARIO("route null handler test") {
  infra::net::http::route r;
  r.methods(core::net::http::emethod::get);

  infra::net::http::request req;

  REQUIRE_THROWS(r.handle(std::move(req)));
}

SCENARIO("build new route") {
  infra::net::http::route r;

  r.methods(core::net::http::emethod::post | core::net::http::emethod::put);
  r.handler([](auto&& req) { 
    infra::net::http::response resp { core::net::http::estatus::ok };
    return resp;
  });

  REQUIRE(r.is_allowed_method(core::net::http::emethod::head));
  REQUIRE(r.is_allowed_method(core::net::http::emethod::post));
  REQUIRE(r.is_allowed_method(core::net::http::emethod::put));
  
  infra::net::http::request req;
  req.method(core::net::http::emethod::post);

  REQUIRE(r.handle(std::move(req)).status_code() == core::net::http::estatus::ok);
}