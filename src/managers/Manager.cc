#include "Manager.h"

#include "LMBot.h"

namespace lmbot
{
    namespace managers
    {
        void Manager::process(const TgBot::Message::Ptr message)
        {
            bot->logger->log()->warn("Function to process INLINE QUERY is not implemented in derived manager class");
        }
        void Manager::process(const TgBot::InlineQuery::Ptr query)
        {
            bot->logger->log()->warn("Function to process INLINE QUERY is not implemented in derived manager class");
        }
    } // namespace managers
} // namespace lmbot
