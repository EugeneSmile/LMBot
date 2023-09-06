#ifndef LMB_MANAGERS_CALLBACKQUERY_H_GUARD
#define LMB_MANAGERS_CALLBACKQUERY_H_GUARD

#include <string>
#include <vector>

#include <tgbot/tgbot.h>

#include "Manager.h"

namespace lmb
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
} // namespace lmb

#endif