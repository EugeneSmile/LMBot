#ifndef LMB_MODULES_MODULE_H_GUARD
#define LMB_MODULES_MODULE_H_GUARD

#include <memory>
#include <variant>

#include "tgbot/tgbot.h"

#include "Bot.h"

namespace lmb
{
    class LMBot;
    namespace modules
    {

        class Module : public bot::Bot
        {
        public:
            virtual void process(TgBot::Message::Ptr message);
            virtual void process(TgBot::CallbackQuery::Ptr query);
            virtual void process(TgBot::InlineQuery::Ptr query);
        };

    } // namespace modules
} // namespace lmb

#endif