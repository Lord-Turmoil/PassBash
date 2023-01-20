/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PashBash                                  *
 *                                                                            *
 *                      File Name : CheatCommand.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 20, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Emm... For debug purpose.                                                *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/CommandHeader.h"

// order <id>
bool CheatCommand::Handle(const ArgListPtr args)
{
	if (!args || (args->size() > 1))
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		cnsl::InsertText(MESSAGE_COLOR, "Usage: order <order>\n");
		return false;
	}

	std::string& order = (*args)[0];
	if (order == "msg")
	{
		PRINT_MESSAGE();
		return true;
	}
	else if (order == "err")
	{
		PRINT_ERROR();
		return true;
	}
	else if (order == "66")
	{
		if (g_passDoc.Save())
		{
			cnsl::InsertText(MESSAGE_COLOR, "Password exported in plain text.\n");
			return true;
		}
		else
		{
			cnsl::InsertText(ERROR_COLOR, "Failed to export password.\n");
			return false;
		}
	}
	else if (order == "99")
	{
		if (g_passDoc.Load())
		{
			cnsl::InsertText(MESSAGE_COLOR, "Plain text password imported.\n");
			g_passDoc.Mark();
			return true;
		}
		else
		{
			cnsl::InsertText(ERROR_COLOR, "Failed to import password.\n");
			return false;
		}
	}

	cnsl::InsertText(ERROR_COLOR, "Invalid order.\n");

	return false;
}