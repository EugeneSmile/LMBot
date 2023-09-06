#include "ChatHelper.h"

#include <spdlog/spdlog.h>

#include "LMBot.h"

namespace lmb
{
    namespace modules
    {
        ChatHelper::ChatHelper()
        {
            database_handler = std::make_shared<DataBaseHelper>((*bot->arguments)["database_file"].as<std::string>());
            cleaner_sleeptime = bot->config->getParameter("chat_helper.cleaner_sleeptime", 30);
            cleaner_sleeptime_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(cleaner_sleeptime));
        }

        ChatHelper::~ChatHelper()
        {
        }

        void ChatHelper::start()
        {
            if (!cleaner_thread_active)
            {
                spdlog::get("main")->info("Starting chat cleaning thread");
                cleaner_thread_active = true;
                cleaner_thread = std::thread(&ChatHelper::cleanMessages, this);
            }
        }

        void ChatHelper::stop()
        {
            spdlog::get("main")->info("Stopping chat cleaning thread...");
            cleaner_thread_active = false;
            if (cleaner_thread.joinable())
                cleaner_thread.join();
            spdlog::get("main")->info("Stopping chat cleaning thread...done");
        }

        void ChatHelper::cleanMessages()
        {
            std::chrono::duration<int64_t, std::nano> timeout{std::chrono::duration<int64_t, std::nano>::zero()};
            std::chrono::high_resolution_clock::time_point timepoint_start = std::chrono::high_resolution_clock::now();
            while (cleaner_thread_active)
            {
                while (timeout < cleaner_sleeptime_ns)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    if (!cleaner_thread_active)
                    {
                        spdlog::get("main")->info("Got break to cleaner, stopping");
                        break;
                    }
                    timeout = std::chrono::high_resolution_clock::now() - timepoint_start;
                }
                doClean();
                timepoint_start = std::chrono::high_resolution_clock::now();
                timeout = std::chrono::duration<int64_t, std::nano>::zero();
            }
        }

        void ChatHelper::doClean()
        {
            database_handler->getOutdated(cleaner_sleeptime_ns.count(), messages_to_delete);
            for (std::pair<int64_t, int32_t> &message : messages_to_delete)
            {
                spdlog::get("main")->info("Cleaning message from chat {} with id {}", message.first, message.second);
                try
                {
                    bot->tgbot->getApi().deleteMessage(message.first, message.second);
                }
                catch (TgBot::TgException &e)
                {
                    spdlog::get("main")->error("Cleaner error: {}", e.what());
                }
            }
            database_handler->deleteMessages(messages_to_delete);
        }

        void ChatHelper::sendVanishingMessage(int64_t chat_id, const std::string &text, bool disableWebPagePreview, int32_t replyToMessageId, TgBot::GenericReply::Ptr replyMarkup, const std::string &parseMode, bool disableNotification)
        {
            TgBot::Message::Ptr message = bot->tgbot->getApi().sendMessage(chat_id, text, disableWebPagePreview, replyToMessageId, replyMarkup, parseMode, disableNotification);
            database_handler->addMessage(chat_id, message->messageId);
        }

        void ChatHelper::addMessageToCleaner(int64_t chat_id, int32_t message_id)
        {
            database_handler->addMessage(chat_id, message_id);
        }

    } // namespace modules

} // namespace lmb
