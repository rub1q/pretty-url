#pragma once

#include "prettyurl/core/utility/string_utils.hpp"

#include <vector>
#include <string>
#include <unordered_map>
#include <optional>

namespace prettyurl::core::db {

class query_result_set final {
public:
  using row_t = std::vector<std::optional<std::string>>;
  using rows_list_t = std::vector<row_t>;

  using columns_list_t = std::vector<std::string>;
  using columns_map_t = std::unordered_map<std::string, std::size_t>;

  class row final {
  public:
    explicit row(const row_t& rows_data, const columns_map_t& column_map)
      : row_data_(rows_data)
      , column_map_(column_map) {
    }

    [[nodiscard]] std::optional<std::string> operator[](std::string_view column_name) const;
    [[nodiscard]] std::optional<std::string> operator[](const std::size_t column_index) const;

    [[nodiscard]] std::size_t size() const;

    template <typename T>
    [[nodiscard]] std::optional<T> get(std::string_view column_name) const {
      const auto value = (*this)[column_name];
      
      if (!value.has_value()) {
        return std::nullopt;
      }

      return core::utility::string_to<T>(*value);
    }

    template <typename T>
    [[nodiscard]] std::optional<T> get(const std::size_t column_idx) const {
      const auto value = (*this)[column_idx];

      if (!value.has_value()) {
        return std::nullopt;
      }

      return core::utility::string_to<T>(*value);
    }

  private:
    const row_t& row_data_;
    const columns_map_t& column_map_;
  };

  void set_columns(const columns_list_t& columns);
  void add_row(const row_t& row_data);

  void reserve(const std::size_t size);

  [[nodiscard]] row operator[](const std::size_t index) const;

  [[nodiscard]] std::size_t size() const;

  [[nodiscard]] rows_list_t::const_iterator begin() const noexcept;
  [[nodiscard]] rows_list_t::const_iterator end() const noexcept;

private:
  rows_list_t rows_;
  columns_list_t columns_;
  columns_map_t column_map_;
};

} // namespace prettyurl::core::db