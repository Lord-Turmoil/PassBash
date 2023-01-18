/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : TreeCommand.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For tree command.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

/********************************************************************
** Of course, we use traverse, but this is a little tricky. There's
** gonna be some leading characters.
*/

#include "../../inc/cmd/CommandHeader.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** tree [group name]
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool TreeCommand::Handle(const ArgListPtr args)
{
	if (args && (args->size() > 1))
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		cnsl::InsertText(MESSAGE_COLOR, "Usage: ls <item name>");
		cnsl::InsertNewLine();
		return false;
	}

	XMLElementPtr current = g_passDoc.GetCurrent();
	XMLElementPtrList list;
	if (!args || args->empty())
		_Tree(current, " ");
	else
	{
		XMLElementPtr node = GetNodeByPath((*args)[0]);
		if (!node)
		{
			cnsl::InsertText(ERROR_COLOR, "Group doesn't exist!\n");
			return false;
		}
		_Tree(node, " ");
	}

	return true;
}

/********************************************************************
** This is a little tricky.
** To make it clear, '.' is for leading space.
** . current [leading "."]
** ...|-- alpha [leading ".'..|.."]
** ...|..  \-- beta [leading ".'..|..'.....]
** ...
*/
void TreeCommand::_Tree(XMLElementPtr node, const std::string& leading)
{
	// First output itself, with leading previously added.
	if (IsGroup(node))
		cnsl::InsertText(GROUP_COLOR, " %s/\n", GetNodeName(node));
	else
	{
		cnsl::InsertText(ITEM_COLOR, " %s\n", GetNodeName(node));
		return;
	}

	// Then, output its children with leading.
	XMLElementPtrList list;
	GetChildren(node, list);
	for (auto it : list)
	{
		cnsl::InsertText("%s", leading.c_str());
		if (it != list.back())
		{
			cnsl::InsertText("  |--");
			_Tree(it, leading + "  |  ");
		}
		else
		{
			cnsl::InsertText("  \\--");
			_Tree(it, leading + "     ");
		}
		it = it->NextSiblingElement();
	}
}