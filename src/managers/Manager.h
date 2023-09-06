#ifndef LMB_MANAGERS_MANAGER_H_GUARD
#define LMB_MANAGERS_MANAGER_H_GUARD

#include <string>
#include <vector>

#include "tgbot/tgbot.h"

#include "Bot.h"

namespace lmb
{
    class LMBot;
    namespace managers
    {
        class Manager : public bot::Bot
        {
        protected:
            std::vector<std::string> command;

        public:
            virtual void process(const TgBot::Message::Ptr message);
            virtual void process(const TgBot::InlineQuery::Ptr query);
        };

    } // namespace modules
} // namespace lmb

#endif