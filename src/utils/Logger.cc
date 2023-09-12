#include "Logger.h"

namespace lmbot
{
    Logger::Logger()
    {
        spdlog::init_thread_pool(8192, 1);
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/lmbot.log", 1048576 * 5, 10, false));
        registerLogger("Logger");
        registerLogger(LOGGER_MAIN_NAME);
        spdlog::flush_every(std::chrono::seconds(10));
        loggers["Logger"]->info("Initialized");
    }

    Logger::~Logger()
    {
        loggers["Logger"]->info("Stopping");
    }

    std::shared_ptr<spdlog::async_logger> Logger::registerLogger(const std::string &name)
    {
        if (loggers.find(name) == loggers.end())
        {
            auto logger = std::make_shared<spdlog::async_logger>(name, begin(sinks), end(sinks), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
            logger->set_pattern("[%d/%m/%Y %H:%M:%S.%e] [%^%l%$] [%n] %v");
            loggers[name] = logger;
            spdlog::register_logger(logger);

            loggers["Logger"]->info("Registred logger {}", name);
        }
        return loggers[name];
    }

    void Logger::flush(const std::string &name)
    {
        loggers[name]->flush();
    }

} // namespace lmbot
