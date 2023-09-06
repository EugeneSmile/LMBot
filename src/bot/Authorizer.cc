#include "Authorizer.h"

#include <spdlog/spdlog.h>

#include "LMBot.h"

namespace lmb
{
    namespace bot
    {
        Authorizer::Authorizer()
        {
            spdlog::get("main")->info("Authorizer: Initializing");
            operation_mode = static_cast<OperationMode>(bot->config->getParameter("auth.mode", 0));
            spdlog::get("main")->info("Authorizer: Operation mode: {}", static_cast<int64_t>(operation_mode));

            ids.admin = bot->config->getParameter("auth.admin", 0);
            spdlog::get("main")->info("Authorizer: Admin id: {}", ids.admin);

            ids.service_group = bot->config->getParameter("auth.service", 0);
            spdlog::get("main")->info("Authorizer: Service group id: {}", ids.service_group);

            ids.authorized = bot->config->getParameter<std::vector<int64_t>>("authorization.ids", {});

            spdlog::get("main")->info("Authorizer: Initialization done");
        }

        Authorizer::~Authorizer()
        {
        }

        bool Authorizer::isAuthorized(int64_t id)
        {
            switch (operation_mode)
            {
            case OperationMode::ALL:
                return true;
                break;

            case OperationMode::LIST:
                return (std::find(ids.authorized.begin(), ids.authorized.end(), id) != ids.authorized.end());
                break;

            default:
            case OperationMode::ADMIN_ONLY:
                return (id == ids.admin);
                break;
            }
        }

        int64_t Authorizer::getServiceId()
        {
            switch (service_reply_mode)
            {
            case ServiceReplyMode::GROUP:
                return ids.service_group;
                break;

            default:
            case ServiceReplyMode::ADMIN:
                return ids.admin;
                break;
            }
        }

    } // namespace bot
} // namespace lmb
