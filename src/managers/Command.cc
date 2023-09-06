#include "Command.h"

#include "LMBot.h"
#include "CommandsList.h"

namespace lmb
{
    namespace managers
    {
        void Command::process(const TgBot::Message::Ptr message)
        {
            splitString(message->text, command, 0x20);
            command.at(0).erase(0, 1);
            toLower(command.at(0));
            if (vars::bot_commands.find(command.at(0)) != vars::bot_commands.end())
            {
                switch (vars::bot_commands[command.at(0)])
                {
                case vars::CommandsList::M_Start:
                    bot->tgbot->getApi().sendMessage(message->chat->id, "Main menu", false, 0, bot::Keyboard::getKeyboard("Main"));
                    break;
                case vars::CommandsList::M_Stop:
                    bot->modules->chat_helper->sendVanishingMessage(message->chat->id, "We are stopping in the safe way");
                    kill(getpid(), SIGQUIT);
                    break;
                case vars::CommandsList::M_Version:
                    bot->tgbot->getApi().sendMessage(message->chat->id, "Bot version: " + std::string(VERSION_FULL));
                    break;
                case vars::CommandsList::M_Status:
                    bot->tgbot->getApi().sendMessage(message->chat->id, "Online\n" + std::to_string(bot->managers->processor->getActiveThreads() - 1) + " of " + std::to_string(bot->managers->processor->getTotalThreads()) + " threads in work");
                    break;
                case vars::CommandsList::M_Callback:
                    bot->tgbot->getApi().sendMessage(message->chat->id, "Callback Example", false, 0, bot::Keyboard::getInlineKeyboard("CB_Example"));
                    break;
                default:
                    break;
                }
            }
            else
            {
                spdlog::get("main")->info("Received unknown command: {}", command.at(0));
            }
        }

    } // namespace managers
} // namespace lmb
