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

#include "../../inc/cmd/FunctionUtil.h"

static void _change_greet()
{
	cnsl::InsertText(MESSAGE_COLOR, "You are now changing your master password.\n");
	cnsl::InsertText(MESSAGE_COLOR, "Press ESC to abort operation.\n");
}

static int _authorize()
{
	char buffer[g_PASSWORD_BUFFER_SIZE + 1];
	int ret;

	cnsl::InsertText(MESSAGE_COLOR, "Please enter your old master password.\n");
	cnsl::InsertText(PROMPT_COLOR, "$ ");
	do
	{
		ret = cnsl::GetPasswordInterruptable(buffer, 0, g_PASSWORD_BUFFER_SIZE);
		if (ret == -1)
		{
			cnsl::InsertNewLine();
			return 1;
		}
	} while (ret == 0);

	while (_STR_DIFF(g_password, buffer))
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
			ret = cnsl::GetPasswordInterruptable(buffer, 0, g_PASSWORD_BUFFER_SIZE);
			if (ret == -1)
			{
				cnsl::InsertNewLine();
				return 1;
			}
		} while (ret == 0);
	}

	cnsl::InsertNewLine();
	cnsl::InsertText(GREETING_COLOR, "Credential confirmed!\n");

	return 0;
}

static int _receive_password()
{
	char buffer[g_PASSWORD_BUFFER_SIZE + 1];
	int ret;

	cnsl::InsertText(MESSAGE_COLOR, "Please enter new master password.\n");
	cnsl::InsertText(MESSAGE_COLOR, "6 to 16 characters, any ascii that is printable.\n");
	do
	{
		cnsl::Clear(0);
		cnsl::InsertText(PROMPT_COLOR, "$ ");
		ret = cnsl::GetStringInterruptible(buffer, 6, g_PASSWORD_LENGTH);
		if (ret == -1)
		{
			cnsl::InsertNewLine();
			return 1;
		}
	} while (ret < 6);

	_format_password(buffer, g_password);

	cnsl::InsertNewLine();
	cnsl::InsertText(GREETING_COLOR, "New master password saved!\n");
	cnsl::InsertText(MESSAGE_COLOR, "Tips: Use clear to prevent leak of password.\n");
	// Sleep(1000);
	// cnsl::Clear();
	// cnsl::Print();

	return 0;
}

static int _reencrypt_data()
{
	g_config.Save();
	if (!g_passDoc.Save(g_password))
	{
		cnsl::InsertText(ERROR_COLOR, "Failed to re-encrypt data!\n");
		return 1;
	}

	return 0;
}


DEC_CMD(change)
{
	_change_greet();

	if (_authorize())
	{
		if (_receive_password())
			return _reencrypt_data();
	}

	return 2;
}
