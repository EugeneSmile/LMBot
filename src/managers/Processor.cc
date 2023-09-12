#include "Processor.h"

#include <spdlog/spdlog.h>

#include "LMBot.h"

namespace lmbot
{
    namespace managers
    {
        Processor::Processor()
        {
            logger = bot->logger->registerLogger("Processor");
            logger->info("Initializing");
            threads_limit = bot->config->getParameter("general.threads_limit", 16);
            thread_wait_limit = bot->config->getParameter("general.thread_wait_limit", 30);

            logger->info("Prepare threads");
            threads.resize(threads_limit);

            logger->info("Setting message handlers");
            bot->tgbot->getEvents().onAnyMessage(std::bind(&Processor::processMessage, this, std::placeholders::_1));
            bot->tgbot->getEvents().onCallbackQuery(std::bind(&Processor::processCallbackQuery, this, std::placeholders::_1));
            bot->tgbot->getEvents().onInlineQuery(std::bind(&Processor::processInlineQuery, this, std::placeholders::_1));
            logger->info("Initialization done");
        }

        void Processor::processMessage(const TgBot::Message::Ptr message)
        {
            std::vector<ThreadData>::iterator it;
            bool thread_found = findFreeThread(threads, it);
            if (thread_found)
            {
                it->message = message;
                it->thread = std::thread(&Processor::processMessageThread, this, it->message);
                it->id = it->thread.get_id();
                it->thread.detach();
            }
        }

        void Processor::processCallbackQuery(const TgBot::CallbackQuery::Ptr query)
        {
            std::vector<ThreadData>::iterator it;
            bool thread_found = findFreeThread(threads, it);
            if (thread_found)
            {
                it->callback_query = query;
                it->thread = std::thread(&Processor::processCallbackQueryThread, this, it->callback_query);
                it->id = it->thread.get_id();
                it->thread.detach();
            }
        }

        void Processor::processInlineQuery(const TgBot::InlineQuery::Ptr query)
        {
            std::vector<ThreadData>::iterator it;
            bool thread_found = findFreeThread(threads, it);
            if (thread_found)
            {
                it->inline_query = query;
                it->thread = std::thread(&Processor::processInlineQueryThread, this, it->inline_query);
                it->id = it->thread.get_id();
                it->thread.detach();
            }
        }

        bool Processor::findFreeThread(std::vector<ThreadData> &threads, std::vector<ThreadData>::iterator &iterator)
        {
            std::vector<ThreadData>::iterator thread_it;
            int32_t tries = 0;
            do
            {
                if (tries == 1)
                    logger->warn("All threads busy, waiting for free thread");
                if (tries > thread_wait_limit)
                {
                    logger->warn("Unable to find free thread after time limit, stop procedure");
                    return false;
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
                thread_it = std::find_if(threads.begin(), threads.end(), [&](const ThreadData &data)
                                         { return data.active == false; });
                tries++;
            } while (thread_it == threads.end());
            thread_it->active = true;
            iterator = thread_it;
            return true;
        }

        void Processor::processMessageThread(const TgBot::Message::Ptr message)
        {

            if (bot->authorizer->isAuthorized(message->from->id))
            {
                try
                {
                    bot->managers->message->process(message);
                }
                catch (const std::exception &e)
                {
                    logger->error("Process manager message exception: {}", e.what());
                }
            }
            else
            {
                logger->warn("Got message from unknown user with id: {}, first name: {} last name: {}, username: {}, user is {}a bot", message->from->id, message->from->firstName, message->from->lastName, message->from->username, (message->from->isBot ? "" : "not "));
            }
            std::find_if(threads.begin(), threads.end(), [&](const ThreadData &data)
                         { return data.id == std::this_thread::get_id(); })
                ->active = false;
        }

        void Processor::processCallbackQueryThread(const TgBot::CallbackQuery::Ptr query)
        {
            if (bot->authorizer->isAuthorized(query->from->id))
            {
                try
                {
                    bot->managers->callback_query->process(query);
                }
                catch (const std::exception &e)
                {
                    logger->error("Process manager callback query exception: {}", e.what());
                }
            }
            else
            {
                bot->tgbot->getApi().sendMessage(bot->authorizer->getServiceId(), "Got callback query from unknown user:\nID: " + std::to_string(query->from->id) + "\nFirst name: " + query->from->firstName + "\nLast name: " + query->from->lastName + "\nUsername: " + query->from->username + "\nBot: " + std::to_string(query->from->isBot));
            }
            std::find_if(threads.begin(), threads.end(), [&](const ThreadData &data)
                         { return data.id == std::this_thread::get_id(); })
                ->active = false;
        }

        void Processor::processInlineQueryThread(const TgBot::InlineQuery::Ptr query)
        {
            if (bot->authorizer->isAuthorized(query->from->id))
            {
                try
                {
                    bot->managers->inline_query->process(query);
                }
                catch (const std::exception &e)
                {
                    logger->error("Process manager inline query exception: {}", e.what());
                }
            }
            else
            {
                bot->tgbot->getApi().sendMessage(bot->authorizer->getServiceId(), "Got inline query from unknown user:\nID: " + std::to_string(query->from->id) + "\nFirst name: " + query->from->firstName + "\nLast name: " + query->from->lastName + "\nUsername: " + query->from->username + "\nBot: " + std::to_string(query->from->isBot));
            }
            std::find_if(threads.begin(), threads.end(), [&](const ThreadData &data)
                         { return data.id == std::this_thread::get_id(); })
                ->active = false;
        }

        uint32_t Processor::getActiveThreads()
        {
            uint32_t threads_in_work = 0;
            for (const auto &thread : threads)
                threads_in_work += thread.active;
            return threads_in_work;
        }

        uint32_t Processor::getTotalThreads()
        {
            return threads_limit;
        }

    } // namespace managers
} // namespace lmbot
