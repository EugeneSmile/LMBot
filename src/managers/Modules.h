#ifndef LMBOT_MANAGERS_MODULES_H_GUARD
#define LMBOT_MANAGERS_MODULES_H_GUARD

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#include "Bot.h"
#include "Module.h"

namespace lmbot
{
    namespace managers
    {
        class Modules : public bot::Bot
        {
        private:
            struct ModuleData
            {
                std::string name;
                void *handle;
                std::shared_ptr<modules::Module> module;
            };

            void dlOpenModule(const std::filesystem::path &path, ModuleData &module_data);
            void dlCloseModule(ModuleData &module_data);
            std::shared_ptr<modules::Module> dlGetModuleInstance(ModuleData &module_data);
            void loadModule(const std::filesystem::path &path);
            void loadModules();
            std::unordered_map<std::string, ModuleData> modules;
            const std::filesystem::path modules_path{"Modules"};
            std::unordered_map<std::string, std::shared_ptr<modules::Module>> commands_map;

        public:
            Modules(/* args */);
            ~Modules();
            void reloadModules();
            std::shared_ptr<modules::Module> &operator[](const std::string &name) { return modules[name].module; };
            bool modulesHaveCommand(const std::string &command);
            void process(TgBot::Message::Ptr message, const std::vector<std::string> &command);
            void process(TgBot::CallbackQuery::Ptr query, const std::vector<std::string> &command);
        };

    } // managers
} // lmbot

#endif // LMBOT_MANAGERS_MODULES_H_GUARD