#ifndef LMBOT_VARS_BASECOMMANDSLIST_H_GUARD
#define LMBOT_VARS_BASECOMMANDSLIST_H_GUARD

#include <string>
#include <unordered_map>

namespace lmbot
{
    namespace vars
    {
        enum BaseCommandsList
        {
            M_Start = 0,
            M_Stop,
            M_Help,
            M_Version,
            M_Main,
            M_Status,
            M_Reload,
            M_Callback
        };

        static std::unordered_map<std::string, BaseCommandsList>
            base_commands =
                {
                    {"start", M_Start},
                    {"stop", M_Stop},
                    {"help", M_Help},
                    {"version", M_Version},
                    {"status", M_Status},
                    {"reload", M_Reload},
                    {"callback", M_Callback}};

        enum BaseCallbackQueries
        {
            CB_Example = 0,
        };

        static std::unordered_map<std::string, BaseCallbackQueries>
            base_callback_queries =
                {
                    {"cb_example", CB_Example}};

    } // vars
} // lmbot

#endif // LMBOT_VARS_COMMANDLIST_H_GUARD
