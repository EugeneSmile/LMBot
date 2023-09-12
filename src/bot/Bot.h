#ifndef LMBOT_BOT_BOT_H_GUARD
#define LMBOT_BOT_BOT_H_GUARD

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/async_logger.h>

namespace lmbot
{
    class LMBot;
    namespace bot
    {
        class Bot
        {
        protected:
            std::shared_ptr<spdlog::async_logger> logger;
            inline static std::shared_ptr<lmbot::LMBot> bot;

        public:
            static void init(std::shared_ptr<lmbot::LMBot> bot);
        };
    } // namespace bot
} // namespace lykomebot

#endif