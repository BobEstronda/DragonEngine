#include "Logger.h"


namespace DragonLogger
{
    LogTime::LogTime(const std::string& Date) :
        Day{Date.substr(0,3)}, DayNumber{Date.substr(8,2)}, Month{Date.substr(4,3)}, Year{Date.substr(20,4)}, Time{Date.substr(11,8)}
    {

    }

    std::string Logger::GetCurrentDateTime()
    {
        auto Time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        char Buf[30];
        ctime_s(Buf, sizeof(Buf), &Time);

        LogTime logTime{ std::string{Buf} };

        return std::format("{0}-{1}-{2} {3}", logTime.Year, logTime.Month, logTime.DayNumber, logTime.Time);
    }

    Logger& Logger::GetInstance()
    {
        static Logger Instance{};
        return Instance;
    }

    void Logger::Init(bool ConsoleLog, bool RetainsLog)
    {
        assert(!m_bInitialized && "Don not call Initialize more than once!");

        if (m_bInitialized)
        {
            std::cout << "Logger has already been initialized!" << std::endl;
            return;
        }

        m_bConsoleLog = ConsoleLog;
        m_bRetainLogs = RetainsLog;
        m_bInitialized = true;
    }
}