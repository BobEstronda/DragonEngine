#pragma once

#include <string>
#include <string_view>
#include <source_location>
#include <vector>
#include <cassert>

#define DRAGON_LOG(x, ...) DragonLogger::Logger::GetInstance().Log(x,__VA_ARGS__);
#define DRAGON_WARN(x, ...) DragonLogger::Logger::GetInstance().Warn(x,__VA_ARGS__);
#define DRAGON_ERROR(x, ...) DragonLogger::Logger::GetInstance().Error(std::source_location::current(),x,__VA_ARGS__);
#define DRAGON_INIT_LOGS(console,retain) DragonLogger::Logger::GetInstance().Init(console,retain);

namespace DragonLogger
{
	struct LogEntry
	{
		enum class LogType { INFO, WARN, ERR, NONE };

		LogType Type{ LogType::INFO };

		std::string Log{ "" };
	};

	struct LogTime
	{
		std::string Day, DayNumber, Month, Year, Time;
		LogTime(const std::string& Date);
	};

	class Logger
	{

	private:
		std::vector<LogEntry> m_LogEntries;
		bool m_bLogAdded{ false }, m_bInitialized{ false }, m_bConsoleLog{ true }, m_bRetainLogs{ true };

		Logger() = default;

		std::string GetCurrentDateTime();

	public:
		static Logger& GetInstance();

		~Logger() = default;

		// Make the logger non-copyable
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		void Init(bool ConsoleLog = true, bool RetainsLog = true);

		template <typename... Args>
		void Log(const std::string& Message, Args&&... args);

		template <typename... Args>
		void Warn(const std::string& Message, Args&&... args);

		template <typename... Args>
		void Error(std::source_location Location,const std::string& Message, Args&&... args);

	};
}

#include "Logger.inl"