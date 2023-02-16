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
 *                    Last Update : February 16, 2023                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Simple login, just check for password.                                   *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/Service.h"
#include "../../inc/cmd/FunctionUtil.h"

#include <tea.h>

static bool _validate(const char* key)
{
	tea::TEABufferReader* reader = new tea::TEABufferReader(g_encodedPassword);
	tea::TEABufferWriter* writer = new tea::TEABufferWriter(g_decodedPassword);
	tea::decode(reader, writer, key);
	delete reader;
	delete writer;

	return _STR_SAME(key, g_decodedPassword);
}

static int _receive_password()
{
	char buffer[g_PASSWORD_BUFFER_SIZE + 1];
	int ret = 0;

	cnsl::InsertText(MESSAGE_COLOR, "Please enter your master password.\n");
	cnsl::InsertText(PROMPT_COLOR, "$ ");
	do
	{
		ret = cnsl::GetPasswordInterruptable(buffer, 0, g_PASSWORD_LENGTH);
		if (ret == -1)
			return 1;
	} while (ret == 0);
	_format_password(buffer, g_password);
	while (!_validate(g_password))
	{
		cnsl::InsertNewLine();
		cnsl::InsertText(ERROR_COLOR, "WRONG PASSWORD!");
		Sleep(800);
		cnsl::Clear(0);
		cnsl::InsertReverseNewLine();
		cnsl::Clear(0);

		cnsl::InsertText(PROMPT_COLOR, "$ ");
		do
		{
			ret = cnsl::GetPasswordInterruptable(buffer, 0, g_PASSWORD_LENGTH);
			if (ret == -1)
				return 1;
		} while (ret == 0);
		_format_password(buffer, g_password);
	}

	return 0;
}

DEC_CMD(login)
{
	cnsl::InsertText(GREETING_COLOR, "Welcome back, my friend!\n");

	if (_receive_password() != 0)
		return 1;

	cnsl::InsertText(GREETING_COLOR, "\nCredential confirmed!\n\n");

	int ret = g_hiddenFactory.execl("host", nullptr);
	if (ret == -1)
	{
		cnsl::InsertText(ERROR_COLOR, "Failed to launch host service!\n");
		return 2;
	}
	else if (ret != 0)
	{
		cnsl::InsertText(ERROR_COLOR, "Host service encountered critical error!\n");
		LOG_PRINT_MESSAGE();
		LOG_PRINT_ERROR();
		cnsl::WaitForKey("Press any key to quit...");
	}

	return 0;
}
