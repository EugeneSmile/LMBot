#include "AnimationConverter.h"

#include "LMBot.h"

namespace lmbot
{
    namespace modules
    {
        extern "C"
        {
            Module *allocator()
            {
                return new AnimationConverter();
            }

            void deleter(Module *ptr)
            {
                delete ptr;
            }
        }

        AnimationConverter::AnimationConverter() : Module("AnimationConverter")
        {
            module_commands = {"hello2"};
            loaded = true;
        }

        AnimationConverter::~AnimationConverter()
        {
            loaded = false;
        }

        void AnimationConverter::hello()
        {
            logger->info("Hello from {} Module!", module_name);
        }

        void AnimationConverter::init(std::shared_ptr<LMBot> bot)
        {
            this->bot = bot;
            logger = bot->logger->registerLogger(module_name);

            video_path = bot->config->getParameter("animation_converter.video_path", "AnimationConverter/Videos");
            animation_path = bot->config->getParameter("animation_converter.animation_path", "AnimationConverter/Gifs");
            converter_command = bot->config->getParameter("animation_converter.converter_command", "AnimationConverter/mp42gif.sh");
        }

        void AnimationConverter::process(const std::vector<std::string> &command, TgBot::Message::Ptr message)
        {
            bot->tgbot->getApi().sendMessage(message->chat->id, "Received video, do conversion to gif");
            std::string animation_file_path = doConversion(message->video->fileId);
            TgBot::InputFile::Ptr animation_file = TgBot::InputFile::fromFile(animation_file_path, "image/gif");
            bot->tgbot->getApi().sendAnimation(message->chat->id, animation_file);
        }

        const std::string AnimationConverter::doConversion(const std::string &video_file_id)
        {
            bool video_file_downloaded = false;
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d-%H%M%S");
            const std::string filename_time = ss.str() + "_" + video_file_id;
            const std::string video_file_path = video_path + "/" + filename_time + video_extention;
            const std::string animation_file_path = animation_path + filename_time + animation_extention;
            std::fstream video_file;
            video_file.open(video_file_path, std::fstream::out);
            if (video_file.is_open())
            {
                video_file << downloadFile(bot->tgbot, video_file_id);
                video_file_downloaded = true;
                video_file.close();
            }
            if (video_file_downloaded)
            {
                const std::string full_command = converter_command + " " + video_file_path + " " + animation_file_path;
                system(full_command.c_str());
            }
            return animation_file_path;
        }

    } // namespace modules
} // namespace lmbot