#pragma once

#include <cstdint>
#include <sqlite3.h>

#include "./SQLiteException.hpp"

using SQLiteBindDestructor = void(*)(void *);

/**
 * Wrapper of `sqlite3_stmt`
 */
class SQLiteStatement {
public:
    explicit SQLiteStatement(sqlite3_stmt *s) : _sqliteStmt(s) {}

    ~SQLiteStatement();

    int step();

    void reset();

    void finalize();

    /** @throws sqlite_error if the parameter is not found */
    [[nodiscard]] int getBindParamIndex(const std::string &name) const;

    void clearBindings();

    void bindBlob(int index, const void *data, uint64_t size, SQLiteBindDestructor destructor);

    void bindDouble(int index, double value);

    void bindInt(int index, int value);

    void bindInt64(int index, int64_t value);

    void bindNull(int index);

    void bindString(int index, const std::string &text, SQLiteBindDestructor destructor);

    void bindU16string(int index, const std::u16string &text, SQLiteBindDestructor destructor);

    void bindText(int index, const char *text, int length, SQLiteBindDestructor destructor);

    void bindText16(int index, const void *text, int length, SQLiteBindDestructor destructor);

    void bindText64(int index, const char *text, uint64_t size, SQLiteBindDestructor destructor, int encoding);

    void bindValue(int index, const sqlite3_value *value);

    void bindPointer(int index, void *pointer, const char *type, SQLiteBindDestructor destructor);

    void bindZeroBlob(int index, int size);

    void bindZeroBlob64(int index, uint64_t size);

    [[nodiscard]] const void *getBlob(int index) const;

    [[nodiscard]] double getDouble(int index) const;

    [[nodiscard]] int getInt(int index) const;

    [[nodiscard]] int64_t getInt64(int index) const;

    [[nodiscard]] const unsigned char *getText(int index) const;

    [[nodiscard]] const void *getText16(int index) const;

    [[nodiscard]] std::string getString(int index) const;

    [[nodiscard]] std::u16string getU16string(int index) const;

    [[nodiscard]] sqlite3_value *getValue(int index) const;

    [[nodiscard]] int getBytes(int index) const;

    [[nodiscard]] int getBytes16(int index) const;

    [[nodiscard]] int getType(int index) const;

    [[nodiscard]] int getColumnCount() const;

    [[nodiscard]] int getDataCount() const;

    [[nodiscard]] sqlite3_stmt *getSqliteStmt() const;

private:
    sqlite3_stmt *_sqliteStmt;
    bool _stmtFinalized = false;

    void checkNotFinalized() const;

    static void checkBindOk(int code);

    void checkColumnIndex(int index) const;
};
