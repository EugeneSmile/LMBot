#include "CallbackQuery.h"

#include "LMBot.h"
#include "BaseCommandsList.h"

namespace lmbot
{
    namespace managers
    {
        void CallbackQuery::process(const TgBot::CallbackQuery::Ptr query)
        {
            splitString(query->data, command, 0x20);
            if (vars::base_callback_queries.find(command.at(0)) != vars::base_callback_queries.end())
            {
                switch (vars::base_callback_queries[command.at(0)])
                {
                case vars::BaseCallbackQueries::CB_Example:
                    bot->tgbot->getApi().answerCallbackQuery(query->id, "Callback received!", true);
                    break;

                default:
                    break;
                }
            }
            else
            {
                bot->logger->log()->info("Received unknown command: {}", command.at(0));
            }
        }

    } // namespace managers
} // namespace lmbot
