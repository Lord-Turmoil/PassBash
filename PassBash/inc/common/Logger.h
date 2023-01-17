/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Logger.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For error logs.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "Macros.h"

#include <vector>
#include <string>

enum class LoggerStatus
{
	GOOD,
	BAD
};

const int LOGGER_BUFFER_SIZE = 1024;

class Logger
{
public:
	static void LogError(const char* format, ...);
	static void ClearErrors();
	static void PrintErrors();

	static void LogMessage(const char* format, ...);
	static void ClearMessages();
	static void PrintMessages();

	static bool Good();

private:
	static std::vector<std::string> m_logs;
	static std::vector<std::string> m_msgs;
	static char m_buffer[LOGGER_BUFFER_SIZE];
};

#define LOG_LOCATION R"(In "%s": )"


#define LOG_ERROR(FORMAT, ...)   Logger::LogError(FORMAT, __VA_ARGS__)
#define LOG_MESSAGE(FORMAT, ...) Logger::LogMessage(FORMAT, __VA_ARGS__)

#define LOG_CLEAR_ERRORS()   Logger::ClearErrors()
#define LOG_CLEAR_MESSAGES() Logger::ClearMessages()

#define PRINT_ERROR()   Logger::PrintErrors()
#define PRINT_MESSAGE() Logger::PrintMessages()

#define HAS_ERROR() (!Logger::Good())
#define STATUS()    (Logger::Good())
#endif