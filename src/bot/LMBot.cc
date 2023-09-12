#include "LMBot.h"

#include <spdlog/spdlog.h>

namespace lmbot
{
    void LMBot::init()
    {
        bot::Bot::init(shared_from_this());

        logger = std::make_unique<Logger>();
        config = std::make_shared<lmbot::ConfigWrapper>((*arguments)["config_file"].as<std::string>());
        spdlog::set_level(static_cast<spdlog::level::level_enum>(config->getParameter("general.loglevel", 2)));
        authorizer = std::make_shared<bot::Authorizer>();
        data = std::make_shared<TgData>();

        logger->log()->info("LMBot version: {}", VERSION_FULL);
        data->token = config->getParameter("bot.token", "");
        data->webhook_url = config->getParameter("bot.webhook_url", "");
        data->webhook_url += data->token;
        data->port = config->getParameter("bot.port", 7770);
        enabled = config->getParameter("bot.enabled", true);

        logger->log()->info("Token: {}", data->token);
        logger->log()->info("Desired webhook url: {}", data->webhook_url);
        logger->log()->info("Port: {}", data->port);

        tgbot = std::make_shared<TgBot::Bot>(data->token);

        logger->log()->info("LMBot: Initializing managers");
        managers = std::make_shared<Managers>();
        managers->processor = std::make_shared<managers::Processor>();
        managers->message = std::make_shared<managers::Message>();
        managers->command = std::make_shared<managers::Command>();
        managers->callback_query = std::make_shared<managers::CallbackQuery>();
        managers->inline_query = std::make_shared<managers::InlineQuery>();
        managers->chat_helper = std::make_shared<managers::ChatHelper>();
        managers->modules = std::make_shared<managers::Modules>();
        logger->log()->info("LMBot: Initializing managers done!");

        /*         logger->log()->info("LMBot: Initializing modules");
                modules = std::make_shared<Modules>();
                modules->animation_converter = std::make_shared<modules::AnimationConverter>();
                logger->log()->info("LMBot: Initializing modules done!");
         */
        bot::Keyboard::createKeyboards();

        enabled &= (managers->command && managers->message && managers->processor && managers->callback_query && managers->inline_query && authorizer && config && tgbot);
        if (!enabled)
            logger->log()->error("LMBot: Initialization failed or bot disabled in config!");
    }

    void LMBot::stop()
    {
        /*         logger->log()->info("LMBot: Stopping modules");
                modules->animation_converter.reset();
                modules.reset();
                logger->log()->info("LMBot: Stopping modules done!"); */

        logger->log()->info("LMBot: Stopping managers");
        managers->modules.reset();
        managers->chat_helper.reset();
        managers->inline_query.reset();
        managers->callback_query.reset();
        managers->command.reset();
        managers->message.reset();
        managers->processor.reset();
        managers.reset();
        logger->log()->info("LMBot: Stopping managers done!");
    }

    void LMBot::deleter(LMBot *bot)
    {
        // Were used during debug, left in case if delete procedures are required
    }

    std::shared_ptr<LMBot> LMBot::getPointer()
    {
        return shared_from_this();
    }

    std::shared_ptr<LMBot> LMBot::create(std::shared_ptr<cxxopts::ParseResult> args)
    {
        std::shared_ptr<LMBot> bot = std::shared_ptr<LMBot>(new LMBot(), deleter);
        bot->arguments = args;
        bot->init();
        return bot;
    }

} // namespace lmbot