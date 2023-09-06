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

std::unique_ptr<TgBot::TgWebhookTcpServer> webhookServer;
bool bot_thread_active = false;
std::thread botThread;

void interruptionSignal(int signal)
{
    if (signal == SIGQUIT)
    {
        spdlog::get("main")->info("We received interruption signal, stopping!");
        webhookServer->stop();
        spdlog::get("main")->info("Waiting for bot thread to stop");
        if (botThread.joinable())
            botThread.join();
        spdlog::get("main")->info("Finished waiting for bot thread");
    }
    else
    {
        spdlog::get("main")->error("Unexpected signal {} received", signal);
    }
}

void botServerThread(std::mutex &mutex, bool &thread_active)
{
    spdlog::get("main")->info("Server is starting in separate thread");
    mutex.lock();
    thread_active = true;
    mutex.unlock();
    webhookServer->start();
    mutex.lock();
    thread_active = false;
    mutex.unlock();
    spdlog::get("main")->info("Server has stopped in separate thread");
}

int main(int argc, char **argv)
{
    cxxopts::Options options("lmbot", "Telegram Bot");

    options.add_options()("c,config,configfile,config_file", "Configuration file", cxxopts::value<std::string>()->default_value("lmbot.toml"))("d,db,dbfile,database_file", "Database file", cxxopts::value<std::string>()->default_value("lmbot.db"));

    std::shared_ptr<cxxopts::ParseResult> args = std::make_shared<cxxopts::ParseResult>(options.parse(argc, argv));
    std::shared_ptr<lmb::LMBot> bot = lmb::LMBot::create(args);
    std::string args_oneline = args->arguments_string();
    std::replace(args_oneline.begin(), args_oneline.end(), '\n', ' ');
    spdlog::get("main")->info("Starting lmbot with options: {}", args_oneline);

    std::signal(SIGINT, interruptionSignal);
    std::signal(SIGQUIT, interruptionSignal);

    if (bot)
    {
        if (bot->enabled)
        {
            try
            {
                spdlog::get("main")->info("Bot username: {}", bot->tgbot->getApi().getMe()->username);
                bot->tgbot->getApi().setWebhook(bot->data->webhook_url);

                spdlog::get("main")->info("Bot last webhook error: {}", bot->tgbot->getApi().getWebhookInfo()->lastErrorMessage);
                spdlog::get("main")->info("Bot waiting events: {}", bot->tgbot->getApi().getWebhookInfo()->pendingUpdateCount);
                spdlog::get("main")->info("Bot webhook url: {}", bot->tgbot->getApi().getWebhookInfo()->url);

                webhookServer = std::make_unique<TgBot::TgWebhookTcpServer>(bot->data->port, *bot->tgbot);
                std::mutex mutex;
                botThread = std::thread(&botServerThread, std::ref(mutex), std::ref(bot_thread_active));

                while (!bot_thread_active)
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));

                bot->modules->chat_helper->start();

                while (bot_thread_active)
                {
                    if (!webhookServer)
                    {
                        spdlog::get("main")->error("Bot crashed!");
                        bot_thread_active = false;
                    }
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                }

                bot->modules->chat_helper->stop();
            }
            catch (std::exception &e)
            {
                spdlog::get("main")->error("General error: {}", e.what());
            }
        }
        bot->stop();
    }
    else
    {
        spdlog::get("main")->error("Unable to create bot instance");
    }
    spdlog::get("main")->info("We are stopping in the safe way");
    spdlog::get("main")->info("Deleting webhook");
    bot->tgbot->getApi().deleteWebhook();
    spdlog::get("main")->info("Done! Bye-bye");
    bot->logger->flush();
    return 0;
}
