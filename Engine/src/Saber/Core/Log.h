#pragma once

#include "Saber/Core/Core.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

namespace Saber
{
class Log
{
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
} // namespace Saber

#define SB_CORE_ERROR(...) ::Saber::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SB_CORE_WARN(...) ::Saber::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SB_CORE_INFO(...) ::Saber::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SB_CORE_TRACE(...) ::Saber::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SB_CORE_CRITICAL(...) ::Saber::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define SB_ERROR(...) ::Saber::Log::GetClientLogger()->error(__VA_ARGS__)
#define SB_WARN(...) ::Saber::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SB_INFO(...) ::Saber::Log::GetClientLogger()->info(__VA_ARGS__)
#define SB_TRACE(...) ::Saber::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SB_CRITICAL(...) ::Saber::Log::GetClientLogger()->critical(__VA_ARGS__)
