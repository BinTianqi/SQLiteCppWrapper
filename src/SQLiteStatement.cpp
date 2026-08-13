#include "SQLiteStatement.hpp"

#include <format>

#include "SQLiteException.hpp"

SQLiteStatement::~SQLiteStatement() {
    finalize();
}

int SQLiteStatement::step() {
    checkNotFinalized();
    const auto ret = sqlite3_step(_sqliteStmt);
    if (ret != SQLITE_OK && ret != SQLITE_DONE && ret != SQLITE_ROW) {
        throw sqlite_error("Failed to step statement", ret);
    }
    return ret;
}

void SQLiteStatement::reset() {
    checkNotFinalized();
    const auto ret = sqlite3_reset(_sqliteStmt);
    if (ret != SQLITE_OK && ret != SQLITE_DONE && ret != SQLITE_ROW) {
        throw sqlite_error("Failed to reset statement", ret);
    }
}

void SQLiteStatement::finalize() {
    if (!_stmtFinalized) {
        sqlite3_finalize(_sqliteStmt);
        _stmtFinalized = true;
    }
}

int SQLiteStatement::getBindParamIndex(std::string_view name) const {
    checkNotFinalized();
    const auto index = sqlite3_bind_parameter_index(_sqliteStmt, name.data());
    if (index == 0) {
        throw sqlite_error(std::format("Parameter {} not found", name));
    }
    return index;
}

void SQLiteStatement::clearBindings() {
    checkNotFinalized();
    sqlite3_clear_bindings(_sqliteStmt);
}

void SQLiteStatement::bindBlob(
    const int index, const void *data, const uint64_t size, const SQLiteBindDestructor destructor
) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_blob64(_sqliteStmt, index, data, size, destructor);
    checkBindOk(ret);
}

void SQLiteStatement::bindBlob(
    std::string_view param, const void *data, uint64_t size, SQLiteBindDestructor destructor
) {
    bindBlob(getBindParamIndex(param), data, size, destructor);
}

void SQLiteStatement::bindDouble(const int index, const double value) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_double(_sqliteStmt, index, value);
    checkBindOk(ret);
}

void SQLiteStatement::bindDouble(std::string_view param, double value) {
    bindDouble(getBindParamIndex(param), value);
}

void SQLiteStatement::bindInt(const int index, const int value) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_int(_sqliteStmt, index, value);
    checkBindOk(ret);
}

void SQLiteStatement::bindInt(std::string_view param, int value) {
    bindInt(getBindParamIndex(param), value);
}

void SQLiteStatement::bindInt64(const int index, const int64_t value) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_int64(_sqliteStmt, index, value);
    checkBindOk(ret);
}

void SQLiteStatement::bindInt64(std::string_view param, int64_t value) {
    bindInt64(getBindParamIndex(param), value);
}

void SQLiteStatement::bindNull(const int index) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_null(_sqliteStmt, index);
    checkBindOk(ret);
}

void SQLiteStatement::bindNull(std::string_view param) {
    bindNull(getBindParamIndex(param));
}

void SQLiteStatement::bindText(
    const int index, const std::string_view text, const SQLiteBindDestructor destructor
) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_text(_sqliteStmt, index, text.data(), static_cast<int>(text.length()), destructor);
    checkBindOk(ret);
}

void SQLiteStatement::bindText(
    std::string_view param, std::string_view text, SQLiteBindDestructor destructor
) {
    bindText(getBindParamIndex(param), text, destructor);
}

void SQLiteStatement::bindText16(
    const int index, const std::u16string_view text, const SQLiteBindDestructor destructor
) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_text16(_sqliteStmt, index, text.data(), static_cast<int>(text.length()), destructor);
    checkBindOk(ret);
}

void SQLiteStatement::bindText16(
    std::string_view param, std::u16string_view text, SQLiteBindDestructor destructor
) {
    bindText16(getBindParamIndex(param), text, destructor);
}

void SQLiteStatement::bindText64(
    const int index, const std::string_view text, const SQLiteBindDestructor destructor, const int encoding
) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_text64(
        _sqliteStmt, index, text.data(), text.length(), destructor, encoding
    );
    checkBindOk(ret);
}

void SQLiteStatement::bindText64(
    std::string_view param, std::string_view text, SQLiteBindDestructor destructor, int encoding
) {
    bindText64(getBindParamIndex(param), text, destructor, encoding);
}

