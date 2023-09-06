#ifndef LMB_MODULES_CHATHELPER_H_GUARD
#define LMB_MODULES_CHATHELPER_H_GUARD

#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <memory>

#include <tgbot/tgbot.h>

#include "Module.h"
#include "DataBaseHelper.h"

namespace lmb
{
    namespace modules
    {

        class ChatHelper : public Module
        {
        private:
            std::shared_ptr<DataBaseHelper> database_handler;
            std::vector<std::pair<std::int64_t, std::int32_t>> messages_to_delete;
            std::thread cleaner_thread;
            int32_t cleaner_sleeptime{30};
            std::chrono::nanoseconds cleaner_sleeptime_ns;
            bool cleaner_thread_active = false;
            void cleanMessages();
            void doClean();

        public:
            explicit ChatHelper();
            ~ChatHelper();
            void start();
            void stop();
            void sendVanishingMessage(int64_t chat_id, const std::string &text, bool disableWebPagePreview = false, int32_t replyToMessageId = 0, TgBot::GenericReply::Ptr replyMarkup = std::make_shared<TgBot::GenericReply>(), const std::string &parseMode = "", bool disableNotification = false);
            void addMessageToCleaner(int64_t chat_id, int32_t message_id);
        };

    } // namespace modules
} // namespace lmb

#endif