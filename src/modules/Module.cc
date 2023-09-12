#include "Module.h"

#include "LMBot.h"

namespace lmbot
{
    namespace modules
    {
        void Module::init(std::shared_ptr<LMBot> bot)
        {
            this->bot = bot;
        }
        void Module::process(TgBot::Message::Ptr message, const std::vector<std::string> &command)
        {
            logger->warn("Function to process MESSAGE is not implemented in derived module class");
        }
        void Module::process(TgBot::CallbackQuery::Ptr query, const std::vector<std::string> &command)
        {
            logger->warn("Function to process CALLBACK QUERY is not implemented in derived module class");
        }
        void Module::process(TgBot::InlineQuery::Ptr query, const std::vector<std::string> &command)
        {
            logger->warn("Function to process INLINE QUERY is not implemented in derived module class");
        }

    } // namespace modules
} // namespace lmbot
