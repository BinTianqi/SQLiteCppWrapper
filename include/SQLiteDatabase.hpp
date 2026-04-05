#pragma once

#include <filesystem>
#include <sqlite3.h>

#include "./SQLiteStatement.hpp"

namespace fs = std::filesystem;

/** Wrapper of \ref sqlite3  */
class SQLiteDatabase {
public:
    /**
     * Open a database connection.
     * @param dbPath Path of the database file
     * @param flags SQLite open flags. Default to SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE
     * @param vfs SQLite VFS module
     */
    explicit SQLiteDatabase(
        const fs::path &dbPath, int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        const char *vfs = nullptr
    );

    ~SQLiteDatabase();

    /** Close the database connection if it's not already closed. */
    void close();

    int execute(const std::string &stmtText) const;

    /**
     * Wrapper of sqlite3_prepare_v3
     */
    [[nodiscard]] SQLiteStatement prepare(const std::string &stmtText, unsigned int flags = 0) const;

    /** @see prepare */
    [[nodiscard]] SQLiteStatement prepare16(const std::u16string &stmtText, unsigned int flags = 0) const;

    [[nodiscard]] sqlite3 *getSqliteHandle() const;

private:
    sqlite3 *_sqliteDb = nullptr;
    bool _dbClosed = false;

    void checkNotClosed() const;

    static void checkPrepareOk(int ret);
};
