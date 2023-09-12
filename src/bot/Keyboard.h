#ifndef LMBOT_BOT_KEYBOARD_H_GUARD
#define LMBOT_BOT_KEYBOARD_H_GUARD

#include <string>
#include <vector>
#include <unordered_map>

#include <tgbot/tgbot.h>

namespace lmbot
{
    namespace bot
    {

        class Keyboard
        {
        private:
            inline static std::unordered_map<std::string, TgBot::ReplyKeyboardMarkup::Ptr> keyboard;
            inline static std::unordered_map<std::string, TgBot::InlineKeyboardMarkup::Ptr> inline_keyboard;

            inline static std::unordered_map<std::string, std::vector<std::string>> buttons{{"Main", {"/status", "/version", "/callback", "/stop"}}};
            inline static std::unordered_map<std::string, std::vector<std::string>> inline_buttons{{"CB_Example", {"Callback Query Button"}}};

        public:
            static void createKeyboards();
            static TgBot::ReplyKeyboardMarkup::Ptr getKeyboard(const std::string &name) { return Keyboard::keyboard[name]; };
            static TgBot::InlineKeyboardMarkup::Ptr getInlineKeyboard(const std::string &name) { return Keyboard::inline_keyboard[name]; };
            static void createKeyboard(const std::string &name, const std::vector<std::string> &buttons, const std::vector<size_t> &structure);
            static void createInlineKeyboard(const std::string &name, const std::string &command, const std::vector<std::string> &buttons, const std::vector<size_t> &structure);
        };

    } // namespace bot
} // namespace lmbot

#endif