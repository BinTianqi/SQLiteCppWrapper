#pragma once

#include <cstdint>
#include <string_view>

#include <sqlite3.h>

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
    [[nodiscard]] int getBindParamIndex(std::string_view name) const;

    /**
     * Wrapper of `sqlite3_clear_bindings()`
     */
    void clearBindings();

    /**
     * Wrapper of `sqlite3_bind_blob64()`
     */
    void bindBlob(int index, const void *data, uint64_t size, SQLiteBindDestructor destructor);

    /**
     * Shorthand for `getBindParamIndex()` + `bindBlob()`
     */
    void bindBlob(std::string_view param, const void *data, uint64_t size, SQLiteBindDestructor destructor);

    /**
     * Wrapper of `sqlite3_bind_double()`
     */
    void bindDouble(int index, double value);

    /** 
     * Shorthand for `getBindParamIndex()` + `bindDouble()`
     */
    void bindDouble(std::string_view param, double value);

    /**
     * Wrapper of `sqlite3_bind_int()`
     */
    void bindInt(int index, int value);

    /** 
     * Shorthand for `getBindParamIndex()` + `bindInt()`
     */
    void bindInt(std::string_view param, int value);

    /**
     * Wrapper of `sqlite3_bind_int64()`
     */
    void bindInt64(int index, int64_t value);

    /** 
     * Shorthand for `getBindParamIndex()` + `bindInt64()`
     */
    void bindInt64(std::string_view param, int64_t value);

    /**
     * Wrapper of `sqlite3_bind_null()`
     */
    void bindNull(int index);

    /** 
     * Shorthand for `getBindParamIndex()` + `bindNull()`
     */
    void bindNull(std::string_view param);

    /**
     * Wrapper of `sqlite3_bind_text()`
     */
    void bindText(int index, std::string_view text, SQLiteBindDestructor destructor);

    /** 
     * Shorthand for `getBindParamIndex()` + `bindText()`
     */
    void bindText(std::string_view param, std::string_view text, SQLiteBindDestructor destructor);

    /**
     * Wrapper of `sqlite3_bind_text16()`
     */
    void bindText16(int index, std::u16string_view text, SQLiteBindDestructor destructor);

    /** 
     * Shorthand for `getBindParamIndex()` + `bindText16()`
     */
    void bindText16(std::string_view param, std::u16string_view text, SQLiteBindDestructor destructor);

    /**
     * Wrapper of `sqlite3_bind_text64()`
     */
    void bindText64(int index, std::string_view text, SQLiteBindDestructor destructor, int encoding);

    /** 
     * Shorthand for `getBindParamIndex()` + `bindText64()`
     */
    void bindText64(std::string_view param, std::string_view text, SQLiteBindDestructor destructor, int encoding);

    /**
     * Wrapper of `sqlite3_bind_value()`
     */
    void bindValue(int index, const sqlite3_value *value);

    /** 
     * Shorthand for `getBindParamIndex()` + `bindValue()`
     */
    void bindValue(std::string_view param, const sqlite3_value *value);

    /**
     * Wrapper of `sqlite3_bind_pointer()`
     */
    void bindPointer(int index, void *pointer, const char *type, SQLiteBindDestructor destructor);

    /** 
     * Shorthand for `getBindParamIndex()` + `bindPointer()`
     */
    void bindPointer(std::string_view param, void *pointer, const char *type, SQLiteBindDestructor destructor);

    /**
     * Wrapper of `sqlite3_bind_zeroblob()`
     */
    void bindZeroBlob(int index, int size);

    /** 
     * Shorthand for `getBindParamIndex()` + `bindZeroBlob()`
     */
    void bindZeroBlob(std::string_view param, int size);

    /**
     * Wrapper of `sqlite3_bind_zeroblob64()`
     */
    void bindZeroBlob64(int index, uint64_t size);

    /** 
     * Shorthand for `getBindParamIndex()` + `bindZeroBlob64()`
     */
    void bindZeroBlob64(std::string_view param, uint64_t size);

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
