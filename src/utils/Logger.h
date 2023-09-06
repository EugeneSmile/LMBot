#ifndef LMB_LOGGER_H_GUARD
#define LMB_LOGGER_H_GUARD

#include <memory>

#include <spdlog/spdlog.h>

namespace spdlog
{
    class async_logger;
}

namespace lmb
{
    class Logger
    {
    private:
        std::shared_ptr<spdlog::async_logger> combined_logger;
        // spdlog::details::async_logger_ptr combined_logger;

    public:
        Logger();
        ~Logger();
        void flush();
    };

} // namespace lmb

#endif