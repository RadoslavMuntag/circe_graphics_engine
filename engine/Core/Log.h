#pragma once

#include "Export.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace Circe {

    /**
     * @brief Centralized logging system using spdlog\
     *  
     * Provides colored console output with different log levels.
     * Usage:
     *   Log::Info("Engine initialized");
     *   Log::Error("Failed to load texture: {}", path);
     */
    class CIRCE_API Log {
    public:
        static void Init();
        static void Shutdown();

        // Core engine logger
        static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }
        
        // Client application logger
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

        // Convenience functions for engine logs
        template<typename... Args>
        static void Trace(Args&&... args) { s_EngineLogger->trace(std::forward<Args>(args)...); }

        template<typename... Args>
        static void Debug(Args&&... args) { s_EngineLogger->debug(std::forward<Args>(args)...); }

        template<typename... Args>
        static void Info(Args&&... args) { s_EngineLogger->info(std::forward<Args>(args)...); }

        template<typename... Args>
        static void Warn(Args&&... args) { s_EngineLogger->warn(std::forward<Args>(args)...); }

        template<typename... Args>
        static void Error(Args&&... args) { s_EngineLogger->error(std::forward<Args>(args)...); }

        template<typename... Args>
        static void Critical(Args&&... args) { s_EngineLogger->critical(std::forward<Args>(args)...); }

    private:
        static std::shared_ptr<spdlog::logger> s_EngineLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

}

// Convenience macros for client applications
#define CIRCE_LOG_TRACE(...)    ::Circe::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CIRCE_LOG_DEBUG(...)    ::Circe::Log::GetClientLogger()->debug(__VA_ARGS__)
#define CIRCE_LOG_INFO(...)     ::Circe::Log::GetClientLogger()->info(__VA_ARGS__)
#define CIRCE_LOG_WARN(...)     ::Circe::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CIRCE_LOG_ERROR(...)    ::Circe::Log::GetClientLogger()->error(__VA_ARGS__)
#define CIRCE_LOG_CRITICAL(...) ::Circe::Log::GetClientLogger()->critical(__VA_ARGS__)
