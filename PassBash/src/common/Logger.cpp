/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Logger.cpp                                *
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

#include "../../inc/common/Logger.h"

#include <cnsl.h>

#include <cstdio>
#include <cstdarg>


std::vector<std::string> Logger::m_logs;
std::vector<std::string> Logger::m_msgs;
char Logger::m_buffer[LOGGER_BUFFER_SIZE];

static LoggerStatus status = LoggerStatus::GOOD;

void Logger::LogError(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vsprintf_s(m_buffer, format, args);
	va_end(args);

	m_logs.push_back(m_buffer);
	status = LoggerStatus::BAD;
}

void Logger::ClearErrors()
{
	status = LoggerStatus::GOOD;
	m_logs.clear();
}

void Logger::PrintErrors()
{
	WORD old = cnsl::SetTextForeground(FOREGROUND_RED);
	
	cnsl::InsertHeaderLine("Error Log", '_');
	for (auto it = m_logs.rbegin(); it != m_logs.rend(); it++)
		cnsl::InsertText("\t%s\n", it->c_str());
	cnsl::InsertSplitLine('_');
	
	cnsl::SetTextForeground(old);
}

void Logger::LogMessage(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vsprintf_s(m_buffer, format, args);
	va_end(args);

	m_msgs.push_back(m_buffer);
}

void Logger::ClearMessages()
{
	m_msgs.clear();
}

void Logger::PrintMessages()
{
	WORD old = cnsl::SetTextForeground(FOREGROUND_LIGHT(FOREGROUND_MAGENTA));

	cnsl::InsertHeaderLine("Message Log", '_');
	for (auto it = m_msgs.rbegin(); it != m_msgs.rend(); it++)
		cnsl::InsertText("\t%s\n", it->c_str());
	cnsl::InsertSplitLine('_');

	cnsl::SetTextForeground(old);
}

bool Logger::Good()
{
	return status == LoggerStatus::GOOD;
}