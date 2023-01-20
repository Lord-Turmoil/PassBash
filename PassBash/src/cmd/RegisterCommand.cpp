/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : RegisterCommand.cpp                       *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Err... For registration at the first time.                               *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/CommandHeader.h"

void RegisterCommand::OnStart()
{
	_PrintGreeting();
}

bool RegisterCommand::Handle(const ArgListPtr args)
{
	_ReceiveNewPassword();
	_ReEncryptData();

	if (!HAS_ERROR())
	{
		Scheduler::GetInstance()
			->AddTask(CommandFactory::SpawnSpecial("Host"), nullptr);
	}

	return !HAS_ERROR();
}

void RegisterCommand::_PrintGreeting()
{
	WORD old = cnsl::SetTextForeground(FOREGROUND_LIGHT(GREETING_COLOR));
	cnsl::InsertText(" Welcome to PassBash, my friend! ");
	cnsl::InsertText(" Or, you can call me - Pash. :)\n");
	cnsl::SetTextForeground(old);
	
	cnsl::InsertText(" I find that it is your first time with me.");
	cnsl::InsertText(" So it is required that you set a master password.\n");

	old = cnsl::SetTextForeground(FOREGROUND_LIGHT(MESSAGE_COLOR));
	cnsl::InsertText(" Please enter and ");
	cnsl::SetTextForeground(FOREGROUND_LIGHT(ERROR_COLOR));
	cnsl::InsertText("REMEMBER");
	cnsl::SetTextForeground(FOREGROUND_LIGHT(MESSAGE_COLOR));
	cnsl::InsertText(" your master password! ");
	cnsl::InsertText("Your master password cannot be retrieved!\n");
	cnsl::SetTextForeground(old);
}

void RegisterCommand::_ReceiveNewPassword()
{
	char buffer[24];
	
	cnsl::InsertText("6 to 16 characters, any ascii that is printable.\n");
	cnsl::InsertText(PROMPT_COLOR, "$ ");
	cnsl::GetString(buffer, 6, 16);

	g_password = buffer;
}

void RegisterCommand::_ReEncryptData()
{
	g_config.Save();
	g_passDoc.Load(g_default.c_str());
	g_passDoc.Save(g_password.c_str());
	g_passDoc.UnLoad();
}