#include "Command.h"

#include "LMBot.h"
#include "BaseCommandsList.h"

namespace lmbot
{
    namespace managers
    {
        void Command::process(const TgBot::Message::Ptr message)
        {
            splitString(message->text, command, 0x20);
            command.at(0).erase(0, 1);
            toLower(command.at(0));
            if (vars::base_commands.find(command.at(0)) != vars::base_commands.end())
            {
                switch (vars::base_commands[command.at(0)])
                {
                case vars::BaseCommandsList::M_Start:
                    bot->tgbot->getApi().sendMessage(message->chat->id, "Main menu", false, 0, bot::Keyboard::getKeyboard("Main"));
                    break;
                case vars::BaseCommandsList::M_Stop:
                    bot->managers->chat_helper->sendVanishingMessage(message->chat->id, "We are stopping in the safe way");
                    kill(getpid(), SIGQUIT);
                    break;
                case vars::BaseCommandsList::M_Version:
                    bot->tgbot->getApi().sendMessage(message->chat->id, "Bot version: " + std::string(VERSION_FULL));
                    break;
                case vars::BaseCommandsList::M_Status:
                    bot->tgbot->getApi().sendMessage(message->chat->id, "Online\n" + std::to_string(bot->managers->processor->getActiveThreads() - 1) + " of " + std::to_string(bot->managers->processor->getTotalThreads()) + " threads in work");
                    break;
                case vars::BaseCommandsList::M_Reload:
                    bot->managers->modules->reloadModules();
                    break;
                case vars::BaseCommandsList::M_Callback:
                    bot->tgbot->getApi().sendMessage(message->chat->id, "Callback Example", false, 0, bot::Keyboard::getInlineKeyboard("CB_Example"));
                    break;
                default:
                    break;
                }
            }
            else
            {
                if (bot->managers->modules->modulesHaveCommand(command.at(0)))
                    bot->managers->modules->process(message, command);
                else
                    bot->logger->log()->info("Received unknown command: {}", command.at(0));
            }
        }

    } // namespace managers
} // namespace lmbot
