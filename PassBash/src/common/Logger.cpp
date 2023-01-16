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

void Logger::Clear()
{
	status = LoggerStatus::GOOD;
	m_logs.clear();
}

void Logger::PrintLogs()
{
	cnsl::TextAttribute newAttr;
	cnsl::TextAttribute oldAttr;

	newAttr.foreground = FOREGROUND_RED;
	newAttr.background = 0;
	cnsl::SetTextAttribute(newAttr, &oldAttr);
	
	cnsl::InsertHeaderLine("Error Log", '_');
	for (auto it = m_logs.rbegin(); it != m_logs.rend(); it++)
		cnsl::InsertText("\t%s\n", it->c_str());
	cnsl::InsertSplitLine('_');
	
	cnsl::SetTextAttribute(oldAttr, nullptr);
}

bool Logger::Good()
{
	return status == LoggerStatus::GOOD;
}