/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : LoginCommand.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Simple login, just check for password.                                   *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/CommandHeader.h"

void LoginCommand::OnStart()
{
	cnsl::InsertText(GREETING_COLOR, "Welcome back, my friend!\n");
}

bool LoginCommand::Handle(const ArgListPtr args)
{
	if (!_ReceivePassword())
		return false;

	cnsl::InsertNewLine();
	cnsl::InsertText(GREETING_COLOR, "Credential confirmed!");
	Sleep(500);

	if (!HAS_ERROR())
	{
		Scheduler::GetInstance()
			->AddTask(CommandFactory::SpawnSpecial("Host"), nullptr);
	}

	return STATUS();
}

bool LoginCommand::_ReceivePassword()
{
	char buffer[32];
	int ret = 0;

	cnsl::InsertText(MESSAGE_COLOR, "Please enter your master password.\n");
	cnsl::InsertText(PROMPT_COLOR, "$ ");
	do
	{
		ret = cnsl::GetPasswordInterruptable(buffer, 0, 31);
		if (ret == -1)
			return false;
	} while (ret == 0);
	while (g_password != buffer)
	{
		cnsl::InsertNewLine();
		cnsl::InsertText(ERROR_COLOR, "WRONG PASSWORD!");
		Sleep(800);
		cnsl::Clear(0);
		cnsl::InsertReverseNewLine();
		cnsl::Clear(0);
		cnsl::InsertCarrige();

		cnsl::InsertText(PROMPT_COLOR, "$ ");
		do
		{
			ret = cnsl::GetPasswordInterruptable(buffer, 0, 31);
			if (ret == -1)
				return false;
		} while (ret == 0);
	}

	return true;
}