#include "CallbackQuery.h"

#include "LMBot.h"
#include "CommandsList.h"

namespace lmb
{
    namespace managers
    {
        void CallbackQuery::process(const TgBot::CallbackQuery::Ptr query)
        {
            splitString(query->data, command, 0x20);
            if (vars::bot_callback_queries.find(command.at(0)) != vars::bot_callback_queries.end())
            {
                switch (vars::bot_callback_queries[command.at(0)])
                {
                case vars::CallbackQueries::CB_Example:
                    bot->tgbot->getApi().answerCallbackQuery(query->id, "Callback received!", true);
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
