#include "prettyurl/core/db/query_result_set.hpp"

#include <stdexcept>

namespace prettyurl::core::db {

std::optional<std::string> query_result_set::row::operator[](std::string_view column_name) const { 
  if (!column_map_.contains(column_name.data())) {
    throw std::invalid_argument("column (" + std::string(column_name) + ") does not exist");
  }

  return row_data_.at(column_map_.at(column_name.data()));
}

std::optional<std::string> query_result_set::row::operator[](const std::size_t column_index) const {
  if (column_index >= size()) {
    throw std::out_of_range("column index is out of range");
  }

  return row_data_.at(column_index);
}

std::size_t query_result_set::row::size() const {
  return row_data_.size();
}

query_result_set::row query_result_set::operator[](const std::size_t index) const {
  if (index >= rows_.size()) {
    throw std::out_of_range("row index is out of range");
  }

  return row(rows_[index], column_map_);
}

void query_result_set::set_columns(const columns_list_t& columns) {
  columns_.reserve(columns.size());
  column_map_.reserve(columns.size());
  
  for (std::size_t idx = 0; const auto& column : columns) {
    columns_.push_back(column);
    column_map_[column] = idx++;
  }
}

void query_result_set::add_row(const row_t& row_data) {
  if (row_data.empty()) {
    return;
  }

  if (row_data.size() != columns_.size()) {
    throw std::runtime_error("the row size (" + std::to_string(row_data.size()) + 
      ") does not match the number of columns (" + std::to_string(columns_.size()) + ")");
  }

  rows_.push_back(row_data);
}

std::size_t query_result_set::size() const {
  return rows_.size();
}

query_result_set::rows_list_t::const_iterator query_result_set::begin() const noexcept {
  return rows_.begin();
}

query_result_set::rows_list_t::const_iterator query_result_set::end() const noexcept {
  return rows_.end();
}

void query_result_set::reserve(const std::size_t size) {
  rows_.reserve(size);
}

} // namespace prettyurl::core::db