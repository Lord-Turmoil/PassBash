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
static bool group_only;

static void _tree_init()
{
	group_only = false;
}

static void _tree_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: tree [options] <item name>\n");
	cnsl::InsertText(MESSAGE_COLOR, "  -g -- list groups only\n");
}

static int _tree_parse_args(int argc, char* argv[], std::string& path)
{
	int opt;
	bool flag = false;
	bool err = false;
	bool tag = false;
	while (opt = getopt(argc, argv, "g"))
	{
		switch (opt)
		{
		case 'g':
			group_only = true;
			break;
		case '!':
			if (flag)
			{
				err = true;
				if (!tag)
				{
					cnsl::InsertText(ERROR_COLOR, "Too many arguments!\n");
					tag = true;
				}
			}
			else
			{
				flag = true;
				path = optarg;
			}
			break;
		case '?':
			err = true;
			cnsl::InsertText(ERROR_COLOR, "Unknown parameter \"-%c\"\n", optopt);
			break;
		default:
			break;
		}
	}
	if (err)
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		return 1;
	}

	return 0;
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
	if (group_only)
		GetGroupChildren(node, list);
	else
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

	_tree_init();

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
