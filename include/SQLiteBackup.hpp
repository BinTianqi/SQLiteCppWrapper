#pragma once

#include <sqlite3.h>

#include "./SQLiteDatabase.hpp"

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
        const std::string &destName,
        const SQLiteDatabase &srcDb,
        const std::string &srcName
    );

    /**
     * @throws sqlite_error if initialization failed
     */
    SQLiteBackup(
        const std::unique_ptr<SQLiteDatabase> &destDb,
        const std::string &destName,
        const std::unique_ptr<SQLiteDatabase> &srcDb,
        const std::string &srcName
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
