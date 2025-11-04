#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iostream>


std::string Logger::GetCurrentDateTime()
{
	// Get the current time point
	auto now = std::chrono::system_clock::now();

	// Convert to time_t for easy manip
	std::time_t timeNow = std::chrono::system_clock::to_time_t(now);

	//Create a tm struc for formatting
	std::tm tmNow{};
	localtime_s(&tmNow, &timeNow);


	//Create string stream to format the output
	std::ostringstream oss;
	oss << std::put_time(&tmNow, "%Y-%m-%d %H:%M:%S");


	return oss.str();
}

void Logger::Log(std::string_view msg)
{
	LogEntry logEntry;
	logEntry.type = LogType::info;
	logEntry.dateTime = GetCurrentDateTime();
	logEntry.msg = msg;

	messages.push_back(logEntry);

	std::cout << ANSI_COLOR_GREEN << "LOG [" << logEntry.dateTime << "]" << logEntry.msg << ANSI_COLOR_RESET << std::endl;

}

void  Logger::Err(std::string_view msg)
{
	LogEntry logEntry;
	logEntry.type = LogType::error;
	logEntry.dateTime = GetCurrentDateTime();
	logEntry.msg = msg;

	messages.push_back(logEntry);

	std::cout << ANSI_COLOR_RED << "LOG [" << logEntry.dateTime << "]" << logEntry.msg << ANSI_COLOR_RESET << std::endl;
}