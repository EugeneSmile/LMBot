#include "Keyboard.h"

namespace lmbot
{
    namespace bot
    {

        void Keyboard::createKeyboards()
        {
            createKeyboard("Main", buttons["Main"], {4});
            createInlineKeyboard("CB_Example", "cb_example", inline_buttons["CB_Example"], {1});
        }

        void Keyboard::createKeyboard(const std::string &name, const std::vector<std::string> &buttons, const std::vector<size_t> &structure)
        {
            TgBot::ReplyKeyboardMarkup::Ptr rkm(new TgBot::ReplyKeyboardMarkup);

            std::vector<TgBot::KeyboardButton::Ptr> keyboard_row;
            uint8_t struct_index = 0;
            uint8_t keys_in_row = structure.at(struct_index);
            for (uint8_t i = 0; i < buttons.size(); ++i)
            {
                TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
                button->text = buttons.at(i);
                keyboard_row.push_back(button);
                keys_in_row--;
                if (keys_in_row == 0)
                {
                    rkm->keyboard.push_back(keyboard_row);
                    keyboard_row.resize(0);
                    if (struct_index < structure.size() - 1)
                        struct_index++;
                    keys_in_row = structure.at(struct_index);
                }
            }
            Keyboard::keyboard[name] = rkm;
        }

        void Keyboard::createInlineKeyboard(const std::string &name, const std::string &command, const std::vector<std::string> &buttons, const std::vector<size_t> &structure)
        {
            TgBot::InlineKeyboardMarkup::Ptr ikm(new TgBot::InlineKeyboardMarkup);

            std::vector<TgBot::InlineKeyboardButton::Ptr> keyboard_row;
            uint8_t struct_index = 0;
            uint8_t keys_in_row = structure.at(struct_index);
            for (uint8_t i = 0; i < buttons.size(); ++i)
            {
                TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
                button->text = buttons.at(i);
                std::string callback_data;
                callback_data = command + " " + buttons.at(i);
                button->callbackData = callback_data;
                keyboard_row.push_back(button);
                keys_in_row--;
                if (keys_in_row == 0)
                {
                    ikm->inlineKeyboard.push_back(keyboard_row);
                    keyboard_row.resize(0);
                    if (struct_index < structure.size() - 1)
                        struct_index++;
                    keys_in_row = structure.at(struct_index);
                }
            }
            Keyboard::inline_keyboard[name] = ikm;
        }

    } // namespace bot
} // namespace lmbot