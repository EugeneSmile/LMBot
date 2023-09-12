#ifndef LMBOT_BOT_LMBOT_H_GUARD
#define LMBOT_BOT_LMBOT_H_GUARD

#include <string>
#include <memory>

#include <tgbot/tgbot.h>

#include <cxxopts.hpp>

// Base
#include "Bot.h"

// Managers
#include "Processor.h"
#include "Message.h"
#include "Command.h"
#include "CallbackQuery.h"
#include "InlineQuery.h"
#include "ChatHelper.h"
#include "Modules.h"

// Utils
#include "Authorizer.h"
#include "ConfigWrapper.h"
#include "Keyboard.h"
#include "Version.h"
#include "Logger.h"

// Vars
#include "BaseCommandsList.h"

namespace lmbot
{

    class LMBot : public std::enable_shared_from_this<LMBot>
    {
    private:
        LMBot() = default;

    public:
        struct TgData
        {
            std::string token = "";
            std::string webhook_url = "";
            uint32_t port = 7771;
        };

        struct Managers
        {
            std::shared_ptr<managers::Processor> processor;
            std::shared_ptr<managers::Message> message;
            std::shared_ptr<managers::Command> command;
            std::shared_ptr<managers::CallbackQuery> callback_query;
            std::shared_ptr<managers::InlineQuery> inline_query;
            std::shared_ptr<managers::ChatHelper> chat_helper;
            std::shared_ptr<managers::Modules> modules;
        };

        /*         struct Modules
                {
                    std::shared_ptr<modules::AnimationConverter> animation_converter;
                }; */

        bool enabled = true;
        std::shared_ptr<cxxopts::ParseResult> arguments;
        std::shared_ptr<TgBot::Bot> tgbot;
        std::shared_ptr<bot::Authorizer> authorizer;
        std::shared_ptr<ConfigWrapper> config;
        std::shared_ptr<Logger> logger;

        std::shared_ptr<TgData> data;
        std::shared_ptr<Managers> managers;
        //        std::shared_ptr<Modules> modules;

        std::shared_ptr<LMBot> getPointer();
        [[nodiscard]] static std::shared_ptr<LMBot> create(std::shared_ptr<cxxopts::ParseResult> args);
        void init();
        static void deleter(LMBot *bot);
        void stop();
    };

} // namespace lmbot
#endif