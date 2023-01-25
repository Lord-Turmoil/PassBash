/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Command.cpp                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 25, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Front end of command. Using m_records pattern.                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/Command.h"
#include "../../inc/common/Logger.h"
#include "../../inc/utility/Auxiliary.h"

#include <stdarg.h>

CommandFactory g_generalFactory;
CommandFactory g_editorFactory; // for editor
CommandFactory g_hiddenFactory;	// for hidden commands

int CommandFactory::execl(const char* cmd, ...)
{
	static char* argv[CMD_ARG_SIZE];

	int argc = 0;
	char* arg;
	va_list args;

	va_start(args, cmd);
	arg = va_arg(args, char*);
	while (arg)
	{
		argv[argc++] = arg;
		arg = va_arg(args, char*);
	}
	argv[argc] = nullptr;

	return execv(cmd, argv);
}

int CommandFactory::execv(const char* cmd, char* argv[])
{
	if (!cmd)
		return -1;

	auto ret = m_records.find(cmd);
	if (ret == m_records.end())	// no such command.
		return -1;

	int argc = 0;
	char** arg = argv;
	while (*arg)
	{
		arg++;
		argc++;
	}

	return (ret->second)(argc, argv);
}

int CommandFactory::hook(const char* descr, Command cmd)
{
	if (!descr)
		return -1;

	auto ret = m_records.emplace(descr, cmd);
	if (ret.second)
		return 0;
	else
		return -1;
}

int CommandFactory::unhook(const char* descr)
{
	if (!descr)
	{
		m_records.clear();
		return 0;
	}

	if (m_records.erase(descr))
		return 0;
	else
		return -1;
}
