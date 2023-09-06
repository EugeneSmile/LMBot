#include "Manager.h"

#include "LMBot.h"

namespace lmb
{
    namespace managers
    {
        void Manager::process(const TgBot::Message::Ptr message)
        {
            spdlog::get("main")->warn("Function to process INLINE QUERY is not implemented in derived manager class");
        }
        void Manager::process(const TgBot::InlineQuery::Ptr query)
        {
            spdlog::get("main")->warn("Function to process INLINE QUERY is not implemented in derived manager class");
        }
    } // namespace managers
} // namespace lmb
