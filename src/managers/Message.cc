#include "Message.h"

#include <spdlog/spdlog.h>

#include "LMBot.h"
#include "Utilities.h"

namespace lmb
{
    namespace managers
    {
        void Message::handleText(const TgBot::Message::Ptr message)
        {
            spdlog::get("main")->info("Recieved text message: {}", message->text);
        }

        void Message::handlePicture(const TgBot::Message::Ptr message)
        {
            spdlog::get("main")->info("Recieved picture");
        }

        void Message::handleVideo(const TgBot::Message::Ptr message)
        {
            bot->modules->animation_converter->processMessage(message);
        }

        void Message::handleAudio(const TgBot::Message::Ptr message)
        {
            spdlog::get("main")->info("Recieved audio");
        }

        void Message::handleVoice(const TgBot::Message::Ptr message)
        {
            spdlog::get("main")->info("Recieved voice");
        }

        void Message::handleSticker(const TgBot::Message::Ptr message)
        {
            spdlog::get("main")->info("Recieved sticker");
        }

        void Message::handleAnimation(const TgBot::Message::Ptr message)
        {
            spdlog::get("main")->info("Recieved animation");
        }

        void Message::handleDocument(const TgBot::Message::Ptr message)
        {
            spdlog::get("main")->info("Recieved document");
        }

        void Message::process(const TgBot::Message::Ptr message)
        {
            if (message->text.length() > 0)
            {
                if (message->text[0] == 0x2F)
                    bot->managers->command->process(message);
                else
                    handleText(message);
            }
            if (message->photo.size() > 0)
                handlePicture(message);
            if (message->sticker)
                handleSticker(message);
            if (message->animation)
                handleAnimation(message);
            if (message->video)
                handleVideo(message);
            if (message->audio)
                handleAudio(message);
            if (message->voice)
                handleVoice(message);
            if (message->document && !(message->animation))
                handleDocument(message);
        }
    } // namespace managers
} // namespace lmb