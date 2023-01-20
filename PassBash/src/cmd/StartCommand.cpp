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
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Do all initializations and start the service.                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/CommandHeader.h"


void StartCommand::OnStart()
{
	_InitConsole();
	cnsl::InsertText(FOREGROUND_LIGHT(FOREGROUND_CYAN),
		"# Pash Host Version: 1.1.0\n\n");
}

bool StartCommand::Handle(const ArgListPtr args)
{
	_InitConfig();

	// First use.
	if (g_password == g_default)
	{
		Scheduler::GetInstance()
			->AddTask(CommandFactory::SpawnSpecial("Register"), nullptr);
	}
	else
	{
		Scheduler::GetInstance()
			->AddTask(CommandFactory::SpawnSpecial("Login"), nullptr);
	}

	return !HAS_ERROR();
}

void StartCommand::_InitConsole()
{
	cnsl::InitConsoleSize(120, 30);
	cnsl::InitConsole(116);

	cnsl::SetHeader(g_TITLE, g_COPYRIGHT, g_AUTHOR);
	cnsl::SetTextForeground(FOREGROUND_WHITE);
	cnsl::SetTextBackground(BACKGROUND_BLACK);
	cnsl::Print();
	cnsl::OverflowReprint(false);
}

void StartCommand::_InitConfig()
{
	g_config.Load();
}