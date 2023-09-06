#include "Module.h"

#include "LMBot.h"

namespace lmb
{
    namespace modules
    {
        void Module::process(TgBot::Message::Ptr message)
        {
            spdlog::get("main")->warn("Function to process MESSAGE is not implemented in derived module class");
        }
        void Module::process(TgBot::CallbackQuery::Ptr query)
        {
            spdlog::get("main")->warn("Function to process CALLBACK QUERY is not implemented in derived module class");
        }
        void Module::process(TgBot::InlineQuery::Ptr query)
        {
            spdlog::get("main")->warn("Function to process INLINE QUERY is not implemented in derived module class");
        }

    } // namespace modules
} // namespace lmb
