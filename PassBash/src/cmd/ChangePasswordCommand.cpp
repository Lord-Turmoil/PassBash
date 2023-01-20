/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : ChangePasswordCommand.cpp                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 18, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Change password.                                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/CommandHeader.h"

void ChangePasswordCommand::OnStart()
{
	cnsl::Clear();
	cnsl::Print();

	cnsl::InsertText(MESSAGE_COLOR, "You are now changing your master password.\n");
	cnsl::InsertText(MESSAGE_COLOR, "Press ESC to abort operation.\n");
}

bool ChangePasswordCommand::Handle(const ArgListPtr args)
{
	if (_Authorize())
	{
		if (_ReceivePassword())
		{
			_ReEncryptData();
			return STATUS();
		}
	}



	return STATUS();
}

bool ChangePasswordCommand::_Authorize()
{
	char buffer[32];
	int ret;

	cnsl::InsertText(MESSAGE_COLOR, "Please enter your old master password.\n");
	cnsl::InsertText(PROMPT_COLOR, "$ ");
	do
	{
		ret = cnsl::GetPasswordInterruptable(buffer, 0, 31);
		if (ret == -1)
		{
			cnsl::InsertNewLine();
			return false;
		}
	} while (ret == 0);

	while (g_password != buffer)
	{
		cnsl::InsertNewLine();
		cnsl::InsertText(ERROR_COLOR, "WRONG PASSWORD!");
		Sleep(800);
		cnsl::Clear(0);
		cnsl::InsertReverseNewLine();

		do
		{
			cnsl::Clear(0);
			cnsl::InsertText(PROMPT_COLOR, "$ ");
			ret = cnsl::GetPasswordInterruptable(buffer, 0, 31);
			if (ret == -1)
			{
				cnsl::InsertNewLine();
				return false;
			}
		} while (ret == 0);
	}

	cnsl::InsertNewLine();
	cnsl::InsertText(GREETING_COLOR, "Credential confirmed!\n");

	return true;
}

bool ChangePasswordCommand::_ReceivePassword()
{
	char buffer[32];
	int ret;

	cnsl::InsertText(MESSAGE_COLOR, "Please enter new master password.\n");
	cnsl::InsertText(MESSAGE_COLOR, "6 to 16 characters, any ascii that is printable.\n");
	do
	{
		cnsl::Clear(0);
		cnsl::InsertText(PROMPT_COLOR, "$ ");
		ret = cnsl::GetStringInterruptible(buffer, 6, 16);
		if (ret == -1)
		{
			cnsl::InsertNewLine();
			return false;
		}
	} while (ret < 6);

	g_password = buffer;

	cnsl::InsertNewLine();
	cnsl::InsertText(GREETING_COLOR, "New master password saved!\n");
	cnsl::InsertText(MESSAGE_COLOR, "Tips: Use clear to prevent leak of password.\n");
	Sleep(1000);
	// cnsl::Clear();
	// cnsl::Print();

	return true;
}

bool ChangePasswordCommand::_ReEncryptData()
{
	g_config.Save();
	if (!g_passDoc.Save(g_password.c_str()))
	{
		cnsl::InsertText(ERROR_COLOR, "Failed to re-encrypt data!\n");
		return false;
	}

	return true;
}