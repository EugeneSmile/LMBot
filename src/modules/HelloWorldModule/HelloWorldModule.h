#ifndef LMBOT_MODULES_HELLOWORLDMODULE_HELLOWORLDMODULE_H_GUARD
#define LMBOT_MODULES_HELLOWORLDMODULE_HELLOWORLDMODULE_H_GUARD

#include "Module.h"

namespace lmbot
{
    namespace modules
    {
        class HelloWorldModule : public Module
        {
        private:
        public:
            void init(std::shared_ptr<LMBot> bot);
            void hello();
            HelloWorldModule();
            ~HelloWorldModule();
            void process(const std::vector<std::string> &command, TgBot::Message::Ptr message);
        };
    } // modules
} // lmbot

#endif // LMBOT_MODULES_HELLOWORLDMODULE_HELLOWORLDMODULE_H_GUARD