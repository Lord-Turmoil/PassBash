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

#include "../../inc/cmd/FunctionUtil.h"

static void _reg_greet()
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

static void _receive_password()
{
	char buffer[g_PASSWORD_BUFFER_SIZE + 1];

	cnsl::InsertText("6 to 16 characters, any ascii that is printable.\n");
	cnsl::InsertText(PROMPT_COLOR, "$ ");

	cnsl::InputOptions options;
	options.minLen = 6;
	options.maxLen = g_PASSWORD_LENGTH;
	cnsl::GetString(buffer, options);
	cnsl::InsertNewLine();
	_format_password(buffer, g_password);
}

static void _reencrtypt_data()
{
	g_config.Save();
	g_passDoc.Load(g_default);
	g_passDoc.Save(g_password);
	g_passDoc.UnLoad();
}

DEC_CMD(reg)
{
	_reg_greet();

	_receive_password();
	_reencrtypt_data();

	int ret = g_hiddenFactory.execl("host", nullptr);
	if (ret == -1)
	{
		cnsl::InsertText(ERROR_COLOR, "Failed to launch host service!\n");
		return 1;
	}

	return 0;
}
