#ifndef LMBOT_LOGGER_H_GUARD
#define LMBOT_LOGGER_H_GUARD

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace spdlog
{
    class async_logger;
}

namespace lmbot
{
    constexpr std::string LOGGER_MAIN_NAME = "LMBot";
    class Logger
    {
    private:
        std::vector<spdlog::sink_ptr> sinks;
        std::unordered_map<std::string, std::shared_ptr<spdlog::async_logger>> loggers;
        // std::shared_ptr<spdlog::async_logger> combined_logger;
        // spdlog::details::async_logger_ptr combined_logger;

    public:
        Logger();
        ~Logger();
        std::shared_ptr<spdlog::async_logger> registerLogger(const std::string &name);
        void flush(const std::string &name = LOGGER_MAIN_NAME);
        std::shared_ptr<spdlog::async_logger> log() { return loggers[LOGGER_MAIN_NAME]; };
    };

} // namespace lmbot

#endif