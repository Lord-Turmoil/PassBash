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
	static void Clear();
	static void PrintLogs();
	static bool Good();

private:
	static std::vector<std::string> m_logs;
	static char m_buffer[LOGGER_BUFFER_SIZE];
};

#define LOG_LOCATION R"(In "%s": )"

#define LOG_ERROR(FORMAT, ...) Logger::LogError(FORMAT, __VA_ARGS__)
#define LOG_CLEAR() Logger::Clear()
#define PRINT_LOG() Logger::PrintLogs()
#define HAS_ERROR() (!Logger::Good())

#endif