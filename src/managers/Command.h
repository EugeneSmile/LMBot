#ifndef LMB_MANAGERS_COMMAND_H_GUARD
#define LMB_MANAGERS_COMMAND_H_GUARD

#include <string>
#include <vector>

#include <tgbot/tgbot.h>

#include "Manager.h"

namespace lmb
{
    class LMBot;
    namespace managers
    {
        class Command : public Manager
        {
        public:
            void process(const TgBot::Message::Ptr message);
        };

    } // namespace managers
} // namespace lmb

#endif