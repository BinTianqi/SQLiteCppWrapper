#pragma once

#include <stdexcept>

#include <sqlite3.h>

class sqlite_error : public std::runtime_error {
public:
    explicit sqlite_error(
        const std::string &msg, const int ret = 0
    ) : std::runtime_error(generateMessage(msg, ret)), resCode(ret) {}

private:
    int resCode;

    static std::string generateMessage(const std::string &baseMessage, const int ret = 0) {
        if (ret == 0) return baseMessage;
        return baseMessage + ": " + sqlite3_errstr(ret);
    }
};
