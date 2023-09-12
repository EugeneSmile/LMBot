#include "Modules.h"

#include "LMBot.h"

namespace lmbot
{
    namespace managers
    {
        void Modules::dlOpenModule(const std::filesystem::path &path, ModuleData &module_data)
        {
            if (!(module_data.handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LAZY | RTLD_LOCAL)))
            {
                logger->error(dlerror());
            }
        }

        void Modules::dlCloseModule(ModuleData &module_data)
        {
            logger->info("Unloading module {}", module_data.name);
            if (dlclose(module_data.handle) != 0)
            {
                logger->error(dlerror());
            }
        }

        std::shared_ptr<modules::Module> Modules::dlGetModuleInstance(ModuleData &module_data)
        {
            auto module_function_allocator = reinterpret_cast<modules::Module *(*)()>(dlsym(module_data.handle, "allocator"));
            auto module_function_deleter = reinterpret_cast<void (*)(modules::Module *)>(dlsym(module_data.handle, "deleter"));

            if (!module_function_allocator || !module_function_deleter)
            {
                dlCloseModule(module_data);
                logger->error(dlerror());
            }

            return std::shared_ptr<modules::Module>(module_function_allocator(), [module_function_deleter](modules::Module *ptr)
                                                    { module_function_deleter(ptr); });
        }

        Modules::Modules(/* args */)
        {
            logger = bot->logger->registerLogger("Modules");
            loadModules();
        }

        Modules::~Modules()
        {
        }

        void Modules::loadModules()
        {
            for (auto const &entry : std::filesystem::directory_iterator{modules_path})
                if (entry.is_regular_file() && entry.path().extension() == ".so")
                    loadModule(entry.path());
        }

        void Modules::loadModule(const std::filesystem::path &path)
        {
            logger->debug("Loading module from file {}", path.filename().string());
            ModuleData module_data;
            dlOpenModule(path, module_data);
            module_data.module = dlGetModuleInstance(module_data);
            module_data.name = module_data.module->getModuleName();
            if (module_data.module->isLoaded())
            {
                modules[module_data.name] = module_data;
                modules[module_data.name].module->init(bot);

                for (const std::string &command : modules[module_data.name].module->getModuleCommands())
                {
                    auto it = commands_map.find(command);
                    if (it == commands_map.end())
                    {
                        commands_map.insert({command, modules[module_data.name].module});
                    }
                    else
                    {
                        logger->warn("Duplicate command use: trying to add command '{}' from module '{}', but command already used by module '{}'", command, module_data.name, it->second->getModuleName());
                    }
                }

                logger->info("Loaded module {}", module_data.name);
            }
            else
            {
                logger->warn("Module {} didn't loaded properly", module_data.name);
                dlCloseModule(module_data);
            }
        }

        void Modules::reloadModules()
        {
            for (auto &module : modules)
            {
                module.second.module.reset();
                dlCloseModule(module.second);
            }
            modules.clear();
            commands_map.clear();
            loadModules();
        }

        bool Modules::modulesHaveCommand(const std::string &command)
        {
            return (commands_map.find(command) != commands_map.end());
        }

        void Modules::process(TgBot::Message::Ptr message, const std::vector<std::string> &command)
        {
            commands_map[command.at(0)]->process(message, command);
        }

        void Modules::process(TgBot::CallbackQuery::Ptr query, const std::vector<std::string> &command)
        {
            commands_map[command.at(0)]->process(query, command);
        }
    } // managers
} // lmbot