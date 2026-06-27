# SQLite CPP wrapper

A simplistic C++ wrapper of [SQLite](https://sqlite.org/).

Features:

- Release resource (close database or finalize statement) automatically on object destruction

Add this library to your project

```shell
cd your_project
git submodule add --depth=1 https://github.com/BinTianqi/SQLiteCppWrapper.git third_party/SQLiteCppWrapper
```

```cmake
find_package(SQLite3 REQUIRED)
add_subdirectory(third_party/SQLiteCppWrapper)
# Ensure SQLite3 is linked after sqlite_cpp_wrapper
target_link_libraries(your_app PUBLIC sqlite_cpp_wrapper SQLite3::SQLite3)
```

Using the library

```cpp
#include <SQLiteWrapper.hpp>

int main() {
    auto db = SQLiteDatabase("data.db");
    auto stmt = db.prepare("CREATE TABLE schema_version");
    stmt.step();
    // resources will be released automatically
}
```

You can create you own database class derived from `SQLiteDatabase`

```cpp
class MyDatabase : public SQLiteDatabase {
public:
    MyDatabase(const std::filesystem::path &dbPath) : SQLiteDatabase(dbPath);
};
```
