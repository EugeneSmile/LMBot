#ifndef LMBOT_MANAGERS_CALLBACKQUERY_H_GUARD
#define LMBOT_MANAGERS_CALLBACKQUERY_H_GUARD

#include <string>
#include <vector>

#include <tgbot/tgbot.h>

#include "Manager.h"

namespace lmbot
{
    class LMBot;
    namespace managers
    {

        class CallbackQuery : public Manager
        {
        public:
            void process(const TgBot::CallbackQuery::Ptr query);
        };

    } // namespace managers
} // namespace lmbot

#endif