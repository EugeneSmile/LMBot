#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <memory>

#include <tgbot/tgbot.h>

#include <cxxopts.hpp>

#include "LMBot.h"
#include "Keyboard.h"
#include "Utilities.h"
#include "Logger.h"

std::shared_ptr<lmbot::LMBot> bot;
std::unique_ptr<TgBot::TgWebhookTcpServer> webhookServer;
bool bot_thread_active = false;
std::thread botThread;

void interruptionSignal(int signal)
{
    if (signal == SIGQUIT)
    {
        bot->logger->log()->info("We received interruption signal, stopping!");
        webhookServer->stop();
        bot->logger->log()->info("Waiting for bot thread to stop");
        if (botThread.joinable())
            botThread.join();
        bot->logger->log()->info("Finished waiting for bot thread");
    }
    else
    {
        bot->logger->log()->error("Unexpected signal {} received", signal);
    }
}

void botServerThread(std::mutex &mutex, bool &thread_active)
{
    bot->logger->log()->info("Server is starting in separate thread");
    mutex.lock();
    thread_active = true;
    mutex.unlock();
    webhookServer->start();
    mutex.lock();
    thread_active = false;
    mutex.unlock();
    bot->logger->log()->info("Server has stopped in separate thread");
}

int main(int argc, char **argv)
{
    cxxopts::Options options("lmbot", "Telegram Bot");

    options.add_options()("c,config,configfile,config_file", "Configuration file", cxxopts::value<std::string>()->default_value("lmbot.toml"))("d,db,dbfile,database_file", "Database file", cxxopts::value<std::string>()->default_value("lmbot.db"));

    std::shared_ptr<cxxopts::ParseResult> args = std::make_shared<cxxopts::ParseResult>(options.parse(argc, argv));
    bot = lmbot::LMBot::create(args);
    std::string args_oneline = args->arguments_string();
    std::replace(args_oneline.begin(), args_oneline.end(), '\n', ' ');
    bot->logger->log()->info("Starting lmbot with options: {}", args_oneline);

    std::signal(SIGINT, interruptionSignal);
    std::signal(SIGQUIT, interruptionSignal);

    if (bot)
    {
        if (bot->enabled)
        {
            try
            {
                bot->logger->log()->info("Bot username: {}", bot->tgbot->getApi().getMe()->username);
                bot->tgbot->getApi().setWebhook(bot->data->webhook_url);

                bot->logger->log()->info("Bot last webhook error: {}", bot->tgbot->getApi().getWebhookInfo()->lastErrorMessage);
                bot->logger->log()->info("Bot waiting events: {}", bot->tgbot->getApi().getWebhookInfo()->pendingUpdateCount);
                bot->logger->log()->info("Bot webhook url: {}", bot->tgbot->getApi().getWebhookInfo()->url);

                webhookServer = std::make_unique<TgBot::TgWebhookTcpServer>(bot->data->port, *bot->tgbot);
                std::mutex mutex;
                botThread = std::thread(&botServerThread, std::ref(mutex), std::ref(bot_thread_active));

                while (!bot_thread_active)
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));

                bot->managers->chat_helper->start();

                while (bot_thread_active)
                {
                    if (!webhookServer)
                    {
                        bot->logger->log()->error("Bot crashed!");
                        bot_thread_active = false;
                    }
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                }

                bot->managers->chat_helper->stop();
            }
            catch (std::exception &e)
            {
                bot->logger->log()->error("General error: {}", e.what());
            }
        }
        bot->stop();
    }
    else
    {
        bot->logger->log()->error("Unable to create bot instance");
    }
    bot->logger->log()->info("We are stopping in the safe way");
    bot->logger->log()->info("Deleting webhook");
    bot->tgbot->getApi().deleteWebhook();
    bot->logger->log()->info("Done! Bye-bye");
    bot->logger->flush();
    return 0;
}
