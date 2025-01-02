#include "prettyurl/infra/db/pg_session.hpp"

namespace prettyurl::infra::db {

pg_session::~pg_session() {
  if (trn_) {
    rollback_transaction();
  }

  disconnect();
}

bool pg_session::is_connected() const {
  return conn_ && conn_->is_open();
}

void pg_session::reconnect() {
  disconnect();
  connect();
}

void pg_session::begin_transaction() {
  if (!is_connected()) {
    throw std::runtime_error("connection with db is absent");
  }

  if (trn_) {
    return; // transaction is already processing
  }
  
  trn_ = std::make_unique<pqxx::work>(*conn_);
}

void pg_session::commit_transaction() {
  if (!trn_) {
    return;
  }

  try {
    trn_->commit();
    trn_.release();
  } catch (const std::exception& e) {
    throw std::runtime_error("failed to commit transaction (" + std::string(e.what()) + ")");
  }
}

void pg_session::rollback_transaction() {
  if (!trn_) {
    return;
  }

  try {
    trn_->abort();
    trn_.release();
  } catch (const std::exception& e) {
    throw std::runtime_error("failed to rollback transaction (" + std::string(e.what()) + ")");
  }
}

std::optional<core::db::query_result_set> pg_session::execute(std::string_view query) {
  try {
    if (!trn_) {
      begin_transaction();
    }

    pqxx::result res;

    if (!query_params_.empty()) {
      res = trn_->exec_prepared(query.data(), query_params_);
    } else {
      res = trn_->exec(query);
    }

    commit_transaction();

    if (res.empty()) {
      return std::nullopt;
    }

    core::db::query_result_set result_set;
    core::db::query_result_set::columns_list_t columns;
    core::db::query_result_set::row_t row;

    result_set.reserve(res.size());
    columns.reserve(res.columns());
    row.reserve(res.columns());

    for (int i = 0 ; i < res.columns(); i++) {
      columns.emplace_back(res.column_name(i));
    }

    result_set.set_columns(columns);

    for (const auto& row_data : res) {
      for (int i = 0; i < res.columns(); i++) {
        if (row_data[i].is_null()) {
          row.emplace_back(std::nullopt);
        } else {
          row.emplace_back(row_data[i].c_str());
        }
      }
      
      result_set.add_row(row);
      row.clear();
    }

    return result_set;
  } catch (const std::exception& e) {
    rollback_transaction();

    throw std::runtime_error("an error occured while executing sql query (" + std::string(e.what()) + ")");
  }
}

void pg_session::connect() {
  if (conn_) {
    conn_.release();
  }
  
  conn_ = std::make_unique<pqxx::connection>(conn_string_);
  
  if (!is_connected()) {
    throw std::runtime_error("failed to open db connection");
  }
}

void pg_session::disconnect() {
  if (!conn_) {
    return;
  }
  
  if (is_connected()) {
    conn_->close();
  }

  conn_.release();
}

} // namespace prettyurl::infra::db