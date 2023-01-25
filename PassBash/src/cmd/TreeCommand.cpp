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
 *                    Last Update : January 25, 2023                          *
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

#include "../../inc/cmd/FunctionUtil.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** tree [group name]
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static void _tree_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: ls <item name>");
}

static int _tree_parse_args(int argc, char* argv[], std::string& path)
{
	return _parse_optional_args(argc, argv, path);
}

/********************************************************************
** This is a little tricky.
** To make it clear, '.' is for leading space.
** . current [leading "."]
** ...|-- alpha [leading ".'..|.."]
** ...|..  \-- beta [leading ".'..|..'.....]
** ...
*/
static void _tree(XMLElementPtr node, const std::string& leading)
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
			_tree(it, leading + "  |  ");
		}
		else
		{
			cnsl::InsertText("  \\--");
			_tree(it, leading + "     ");
		}
		it = it->NextSiblingElement();
	}
}

DEC_CMD(tree)
{
	std::string path;

	GetPresentWorkingDirectory(path);
	if (_tree_parse_args(argc, argv, path) != 0)
	{
		_tree_usage();
		return 1;
	}
	
	XMLElementPtr node = GetNodeByPath(path);
	if (!node)
	{
		cnsl::InsertText(ERROR_COLOR, "Group doesn't exist!\n");
		return 2;
	}
	_tree(node, " ");
	
	return 0;
}
