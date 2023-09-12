#ifndef LMBOT_MODULES_ANIMATIONCONVERTER_ANIMATIONCONVERTER_H_GUARD
#define LMBOT_MODULES_ANIMATIONCONVERTER_ANIMATIONCONVERTER_H_GUARD

#include <fstream>
#include <chrono>
#include <ctime>

#include "Module.h"
#include "Utilities.h"

namespace lmbot
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
            const std::string doConversion(const std::string &video_file_id);

        public:
            AnimationConverter();
            ~AnimationConverter();
            void hello();
            void init(std::shared_ptr<LMBot> bot);
            void process(const std::vector<std::string> &command, TgBot::Message::Ptr message);
        };

    } // namespace modules
} // namespace lmbot

#endif