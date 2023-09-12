#ifndef LMBOT_MODULES_MODULE_H_GUARD
#define LMBOT_MODULES_MODULE_H_GUARD

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>

#include <tgbot/tgbot.h>
#include <spdlog/spdlog.h>
#include <spdlog/async_logger.h>

namespace lmbot
{
    class LMBot;
    namespace modules
    {
        class Module
        {
        protected:
            Module(const std::string &name) : module_name(name){};
            std::string module_name;
            std::unordered_set<std::string> module_commands;
            std::unordered_set<std::string> callback_querries;
            std::shared_ptr<LMBot> bot;
            std::shared_ptr<spdlog::async_logger> logger;
            bool loaded;

        public:
            virtual ~Module() = default;
            virtual void hello() = 0;
            virtual void init(std::shared_ptr<LMBot> bot);
            virtual void process(TgBot::Message::Ptr message, const std::vector<std::string> &command = {});
            virtual void process(TgBot::CallbackQuery::Ptr query, const std::vector<std::string> &command = {});
            virtual void process(TgBot::InlineQuery::Ptr query, const std::vector<std::string> &command = {});
            const std::string &getModuleName() { return module_name; };
            const std::unordered_set<std::string> &getModuleCommands() { return module_commands; };
            const std::unordered_set<std::string> &getModuleCallbackQuerries() { return callback_querries; };
            bool isLoaded() { return loaded; };
        };

    } // namespace modules
} // namespace lmbot

#endif