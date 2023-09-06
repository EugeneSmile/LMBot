#include "Logger.h"

#include <vector>

#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace lmb
{
    Logger::Logger()
    {
        spdlog::init_thread_pool(8192, 1);
        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/lmbot.log", 1048576 * 5, 10, false));
        combined_logger = std::make_shared<spdlog::async_logger>("main", begin(sinks), end(sinks), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        spdlog::register_logger(combined_logger);

        spdlog::flush_every(std::chrono::seconds(10));

        combined_logger->info("LMBot start");
        combined_logger->info("Logger: Initialized");
    }

    Logger::~Logger()
    {
        combined_logger->info("Logger: Stopping");
    }

    void Logger::flush()
    {
        combined_logger->flush();
    }

} // namespace lmb
