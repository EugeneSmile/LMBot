#include "Utilities.h"

namespace lmb
{
    std::string downloadFile(std::shared_ptr<TgBot::Bot> bot, std::string file_id)
    {
        return bot->getApi().downloadFile(bot->getApi().getFile(file_id)->filePath);
    }

    void toLower(std::string &str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                       { return std::tolower(c); });
    }

    bool splitString(const std::string &input, std::vector<std::string> &output, char delimiter)
    {
        size_t pos = input.find(delimiter);
        size_t initialPos = 0;
        output.resize(0);

        // Decompose statement
        while (pos != std::string::npos)
        {
            output.push_back(input.substr(initialPos, pos - initialPos));
            initialPos = pos + 1;
            pos = input.find(delimiter, initialPos);
        }

        // Add the last one
        output.push_back(input.substr(initialPos, std::min(pos, input.size()) - initialPos + 1));

        if (output.size() <= 1)
        {
            return false;
        }
        return true;
    }
} // namespace lmb