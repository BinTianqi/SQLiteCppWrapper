#pragma once

#include <sqlite3.h>

#include "SQLiteDatabase.hpp"

/**
 * Wrapper of `sqlite3_backup`
 */
class SQLiteBackup {
public:
    /**
     * @throws sqlite_error if initialization failed
     */
    SQLiteBackup(
        const SQLiteDatabase &destDb,
        std::string_view destName,
        const SQLiteDatabase &srcDb,
        std::string_view srcName
    );

    /**
     * @throws sqlite_error if initialization failed
     */
    SQLiteBackup(
        const std::unique_ptr<SQLiteDatabase> &destDb,
        std::string_view destName,
        const std::unique_ptr<SQLiteDatabase> &srcDb,
        std::string_view srcName
    );

    ~SQLiteBackup();

    /**
     * Wrapper of `sqlite3_backup_step()`
     */
    int step(int page) const;

    /**
     * Wrapper of `sqlite3_backup_remaining()`
     */
    [[nodiscard]] int getRemaining() const;

    /**
     * Wrapper of `sqlite3_backup_pagecount()`
     */
    [[nodiscard]] int getPageCount() const;

    /**
     * Wrapper of `sqlite3_backup_pagecount()`
     */
    void finish();

private:
    bool finished = false;
    sqlite3_backup *handle;
};
