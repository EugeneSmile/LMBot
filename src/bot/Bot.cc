#include "Bot.h"

namespace lmbot
{
    namespace bot
    {
        void Bot::init(std::shared_ptr<lmbot::LMBot> bot)
        {
            Bot::bot = bot;
        }
    } // namespace bot
} // namespace lmbot
