#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/pattern_formatter.h>

namespace Circe {

    std::shared_ptr<spdlog::logger> Log::s_EngineLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init() {
        // Create color console sinks
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        
        // Set pattern: [timestamp] [logger name] [level] message
        spdlog::set_pattern("%^[%T] [%n] [%l]%$ %v");

        // Create engine logger
        s_EngineLogger = std::make_shared<spdlog::logger>("CIRCE", consoleSink);
        s_EngineLogger->set_level(spdlog::level::trace);
        spdlog::register_logger(s_EngineLogger);

        // Create client logger
        s_ClientLogger = std::make_shared<spdlog::logger>("APP", consoleSink);
        s_ClientLogger->set_level(spdlog::level::trace);
        spdlog::register_logger(s_ClientLogger);

        Info("Logging system initialized");
    }

    void Log::Shutdown() {
        Info("Shutting down logging system");
        spdlog::shutdown();
    }

}