void SQLiteStatement::bindValue(const int index, const sqlite3_value *value) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_value(_sqliteStmt, index, value);
    checkBindOk(ret);
}

void SQLiteStatement::bindValue(std::string_view param, const sqlite3_value *value) {
    bindValue(getBindParamIndex(param), value);
}

void SQLiteStatement::bindPointer(
    const int index, void *pointer, const char *type, const SQLiteBindDestructor destructor
) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_pointer(_sqliteStmt, index, pointer, type, destructor);
    checkBindOk(ret);
}

void SQLiteStatement::bindPointer(
    std::string_view param, void *pointer, const char *type, SQLiteBindDestructor destructor
) {
    bindPointer(getBindParamIndex(param), pointer, type, destructor);
}

void SQLiteStatement::bindZeroBlob(const int index, const int size) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_zeroblob(_sqliteStmt, index, size);
    checkBindOk(ret);
}

void SQLiteStatement::bindZeroBlob(std::string_view param, int size) {
    bindZeroBlob(getBindParamIndex(param), size);
}

void SQLiteStatement::bindZeroBlob64(const int index, const uint64_t size) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_zeroblob64(_sqliteStmt, index, size);
    checkBindOk(ret);
}

void SQLiteStatement::bindZeroBlob64(std::string_view param, uint64_t size) {
    bindZeroBlob64(getBindParamIndex(param), size);
}

const void *SQLiteStatement::getBlob(const int index) const {
    checkColumnIndex(index);
    return sqlite3_column_blob(_sqliteStmt, index);
}

double SQLiteStatement::getDouble(const int index) const {
    checkColumnIndex(index);
    return sqlite3_column_double(_sqliteStmt, index);
}

int SQLiteStatement::getInt(const int index) const {
    checkColumnIndex(index);
    return sqlite3_column_int(_sqliteStmt, index);
}

int64_t SQLiteStatement::getInt64(const int index) const {
    checkColumnIndex(index);
    return sqlite3_column_int64(_sqliteStmt, index);
}

const unsigned char *SQLiteStatement::getText(const int index) const {
    checkColumnIndex(index);
    return sqlite3_column_text(_sqliteStmt, index);
}

const void *SQLiteStatement::getText16(const int index) const {
    checkColumnIndex(index);
    return sqlite3_column_text16(_sqliteStmt, index);
}

std::string SQLiteStatement::getString(const int index) const {
    checkColumnIndex(index);
    const auto ptr = sqlite3_column_text(_sqliteStmt, index);
    return {reinterpret_cast<const char *>(ptr)};
}

std::u16string SQLiteStatement::getU16string(const int index) const {
    checkColumnIndex(index);
    const auto ptr = sqlite3_column_text16(_sqliteStmt, index);
    return {static_cast<const char16_t *>(ptr)};
}

sqlite3_value *SQLiteStatement::getValue(const int index) const {
    checkColumnIndex(index);
    return sqlite3_column_value(_sqliteStmt, index);
}

int SQLiteStatement::getBytes(const int index) const {
    checkColumnIndex(index);
    return sqlite3_column_bytes(_sqliteStmt, index);
}

int SQLiteStatement::getBytes16(const int index) const {
    checkColumnIndex(index);
    return sqlite3_column_bytes16(_sqliteStmt, index);
}

int SQLiteStatement::getType(const int index) const {
    checkColumnIndex(index);
    return sqlite3_column_type(_sqliteStmt, index);
}

int SQLiteStatement::getColumnCount() const {
    checkNotFinalized();
    return sqlite3_column_count(_sqliteStmt);
}

int SQLiteStatement::getDataCount() const {
    checkNotFinalized();
    return sqlite3_data_count(_sqliteStmt);
}

sqlite3_stmt *SQLiteStatement::getSqliteStmt() const {
    return _sqliteStmt;
}

void SQLiteStatement::checkNotFinalized() const {
    if (_stmtFinalized) {
        throw sqlite_error("SQLiteStatement is already finalized");
    }
}

void SQLiteStatement::checkBindOk(const int code) {
    if (code != SQLITE_OK) {
        throw sqlite_error("Failed to bind parameter", code);
    }
}

void SQLiteStatement::checkColumnIndex(const int index) const {
    checkNotFinalized();
    const auto count = sqlite3_column_count(_sqliteStmt);
    if (index >= count) {
        throw sqlite_error("Column index out of range");
    }
}
