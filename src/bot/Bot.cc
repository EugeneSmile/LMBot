#include "Bot.h"

namespace lmb
{
    namespace bot
    {
        void Bot::init(std::shared_ptr<lmb::LMBot> bot)
        {
            Bot::bot = bot;
        }
    } // namespace bot
} // namespace lmb
