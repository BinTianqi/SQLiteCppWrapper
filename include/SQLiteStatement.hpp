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
    /**
     * Use SQLiteDatabase::prepare() to prepare a statement
     */
    explicit SQLiteStatement(sqlite3_stmt *s) : _sqliteStmt(s) {}

    ~SQLiteStatement();

    /**
     * Wrapper of `sqlite3_step()`
     *
     * @throws sqlite_error if the returned value is neither `SQLITE_DONE` nor `SQLITE_ROW`
     */
    int step();

    /**
     * Wrapper of `sqlite3_reset()`
     *
     * @throws sqlite_error if there's a prior error
     */
    void reset();

    /**
     * Wrapper of `sqlite3_finalize()`
     *
     * Does nothing if the statement is already finalized.
     */
    void finalize();

    /**
     * Wrapper of `sqlite3_bind_parameter_index()`
     *
     * @throws sqlite_error if the parameter is not found
     */
    [[nodiscard]] int getBindParamIndex(const std::string &name) const;

    /**
     * Wrapper of `sqlite3_clear_bindings()`
     */
    void clearBindings();

    /**
     * Wrapper of `sqlite3_bind_blob64()`
     */
    void bindBlob(int index, const void *data, uint64_t size, SQLiteBindDestructor destructor);

    /**
     * Wrapper of `sqlite3_bind_double()`
     */
    void bindDouble(int index, double value);

    /**
     * Wrapper of `sqlite3_bind_int()`
     */
    void bindInt(int index, int value);

    /**
     * Wrapper of `sqlite3_bind_int64()`
     */
    void bindInt64(int index, int64_t value);

    /**
     * Wrapper of `sqlite3_bind_null()`
     */
    void bindNull(int index);

    /**
     * Wrapper of `sqlite3_bind_text()`
     */
    void bindString(int index, const std::string &text, SQLiteBindDestructor destructor);

    /**
     * Wrapper of `sqlite3_bind_text16()`
     */
    void bindU16string(int index, const std::u16string &text, SQLiteBindDestructor destructor);

    /**
     * Wrapper of `sqlite3_bind_text()`
     */
    void bindText(int index, const char *text, int length, SQLiteBindDestructor destructor);

    /**
     * Wrapper of `sqlite3_bind_text16()`
     */
    void bindText16(int index, const void *text, int length, SQLiteBindDestructor destructor);

    /**
     * Wrapper of `sqlite3_bind_text64()`
     */
    void bindText64(int index, const char *text, uint64_t size, SQLiteBindDestructor destructor, int encoding);

    /**
     * Wrapper of `sqlite3_bind_value()`
     */
    void bindValue(int index, const sqlite3_value *value);

    /**
     * Wrapper of `sqlite3_bind_pointer()`
     */
    void bindPointer(int index, void *pointer, const char *type, SQLiteBindDestructor destructor);

    /**
     * Wrapper of `sqlite3_bind_zeroblob()`
     */
    void bindZeroBlob(int index, int size);

    /**
     * Wrapper of `sqlite3_bind_zeroblob64()`
     */
    void bindZeroBlob64(int index, uint64_t size);

    /**
     * Wrapper of `sqlite3_column_blob()`
     *
     * @throws sqlite_error if column index out of range
     */
    [[nodiscard]] const void *getBlob(int index) const;

    /**
     * Wrapper of `sqlite3_column_double()`
     *
     * @throws sqlite_error if column index out of range
     */
    [[nodiscard]] double getDouble(int index) const;

    /**
     * Wrapper of `sqlite3_column_int()`
     *
     * @throws sqlite_error if column index out of range
     */
    [[nodiscard]] int getInt(int index) const;

    /**
     * Wrapper of `sqlite3_column_int64()`
     *
     * @throws sqlite_error if column index out of range
     */
    [[nodiscard]] int64_t getInt64(int index) const;

    /**
     * Wrapper of `sqlite3_column_text()`
     *
     * @throws sqlite_error if column index out of range
     */
    [[nodiscard]] const unsigned char *getText(int index) const;

    /**
     * Wrapper of `sqlite3_column_text16()`
     *
     * @throws sqlite_error if column index out of range
     */
    [[nodiscard]] const void *getText16(int index) const;

    /**
     * Wrapper of `sqlite3_column_text()`
     *
     * @throws sqlite_error if column index out of range
     */
    [[nodiscard]] std::string getString(int index) const;

    /**
     * Wrapper of `sqlite3_column_text16()`
     *
     * @throws sqlite_error if column index out of range
     */
    [[nodiscard]] std::u16string getU16string(int index) const;

    /**
     * Wrapper of `sqlite3_column_value()`
     *
     * @throws sqlite_error if column index out of range
     */
    [[nodiscard]] sqlite3_value *getValue(int index) const;

    /**
     * Wrapper of `sqlite3_column_bytes()`
     *
     * @throws sqlite_error if column index out of range
     */
    [[nodiscard]] int getBytes(int index) const;

    /**
     * Wrapper of `sqlite3_column_bytes16()`
     *
     * @throws sqlite_error if column index out of range
     */
    [[nodiscard]] int getBytes16(int index) const;

    /**
     * Wrapper of `sqlite3_column_type()`
     *
     * @throws sqlite_error if column index out of range
     */
    [[nodiscard]] int getType(int index) const;

    /**
     * Wrapper of `sqlite3_column_count()`
     */
    [[nodiscard]] int getColumnCount() const;

    /**
     * Wrapper of `sqlite3_data_count()`
     */
    [[nodiscard]] int getDataCount() const;

    /**
     * Gets the internal `sqlite3_stmt` handle
     */
    [[nodiscard]] sqlite3_stmt *getSqliteStmt() const;

private:
    sqlite3_stmt *_sqliteStmt;
    bool _stmtFinalized = false;

    void checkNotFinalized() const;

    static void checkBindOk(int code);

    void checkColumnIndex(int index) const;
};
