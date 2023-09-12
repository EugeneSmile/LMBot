#include "HelloWorldModule.h"

#include <iostream>

#include "LMBot.h"

namespace lmbot
{
    namespace modules
    {
        extern "C"
        {
            Module *allocator()
            {
                return new HelloWorldModule();
            }

            void deleter(Module *ptr)
            {
                delete ptr;
            }
        }

        HelloWorldModule::HelloWorldModule() : Module("HelloWorldModule")
        {
            module_commands = {"hello1", "hello2"};
            loaded = true;
        }

        HelloWorldModule::~HelloWorldModule()
        {
            loaded = false;
        }

        void HelloWorldModule::init(std::shared_ptr<LMBot> bot)
        {
            this->bot = bot;
            logger = bot->logger->registerLogger(module_name);
            hello();
        }

        void HelloWorldModule::hello()
        {
            logger->info("Hello from {} Module after THIRD try!", module_name);
        }

        void HelloWorldModule::process(const std::vector<std::string> &command, TgBot::Message::Ptr message)
        {
            if (command.at(0) == "hello1")
                hello();
        }

    } // modules
} // lmbot
