#include "../../../../include/prettyurl/core/db/db_session_manager.hpp"
#include "../../../../include/prettyurl/core/db/base_db_session.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <memory>
#include <chrono>
#include <thread>

using namespace prettyurl;
using namespace std::literals;

class my_db_session : public core::db::base_db_session {
public:
  my_db_session() = default;

  constexpr explicit my_db_session(std::string_view conn_str)
    : conn_string_(conn_str) {
  }

  [[nodiscard]] bool is_connected() const override { return true; }

  void reconnect() override {
    disconnect();
    connect();
  }

  void begin_transaction() override {};
  void commit_transaction() override {};
  void rollback_transaction() override {};
  std::optional<core::db::query_result_set> execute_query(std::string_view query) override {};

private:
  void connect() override {}
  void disconnect() override {}

private:
  std::string conn_string_;
};

SCENARIO_METHOD((my_db_session), "db session manager tests") {
  WHEN("pool size equals zero") {
    THEN("throw exception") {
      REQUIRE_THROWS_WITH(core::db::db_session_manager<my_db_session>(0, "db=test;user=admin;password=admin"), "sessions pool size must be > 0");
    }
  }

  constexpr auto POOL_SIZE = 5u;

  core::db::db_session_manager<my_db_session> man(POOL_SIZE, "db=test;user=admin;password=admin");
  REQUIRE(man.free_sessions_count() == POOL_SIZE);

  {
    auto session = man.get_session(1s);
    REQUIRE(man.free_sessions_count() == POOL_SIZE - 1);
  }

  REQUIRE(man.free_sessions_count() == POOL_SIZE);
}