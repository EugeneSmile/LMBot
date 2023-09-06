#include "DataBaseHelper.h"

#include <spdlog/spdlog.h>

namespace lmb
{
    DataBaseHelper::DataBaseHelper(const std::string &db_filename)
    {
        if (sqlite3_open(db_filename.c_str(), &database))
        {
            spdlog::get("main")->error("Can't open database: {}", sqlite3_errmsg(database));
        }
        processCommand("PRAGMA synchronous = OFF");
        processCommand("PRAGMA journal_mode = MEMORY");
        processCommand("CREATE TABLE IF NOT EXISTS Messages (chat_id INTEGER,	message_id INTEGER, query_id INTEGER, timestamp INTEGER, PRIMARY KEY(chat_id, message_id, query_id))");
    }

    DataBaseHelper::~DataBaseHelper()
    {
        sqlite3_close(database);
    }

    int DataBaseHelper::processCommand(const std::string &command)
    {
        char *sql_error = 0;
        int return_code = 0;
        return_code = sqlite3_exec(database, command.c_str(), 0, 0, &sql_error);
        if (return_code)
        {
            spdlog::get("main")->error("SQL Error: Code: {}, Text: {}, Command: {}", return_code, std::string(sql_error), command);
        }
        return return_code;
    }

    void DataBaseHelper::addMessage(int64_t chat_id, int32_t message_id, bool query)
    {
        std::string param = query ? "query_id" : "message_id";
        processCommand("INSERT INTO Messages (chat_id, " + param + ", timestamp) VALUES (" + std::to_string(chat_id) + ", " + std::to_string(message_id) + ", " + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ")");
    }

    void DataBaseHelper::deleteMessages(const std::vector<std::pair<int64_t, int32_t>> &ids, bool query)
    {
        std::string param = query ? "query_id" : "message_id";
        processCommand("BEGIN TRANSACTION");
        for (const std::pair<int64_t, int32_t> &ids_pair : ids)
            processCommand("DELETE FROM Messages WHERE chat_id = '" + std::to_string(ids_pair.first) + "' AND " + param + " = '" + std::to_string(ids_pair.second) + "'");
        processCommand("END TRANSACTION");
    }

    int DataBaseHelper::getOutdatedMessagesCallback(void *data, int argc, char **argv, char **azColName)
    {
        std::vector<std::pair<int64_t, int32_t>> *casted_data = static_cast<std::vector<std::pair<int64_t, int32_t>> *>(data);
        std::pair<int64_t, int32_t> ids_pair{std::stoi(argv[0]), std::stoi(argv[1])};
        casted_data->push_back(ids_pair);

        if (argc == 0)
            return 1;
        return 0;
    }

    void DataBaseHelper::getOutdated(int64_t cleaner_sleeptime_ns, std::vector<std::pair<int64_t, int32_t>> &result)
    {
        const std::string get_outdated_messages = "SELECT chat_id, message_id FROM Messages WHERE " + std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count()) + " - timestamp > " + std::to_string(cleaner_sleeptime_ns) + " AND message_id IS NOT NULL ORDER BY chat_id, message_id ASC";
        const std::string get_outdated_queries = "SELECT chat_id, query_id FROM Messages WHERE " + std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count()) + " - timestamp > " + std::to_string(cleaner_sleeptime_ns) + "AND query_id IS NOT NUL ORDER BY chat_id, message_id ASC";

        result.clear();

        int return_code = 0;
        return_code = sqlite3_exec(database, get_outdated_messages.c_str(), &DataBaseHelper::getOutdatedMessagesCallback, &result, NULL);

        if (return_code)
        {
            char *sql_error = 0;
            spdlog::get("main")->error("SQLite return code: {}, sql_error: {}", return_code, sql_error);
        }
    }

} // namespace lmb
