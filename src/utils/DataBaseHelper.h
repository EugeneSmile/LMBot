#ifndef LMBOT_DATABASEHELPER_H_GUARD
#define LMBOT_DATABASEHELPER_H_GUARD

#include <iostream>
#include <chrono>
#include <functional>
#include <cassert>

#include <sqlite3.h>

#include "Bot.h"

namespace lmbot
{
    class DataBaseHelper : public bot::Bot
    {
    private:
        int processCommand(const std::string &command);
        sqlite3 *database;
        const std::string database_filename = "lmbot.db";

    public:
        explicit DataBaseHelper(const std::string &db_filename);
        ~DataBaseHelper();
        void addMessage(int64_t chat_id, int32_t message_id, bool query = false);
        void deleteMessages(const std::vector<std::pair<int64_t, int32_t>> &ids, bool query = false);
        void getOutdated(int64_t cleaner_sleeptime_ns, std::vector<std::pair<int64_t, int32_t>> &result);
        static int getOutdatedMessagesCallback(void *data, int argc, char **argv, char **azColName);
    };

} // namespace lmbot
#endif