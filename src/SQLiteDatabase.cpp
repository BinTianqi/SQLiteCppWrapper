#include "SQLiteDatabase.hpp"

SQLiteDatabase::SQLiteDatabase(const std::filesystem::path &dbPath, const int flags, const char *vfs) {
    const auto ret = sqlite3_open_v2(dbPath.c_str(), &_sqliteDb, flags, vfs);
    if (ret != SQLITE_OK) {
        throw sqlite_error("Failed to open database", ret);
    }
}

SQLiteDatabase::~SQLiteDatabase()  {
    close();
}

void SQLiteDatabase::close() {
    if (!_dbClosed) {
        sqlite3_close_v2(_sqliteDb);
        _dbClosed = true;
    }
}

int SQLiteDatabase::execute(const std::string_view stmtText) const {
    auto stmt = prepare(stmtText);
    return stmt.step();
}

SQLiteStatement SQLiteDatabase::prepare(const std::string_view stmtText, const unsigned int flags) const {
    checkNotClosed();
    sqlite3_stmt *stmt;
    const auto ret = sqlite3_prepare_v3(
        _sqliteDb, stmtText.data(), static_cast<int>(stmtText.length()), flags, &stmt, nullptr
    );
    checkPrepareOk(ret);
    return SQLiteStatement(stmt);
}

SQLiteStatement SQLiteDatabase::prepare16(const std::u16string_view stmtText, const unsigned int flags) const {
    checkNotClosed();
    sqlite3_stmt *stmt;
    const auto ret = sqlite3_prepare16_v3(
        _sqliteDb, stmtText.data(), static_cast<int>(stmtText.length()), flags, &stmt, nullptr
    );
    checkPrepareOk(ret);
    return SQLiteStatement(stmt);
}

sqlite3 *SQLiteDatabase::getSqliteHandle() const {
    return _sqliteDb;
}

void SQLiteDatabase::checkNotClosed() const {
    if (_dbClosed) {
        throw sqlite_error("SQLiteDatabase is already closed");
    }
}

void SQLiteDatabase::checkPrepareOk(const int ret) {
    if (ret != SQLITE_OK) {
        throw sqlite_error("Failed to prepare statement", ret);
    }
}
