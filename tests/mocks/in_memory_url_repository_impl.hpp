#pragma once

#include "../../include/prettyurl/core/repositories/url_repository.hpp"

#include <unordered_map>
#include <stdexcept>

class in_memory_url_repository_impl final : public prettyurl::core::repositories::url_repository {
public:
  [[nodiscard]] std::uint64_t get_last_id() final {
    return last_id_;
  }

  [[nodiscard]] std::optional<std::string> get_long_url(std::string_view short_url) final {
    if (!short_to_long_url_map_.contains(std::string(short_url.data()))) {
      return std::nullopt;
    }

    return short_to_long_url_map_.at(std::string(short_url.data()));
  }

  [[nodiscard]] std::optional<std::string> get_short_url(std::string_view long_url) final {
    if (!long_to_short_url_map_.contains(std::string(long_url.data()))) {
      return std::nullopt;
    }

    return long_to_short_url_map_.at(std::string(long_url.data()));
  }
  
  void add_short_url(std::string_view short_url, std::string_view long_url) final {
    if (short_to_long_url_map_.contains(short_url.data())) {
      throw std::runtime_error("short url already exists");
    }

    short_to_long_url_map_.emplace(short_url, long_url);
    long_to_short_url_map_.emplace(long_url, short_url);

    ++last_id_;
  }

private:
  std::uint64_t last_id_ { 0u };

  std::unordered_map<std::string, std::string> short_to_long_url_map_;
  std::unordered_map<std::string, std::string> long_to_short_url_map_;
};