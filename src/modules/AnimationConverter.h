#ifndef LMB_MODULES_ANIMATIONCONVERTER_H_GUARD
#define LMB_MODULES_ANIMATIONCONVERTER_H_GUARD

#include <fstream>
#include <chrono>
#include <ctime>
#include <tgbot/tgbot.h>

#include "Module.h"
#include "Utilities.h"

namespace lmb
{
    namespace modules
    {

        class AnimationConverter : public Module
        {
        private:
            std::string video_path;
            std::string animation_path;
            std::string converter_command;
            const std::string video_extention = ".mp4";
            const std::string animation_extention = ".gif";
            bool in_process = false;

        public:
            explicit AnimationConverter();
            const std::string doConversion(const std::string &video_file_id);
            void processMessage(TgBot::Message::Ptr message);
        };

    } // namespace modules
} // namespace lmb

#endif