#pragma once

#include <sqlite3.h>

#include "./SQLiteDatabase.hpp"

class SQLiteBackup {
public:
    SQLiteBackup(
        const SQLiteDatabase &destDb,
        const std::string &destName,
        const SQLiteDatabase &srcDb,
        const std::string &srcName
    );

    SQLiteBackup(
        const std::unique_ptr<SQLiteDatabase> &destDb,
        const std::string &destName,
        const std::unique_ptr<SQLiteDatabase> &srcDb,
        const std::string &srcName
    );

    ~SQLiteBackup();

    int step(int page) const;

    [[nodiscard]] int getRemaining() const;

    [[nodiscard]] int getPageCount() const ;

    void finish();

private:
    bool finished = false;
    sqlite3_backup *handle;
};
