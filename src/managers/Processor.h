#ifndef LMB_MANAGERS_PROCESSOR_H_GUARD
#define LMB_MANAGERS_PROCESSOR_H_GUARD

#include <thread>
#include <vector>

#include <tgbot/tgbot.h>

#include "Bot.h"
#include "Utilities.h"

namespace lmb
{
    class LMBot;
    namespace managers
    {

        class Processor : public bot::Bot
        {
        private:
            struct ThreadData
            {
                bool active = false;
                std::thread thread;
                std::thread::id id;
                TgBot::Message::Ptr message;
                TgBot::CallbackQuery::Ptr callback_query;
                TgBot::InlineQuery::Ptr inline_query;
            };
            std::vector<ThreadData> threads;
            int32_t threads_limit;
            int32_t thread_wait_limit;

            bool findFreeThread(std::vector<ThreadData> &threads, std::vector<ThreadData>::iterator &iterator);
            void processMessageThread(const TgBot::Message::Ptr message);
            void processCallbackQueryThread(const TgBot::CallbackQuery::Ptr callback_query);
            void processInlineQueryThread(const TgBot::InlineQuery::Ptr inline_query);

        public:
            explicit Processor();
            void processMessage(const TgBot::Message::Ptr message);
            void processCallbackQuery(const TgBot::CallbackQuery::Ptr callback_query);
            void processInlineQuery(const TgBot::InlineQuery::Ptr inline_query);
            uint32_t getActiveThreads();
            uint32_t getTotalThreads();
        };

    } // namespace managers
} // namespace lmb

#endif