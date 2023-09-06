#ifndef LMB_UTILITIES_H_GUARD
#define LMB_UTILITIES_H_GUARD

#include <tgbot/tgbot.h>

#include <string>

namespace lmb
{
    bool splitString(const std::string &input, std::vector<std::string> &output, char delimiter);
    void toLower(std::string &str);
    std::string downloadFile(std::shared_ptr<TgBot::Bot> bot, std::string file_id);
} // namespace lmb
#endif