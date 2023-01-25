/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : StartCommand.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update : January 25, 2023                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Do all initializations and start the service.                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/Service.h"
#include "../../inc/cmd/FunctionUtil.h"


static void _start_init_console()
{
	cnsl::InitConsoleSize(120, 30);
	cnsl::InitConsole(116);

	cnsl::SetHeader(g_TITLE, g_COPYRIGHT, g_AUTHOR);
	cnsl::SetTextForeground(FOREGROUND_WHITE);
	cnsl::SetTextBackground(BACKGROUND_BLACK);
	cnsl::Print();
	cnsl::OverflowReprint(false);
}

static int _start_init_config()
{
	if (g_config.Load())
		return 0;
	else
		return 1;
}

DEC_CMD(start)
{
	_start_init_console();
	cnsl::InsertText(FOREGROUND_LIGHT(FOREGROUND_CYAN),
		"# Pash Host Version: 2.0.0\n\n");

	_start_init_config();

	int ret;
	const char* cmd;
	// First use.
	if (g_encodedPassword[0] == '\0')
		cmd = "reg";
	else
		cmd = "login";

	ret = g_hiddenFactory.execl(cmd, nullptr);
	if (ret == -1)
	{
		cnsl::InsertText(ERROR_COLOR, "Failed to launch service \"%s\".\n", cmd);
		return 2;
	}

	return 0;
}