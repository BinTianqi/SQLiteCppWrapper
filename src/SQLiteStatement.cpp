#include "SQLiteStatement.hpp"

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

int SQLiteStatement::getBindParamIndex(const std::string &name) const {
    checkNotFinalized();
    const auto index = sqlite3_bind_parameter_index(_sqliteStmt, name.c_str());
    if (index == 0) {
        throw sqlite_error("Parameter " + name + " not found");
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

void SQLiteStatement::bindDouble(const int index, const double value) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_double(_sqliteStmt, index, value);
    checkBindOk(ret);
}

void SQLiteStatement::bindInt(const int index, const int value) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_int(_sqliteStmt, index, value);
    checkBindOk(ret);
}

void SQLiteStatement::bindInt64(const int index, const int64_t value) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_int64(_sqliteStmt, index, value);
    checkBindOk(ret);
}

void SQLiteStatement::bindNull(const int index) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_null(_sqliteStmt, index);
    checkBindOk(ret);
}

void SQLiteStatement::bindString(
    const int index, const std::string &text, const SQLiteBindDestructor destructor
) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_text(_sqliteStmt, index, text.c_str(), -1, destructor);
    checkBindOk(ret);
}

void SQLiteStatement::bindU16string(
    const int index, const std::u16string &text, const SQLiteBindDestructor destructor
) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_text16(_sqliteStmt, index, text.c_str(), -1, destructor);
    checkBindOk(ret);
}

void SQLiteStatement::bindText(
    const int index, const char *text, const int length, const SQLiteBindDestructor destructor
) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_text(_sqliteStmt, index, text, length, destructor);
    checkBindOk(ret);
}

void SQLiteStatement::bindText16(
    const int index, const void *text, const int length, const SQLiteBindDestructor destructor
) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_text16(_sqliteStmt, index, text, length, destructor);
    checkBindOk(ret);
}

void SQLiteStatement::bindText64(
    const int index, const char *text, const uint64_t size, const SQLiteBindDestructor destructor, const int encoding
) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_text64(
        _sqliteStmt, index, text, size, destructor, encoding
    );
    checkBindOk(ret);
}

void SQLiteStatement::bindValue(const int index, const sqlite3_value *value) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_value(_sqliteStmt, index, value);
    checkBindOk(ret);
}

void SQLiteStatement::bindPointer(
    const int index, void *pointer, const char *type, const SQLiteBindDestructor destructor
) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_pointer(_sqliteStmt, index, pointer, type, destructor);
    checkBindOk(ret);
}

void SQLiteStatement::bindZeroBlob(const int index, const int size) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_zeroblob(_sqliteStmt, index, size);
    checkBindOk(ret);
}

void SQLiteStatement::bindZeroBlob64(const int index, const uint64_t size) {
    checkNotFinalized();
    const auto ret = sqlite3_bind_zeroblob64(_sqliteStmt, index, size);
    checkBindOk(ret);
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
