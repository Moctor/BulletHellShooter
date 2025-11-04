#pragma once

#include <string>
#include <vector>

#define ANSI_COLOR_RED "\x1B[91m"
#define ANSI_COLOR_GREEN "\x1B[32m"
#define ANSI_COLOR_RESET "\033[0m"


enum class LogType : uint8_t { info, warning, error, critical };


struct LogEntry
{
	LogType type;
	std::string dateTime;
	std::string msg;
};

class Logger
{
	public:
		static void Log(std::string_view msg);
		static void Err(std::string_view msg);


	private:
		static std::string GetCurrentDateTime();
		inline static std::vector<LogEntry> messages;

};
