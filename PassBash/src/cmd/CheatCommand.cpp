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
 *                    Last Update : January 25, 2023                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Emm... For debug purpose.                                                *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/FunctionUtil.h"


// order <id>
static void _order_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: order <order>\n");
}

static int _order_parse_args(int argc, char* argv[], std::string& order)
{
	return _parse_args(argc, argv, order);
}

DEC_CMD(order)
{
	std::string order;

	if (_order_parse_args(argc, argv, order) != 0)
	{
		_order_usage();
		return 1;
	}

	if (order == "msg")
		LOG_PRINT_MESSAGE();
	else if (order == "cmsg")
		LOG_CLEAR_MESSAGES();
	else if (order == "err")
		LOG_PRINT_ERROR();
	else if (order == "cerr")
		LOG_CLEAR_ERRORS();
	else if (order == "66")
	{
		if (g_passDoc.Save())
			cnsl::InsertText(MESSAGE_COLOR, "Password exported in plain text.\n");
		else
		{
			cnsl::InsertText(ERROR_COLOR, "Failed to export password.\n");
			return 2;
		}
	}
	else if (order == "99")
	{
		if (g_passDoc.Load())
		{
			cnsl::InsertText(MESSAGE_COLOR, "Plain text password imported.\n");
			g_passDoc.Mark();
		}
		else
		{
			cnsl::InsertText(ERROR_COLOR, "Failed to import password.\n");
			return 3;
		}
	}
	else
	{
		cnsl::InsertText(ERROR_COLOR, "Invalid order.\n");
		return 4;
	}

	return 0;
}