#ifndef LMBOT_MANAGERS_MESSAGE_H_GUARD
#define LMBOT_MANAGERS_MESSAGE_H_GUARD

#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <vector>

#include <tgbot/tgbot.h>

#include "Manager.h"

namespace lmbot
{
    class LMBot;
    namespace managers
    {

        class Message : public Manager
        {
        public:
            void process(const TgBot::Message::Ptr message);
            void handleText(const TgBot::Message::Ptr message);
            void handlePicture(const TgBot::Message::Ptr message);
            void handleVideo(const TgBot::Message::Ptr message);
            void handleAudio(const TgBot::Message::Ptr message);
            void handleVoice(const TgBot::Message::Ptr message);
            void handleSticker(const TgBot::Message::Ptr message);
            void handleAnimation(const TgBot::Message::Ptr message);
            void handleDocument(const TgBot::Message::Ptr message);
        };

    } // namespace managers
} // namespace lmbot

#endif
