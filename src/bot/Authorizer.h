#ifndef LMB_BOT_AUTHORIZER_H_GUARD
#define LMB_BOT_AUTHORIZER_H_GUARD

#include <cstdint>
#include <vector>
#include <algorithm>

#include "Bot.h"

namespace lmb
{
    class LMBot;
    namespace bot
    {

        class Authorizer : public Bot
        {
        private:
            enum OperationMode
            {
                ADMIN_ONLY = 0,
                LIST = 1,
                ALL = 2,
            } operation_mode;

            enum ServiceReplyMode
            {
                ADMIN = 0,
                GROUP = 1
            } service_reply_mode;

            struct IDs
            {
                int64_t admin;
                int64_t service_group;
                std::vector<int64_t> authorized;
            } ids;

        public:
            explicit Authorizer();
            ~Authorizer();
            bool isAuthorized(int64_t id);
            int64_t getServiceId();
        };

    } // namespace bot
} // namespace lmb

#endif