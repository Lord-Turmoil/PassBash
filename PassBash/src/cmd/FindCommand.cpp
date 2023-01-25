/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : FindCommand.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 20, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Find command.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/FunctionUtil.h"

#include <regex>

static bool is_deep;
static bool is_strict;
std::string pattern;
std::regex reg_pattern;

static void _find_init()
{
	is_deep = false;
	is_strict = false;
	pattern = "";
}

static void _find_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: find [options] <regular expression>\n");
	cnsl::InsertText(MESSAGE_COLOR, "  -d -- enable deep search inside password item\n");
	cnsl::InsertText(MESSAGE_COLOR, "  -s -- enable strict search with regex, no \".*\" added around\n");
}

static int _find_parse_args(int argc, char* argv[])
{
	int opt;
	bool flag = false;
	bool err = false;
	while (opt = getopt(argc, argv, "ds"))
	{
		switch (opt)
		{
		case 'd':
			is_deep = true;
			break;
		case 's':
			is_strict = true;
			break;
		case '!':
			if (flag)
			{
				err = true;
				cnsl::InsertText(ERROR_COLOR, "Too many arguments!\n");
			}
			else
			{
				flag = true;
				pattern = optarg;
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
	if (!flag)
		return 2;

	return 0;
}

void _search_item(XMLElementPtr root, XMLElementPtrList& list)
{
	if (!IsItem(root))
		return;

	if (std::regex_match(GetNodeName(root), reg_pattern))
	{
		list.push_back(root);
		return;
	}

	if (!is_deep)
		return;

	XMLElementPtr it = root->FirstChildElement();
	while (it)
	{
		if (std::regex_match(GetNodeAttr(it, "key"), reg_pattern))
		{
			list.push_back(root);
			return;
		}
		else if (std::regex_match(GetNodeAttr(it, "value"), reg_pattern))
		{
			list.push_back(root);
			return;
		}
		it = it->NextSiblingElement();
	}
}

void _search(XMLElementPtr root, XMLElementPtrList& list)
{
	XMLElementPtr it = root->FirstChildElement();
	while (it)
	{
		if (IsGroup(it))
		{
			if (std::regex_match(GetNodeName(it), reg_pattern))
				list.push_back(it);
			_search(it, list);
		}
		else
			_search_item(it, list);

		it = it->NextSiblingElement();
	}
}

static void _find(XMLElementPtrList& list)
{
	if (is_strict)
		reg_pattern.assign(pattern, std::regex::icase);
	else
		reg_pattern.assign(".*" + pattern + ".*", std::regex::icase);
	_search(g_passDoc.GetCurrent(), list);
}

DEC_CMD(find)
{
	_find_init();

	if (_find_parse_args(argc, argv) != 0)
	{
		_find_usage();
		return 1;
	}

	XMLElementPtrList list;
	_find(list);

	if (list.empty())
		cnsl::InsertText(MESSAGE_COLOR, "Nothing...\n");
	else
	{
		int size = (int)list.size();
		cnsl::InsertText(MESSAGE_COLOR, "Total %d %s:\n",
			size, (size > 1) ? "results" : "result");
		std::string path;
		for (auto it : list)
		{
			cnsl::InsertText(IsGroup(it) ? GROUP_COLOR : ITEM_COLOR,
				"\t%s\n", GetNodeDirectory(it, path));
		}
	}

	return 0;
}
