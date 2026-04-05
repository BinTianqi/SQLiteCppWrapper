#include "SQLiteBackup.hpp"

SQLiteBackup::SQLiteBackup(
    const SQLiteDatabase &destDb, const std::string &destName, const SQLiteDatabase &srcDb, const std::string &srcName
) {
    handle = sqlite3_backup_init(
        destDb.getSqliteHandle(), destName.c_str(), srcDb.getSqliteHandle(), srcName.c_str()
    );
    if (handle == nullptr) {
        throw sqlite_error("Failed to initialize backup");
    }
}

SQLiteBackup::SQLiteBackup(
    const std::unique_ptr<SQLiteDatabase> &destDb,
    const std::string &destName,
    const std::unique_ptr<SQLiteDatabase> &srcDb,
    const std::string &srcName
) {
    handle = sqlite3_backup_init(
        destDb->getSqliteHandle(), destName.c_str(), srcDb->getSqliteHandle(), srcName.c_str()
    );
    if (handle == nullptr) {
        throw sqlite_error("Failed to initialize backup");
    }
}

SQLiteBackup::~SQLiteBackup() {
    finish();
}

int SQLiteBackup::step(const int page) const {
    const auto ret = sqlite3_backup_step(handle, page);
    if (ret != SQLITE_OK && ret != SQLITE_DONE) {
        throw sqlite_error("Failed to step backup", ret);
    }
    return ret;
}

int SQLiteBackup::getRemaining() const {
    return sqlite3_backup_remaining(handle);
}

int SQLiteBackup::getPageCount() const {
    return sqlite3_backup_pagecount(handle);
}

void SQLiteBackup::finish() {
    if (!finished) {
        const auto ret = sqlite3_backup_finish(handle);
        if (ret != SQLITE_OK) {
            throw sqlite_error("Failed to finish backup", ret);
        }
        finished = true;
    }
}
