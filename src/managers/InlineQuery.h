#ifndef LMB_MANAGERS_INLINEQUERY_H_GUARD
#define LMB_MANAGERS_INLINEQUERY_H_GUARD

#include <tgbot/tgbot.h>

#include "Manager.h"

namespace lmb
{
    class LMBot;
    namespace managers
    {

        class InlineQuery : public Manager
        {
        public:
            void process(const TgBot::InlineQuery::Ptr query);
        };

    } // namespace managers
} // namespace lmb

#endif