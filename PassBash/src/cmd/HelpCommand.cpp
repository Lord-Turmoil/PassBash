/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : HelpCommand.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 18, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For global help info.                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/FunctionUtil.h"

 
/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** help
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DEC_CMD(help)
{
	WORD old = cnsl::SetTextForeground(MESSAGE_COLOR);

	cnsl::InsertHeaderLine("PassBash Help", '_');
	cnsl::InsertNewLine();
	cnsl::InsertText(" Tips: There shall not be space in group or item name. Use '-' or else instead.\n\n");
	cnsl::InsertText("        help, h -- show help info\n");
	cnsl::InsertText("                    usage: help\n\n");
	cnsl::InsertText("  clear, cls, c -- clear screen\n");
	cnsl::InsertText("                    usage: clear\n\n");
	cnsl::InsertText("          mkdir -- create group recursively\n");
	cnsl::InsertText("                    usage: mkdir <group name>\n\n");
	cnsl::InsertText("          touch -- create new password item recursively\n");
	cnsl::InsertText("                    usage: touch <item name>\n\n");
	cnsl::InsertText("            cat -- show the content of a password item\n");
	cnsl::InsertText("                    usage: cat <item name>\n\n");
	cnsl::InsertText("             cd -- change working group directory\n");
	cnsl::InsertText("                    usage: cd <group name>\n");
	cnsl::InsertText("                     tips: .  self\n");
	cnsl::InsertText("                           .. parent\n");
	cnsl::InsertText("                           ./ root\n");
	cnsl::InsertText("                           -  last group\n\n");
	cnsl::InsertText("            pwd -- show present working directory\n");
	cnsl::InsertText("                    usage: pwd\n\n");
	cnsl::InsertText("     rename, rn -- rename a group or item\n");
	cnsl::InsertText("                    usage: rename <old> <new>\n\n");
	cnsl::InsertText("             ls -- list group and item in current group or specified group\n");
	cnsl::InsertText("                    usage: ls [group name]\n\n");
	cnsl::InsertText("           tree -- list nodes in tree form\n");
	cnsl::InsertText("                    usage: tree [group name]\n");
	cnsl::InsertText("                           -g -- list groups only\n\n");
	cnsl::InsertText("      edit, mod -- edit password item\n");
	cnsl::InsertText("                    usage: mod <item name>\n\n");
	cnsl::InsertText("       move, mv -- move group or item recursively\n");
	cnsl::InsertText("                    usage: move <src> <dest>\n\n");
	cnsl::InsertText("           find -- find group or password entry in current group\n");
	cnsl::InsertText("                    usage: find [-d] <regular expression>\n");
	cnsl::InsertText("                           -d -- enable deep search\n");
	cnsl::InsertText("                           -s -- enable strict search with regex\n");
	cnsl::InsertText("                                 no \".*\" added around automatically\n\n");
	cnsl::InsertText("        save, s -- save changes\n");
	cnsl::InsertText("                    usage: save\n\n");
	cnsl::InsertText("       remaster -- reset master password\n");
	cnsl::InsertText("                    usage: remaster\n\n");
	cnsl::InsertText("        quit, q -- quit PassBash\n");
	cnsl::InsertText("         change -- change current master password\n");
	cnsl::InsertText("                    usage: change\n");
	cnsl::InsertText("            tea -- external tool, encrypt or decrypt file\n");
	cnsl::InsertText("                    usage: tea [-d] <input file> <output file> [-p password]\n");
	cnsl::InsertText("                           -d -- to decode file\n");
	cnsl::InsertSplitLine('_');
	cnsl::InsertNewLine();

	cnsl::SetTextForeground(old);

	return true;
}