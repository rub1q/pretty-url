#include "prettyurl/infra/config/json_config_parser.hpp"

#include "rapidjson/document.h"

#include <stdexcept>
#include <fstream>

namespace prettyurl::infra::config {

const json_config_parser& json_config_parser::instance() noexcept {
  static json_config_parser parser {};
  return parser;
}

void json_config_parser::parse(const std::filesystem::path& path, core::config::app_config& cfg) const {
  if (path.empty()) {
    throw std::invalid_argument("invalid config path");
  }

  std::ifstream is(path, std::ios::in);

  if (!is) {
    throw std::runtime_error("unable to open the config file");
  }

  std::string input { std::istreambuf_iterator<char>{is}, {} };
  is.close();

  namespace json = rapidjson;

  json::Document root;
  json::ParseResult ok = root.Parse(input.c_str());

  if (!ok) {
    throw std::runtime_error("an error occured while parsing config file (" + std::to_string(root.GetErrorOffset()) + ")");
  }

  if (root.HasMember("server")) {
    auto server = root["server"].GetObject();

    cfg.server.ip = server["ip"].GetString();
    cfg.server.port = server["port"].GetUint();
    cfg.server.thread_pool_size = server["thread_pool_size"].GetUint();
    cfg.server.read_timeout_sec = std::chrono::seconds(server["read_timeout_sec"].GetUint());
    cfg.server.read_timeout_sec = std::chrono::seconds(server["write_timeout_sec"].GetUint());
  } else {
    if (const auto ip = std::getenv("PRETTYURL_SERVER_IP"); ip) {
      cfg.server.ip = ip;
    } else {
      throw std::runtime_error("server ip does not specified");
    }

    if (const auto port = std::getenv("PRETTYURL_SERVER_PORT"); port) {
      const auto iport = std::stoi(port);

      if (iport < 1024 || iport > std::numeric_limits<std::uint16_t>::max()) {
        throw std::runtime_error("invalid port value");
      }

      cfg.server.ip = iport;
    } else {
      throw std::runtime_error("server port does not specified");
    }

    if (const auto thpool_size = std::getenv("PRETTYURL_SERVER_THPOOL_SIZE"); thpool_size) {
      cfg.server.thread_pool_size = std::stoi(thpool_size);
    }

    if (const auto read_to = std::getenv("PRETTYURL_SERVER_READ_TIMEOUT"); read_to) {
      cfg.server.read_timeout_sec = std::chrono::seconds(std::stoi(read_to));
    }

    if (const auto write_to = std::getenv("PRETTYURL_SERVER_WRITE_TIMEOUT"); write_to) {
      cfg.server.write_timeout_sec = std::chrono::seconds(std::stoi(write_to));
    }
  }

  if (root.HasMember("logging")) {
    auto logging = root["logging"].GetObject();

    cfg.logging.path = logging["path"].GetString();
    cfg.logging.level = logging["level"].GetString();
  } else {
    if (const auto log_path = std::getenv("PRETTYURL_LOG_PATH"); log_path) {
      cfg.logging.path = log_path;
    } else {
      throw std::runtime_error("log path does not specified");
    }

    if (const auto log_lvl = std::getenv("PRETTYURL_LOG_LEVEL"); log_lvl) {
      cfg.logging.level = log_lvl;
    }
  }

  if (root.HasMember("db")) {
    auto db = root["db"].GetObject();

    cfg.db.ip = db["ip"].GetString();
    cfg.db.port = db["port"].GetUint();

    if (db.HasMember("connect_timeout_sec")) {
      cfg.db.connect_timeout_sec = db["connect_timeout_sec"].GetUint();
    }
  } else {
    if (const auto db_ip = std::getenv("PRETTYURL_DB_IP"); db_ip) {
      cfg.db.ip = db_ip;
    } else {
      throw std::runtime_error("db ip does not specified");
    }

    if (const auto db_port = std::getenv("PRETTYURL_DB_PORT"); db_port) {
      const auto idb_port = std::stoi(db_port);

      if (idb_port < 1024 || idb_port > std::numeric_limits<std::uint16_t>::max()) {
        throw std::runtime_error("invalid db port value");
      }

      cfg.db.port = idb_port;
    } else {
      throw std::runtime_error("db port does not specified");
    }

    if (const auto db_conn_to = std::getenv("PRETTYURL_DB_CONNECT_TIMEOUT"); db_conn_to) {
      cfg.db.connect_timeout_sec = std::stoi(db_conn_to);
    }
  }

  if (const auto db_user = std::getenv("PRETTYURL_DB_USER"); db_user) {
    cfg.db.username = db_user;
  } else {
    throw std::runtime_error("db username does not specified");
  }

  if (const auto db_pass = std::getenv("PRETTYURL_DB_PASS"); db_pass) {
    cfg.db.password = db_pass;
  } else {
    throw std::runtime_error("db password does not specified");
  }

  if (const auto db_name = std::getenv("PRETTYURL_DB_NAME"); db_name) {
    cfg.db.db_name = db_name;
  } else {
    throw std::runtime_error("db name does not specified");
  }
}

} // namespace prettyurl::infra::config