#ifndef LMB_VARS_COMMANDSLIST_H_GUARD
#define LMB_VARS_COMMANDSLIST_H_GUARD

#include <string>
#include <unordered_map>

namespace lmb
{
    namespace vars
    {
        enum CommandsList
        {
            M_Start = 0,
            M_Stop,
            M_Version,
            M_Main,
            M_Status,
            M_Callback
        };

        static std::unordered_map<std::string, CommandsList>
            bot_commands =
                {
                    {"start", M_Start},
                    {"stop", M_Stop},
                    {"version", M_Version},
                    {"status", M_Status},
                    {"callback", M_Callback}};

        enum CallbackQueries
        {
            CB_Example = 0,
        };

        static std::unordered_map<std::string, CallbackQueries>
            bot_callback_queries =
                {
                    {"cb_example", CB_Example}};

    } // vars
} // lmb

#endif // LMB_VARS_COMMANDLIST_H_GUARD
