#ifndef LMB_BOT_BOT_H_GUARD
#define LMB_BOT_BOT_H_GUARD

#include <memory>

namespace lmb
{
    class LMBot;
    namespace bot
    {
        class Bot
        {
        protected:
            inline static std::shared_ptr<lmb::LMBot> bot;

        public:
            static void init(std::shared_ptr<lmb::LMBot> bot);
        };
    } // namespace bot
} // namespace lykomebot

#endif