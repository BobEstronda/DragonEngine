#pragma once

#include <string>
#include <string_view>
#include <source_location>
#include <vector>
#include <cassert>

namespace DragonLogger
{
	struct LogEntry
	{
		enum class LogType { INFO, WARN, ERR, NONE };

		LogType Type{ LogType::INFO };

		std::string Log{ "" };
	};

	class Logger
	{

	private:
		Logger() = default;
		~Logger() = default;



	public:

	};
}
