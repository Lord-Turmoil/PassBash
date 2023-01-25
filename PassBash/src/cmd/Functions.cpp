/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Functions.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update : January 25, 2023                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For some tiny commands.                                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/FunctionUtil.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** clear
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DEC_CMD(clear)
{
	cnsl::Clear();

	return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** pwd
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DEC_CMD(pwd)
{
	cnsl::InsertText("\t%s\n", g_pwd.c_str());

	return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** cd <group name>
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static XMLElementPtr last_dir = nullptr;

static int _cd_parse_arg(int argc, char* argv[], std::string& path)
{
	return _parse_optional_args(argc, argv, path);
}

static void _cd_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: cd <group name>\n");
}

DEC_CMD(cd)
{
	std::string path(ROOT_DIR_NAME);
	if (_cd_parse_arg(argc, argv, path) != 0)
	{
		_cd_usage();
		return 1;
	}

	XMLElementPtr node;
	if (path == "-")
		node = last_dir;
	else
		node = GetNodeByPath(path);

	if (!node)
	{
		cnsl::InsertText(ERROR_COLOR, "Group doesn't exist!\n");
		return false;
	}
	if (!IsGroup(node))
	{
		cnsl::InsertText(ERROR_COLOR, "You can only enter a group!\n");
		_cd_usage();
		return false;
	}

	last_dir = g_passDoc.SetCurrent(node);
	GetPresentWorkingDirectory(g_pwd);

	return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** ls [group name]
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static int _ls_parse_arg(int argc, char* argv[], std::string& path)
{
	return _parse_optional_args(argc, argv, path);
}

static void _ls_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: ls [group name]\n");
}

DEC_CMD(ls)
{
	std::string path;
	GetPresentWorkingDirectory(path);

	if (_ls_parse_arg(argc, argv, path) != 0)
	{
		_ls_usage();
		return 1;
	}

	XMLElementPtr node = GetNodeByPath(path);
	if (!node)
	{
		cnsl::InsertText(ERROR_COLOR, "Group doesn't exist!\n");
		return 2;
	}
	else if (!IsGroup(node))
	{
		cnsl::InsertText(ERROR_COLOR, "You can only list a group.\n");
		_ls_usage();
		return 3;
	}

	XMLElementPtrList list;
	GetChildren(node, list);
	if (list.empty())
		cnsl::InsertText(MESSAGE_COLOR, "Nothing...\n");
	else
	{
		int size = (int)list.size();
		cnsl::InsertText(MESSAGE_COLOR, "Total %d groups and items:\n", size);
		for (auto it : list)
		{
			if (IsGroup(it))
				cnsl::InsertText(GROUP_COLOR, "\t%s/\n", GetNodeAttr(it, "name"));
			else
				cnsl::InsertText(ITEM_COLOR, "\t%s\n", GetNodeAttr(it, "name"));
		}
	}

	return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** touch <item name>
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static int _touch_parse_arg(int argc, char* argv[], std::string& path)
{
	return _parse_args(argc, argv, path);
}

static void _touch_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: touch <item name>\n");
}

DEC_CMD(touch)
{
	std::string path("");

	if (_touch_parse_arg(argc, argv, path) != 0)
	{
		_touch_usage();
		return 1;
	}

	std::string name;
	GetBaseName(path, name);
	if (name.empty())
	{
		cnsl::InsertText(ERROR_COLOR, "No name? You must specify a name!\n");
		return 2;
	}

	XMLElementPtr node = GetNodeByPath(path);
	if (node)
	{
		cnsl::InsertText(ERROR_COLOR, "%s with name \"%s\" already exists!\n",
			IsGroup(node) ? "Group" : "Password item", path.c_str());
		return 3;
	}

	node = CreateItemNodeByPath(path);
	if (node)
		cnsl::InsertText(MESSAGE_COLOR, "Password item \"%s\" created.\n", path.c_str());
	else
	{
		cnsl::InsertText(ERROR_COLOR, "Failed to create password item \"%s\".\n", path.c_str());
		return 4;
	}

	g_passDoc.Mark();

	return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** mkdir <group name>
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
int _mkdir_parse_arg(int argc, char* argv[], std::string& path)
{
	return _parse_args(argc, argv, path);
}

void _mkdir_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: mkdir <group name>\n");
}

DEC_CMD(mkdir)
{
	std::string path;

	if (_mkdir_parse_arg(argc, argv, path) != 0)
	{
		_mkdir_usage();
		return 1;
	}

	XMLElementPtr node = GetNodeByPath(path);
	if (node)
	{
		cnsl::InsertText(ERROR_COLOR, "%s with name \"%s\" already exists!\n",
			IsGroup(node) ? "Group" : "Password item", path.c_str());
		return 2;
	}

	node = CreateGroupNodeByPath(path);
	if (node)
		cnsl::InsertText(MESSAGE_COLOR, "Group \"%s\" created.\n", path.c_str());
	else
	{
		cnsl::InsertText(ERROR_COLOR, "Failed to create group \"%s\".\n", path.c_str());
		return 3;
	}

	g_passDoc.Mark();

	return true;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** rm <node name>
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static void _remove_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: rm <path>\n");
}

static int _remove_parse_args(int argc, char* argv[], std::string& path)
{
	return _parse_args(argc, argv, path);
}

static int _remove_confirm(const char* prompt)
{
	char buffer[4];

	cnsl::InsertText(ERROR_COLOR, prompt);
	cnsl::GetString(buffer, 1, 1);
	cnsl::InsertNewLine();

	return (tolower(buffer[0]) == 'y');
}

static int _remove_root()
{
	if (_remove_confirm("You're sure to delete ALL passwords? (Y/N) $ "))
	{
		g_passDoc.SetCurrent(g_passDoc.GetRoot());
		GetPresentWorkingDirectory(g_pwd);
		DeleteChildren(g_passDoc.GetRoot());
		cnsl::InsertText(MESSAGE_COLOR, "All passwords cleared!\n");
	}
	else
		cnsl::InsertText(MESSAGE_COLOR, "Nothing changed.\n");

	return 0;
}

static int _remove_current()
{
	if (_remove_confirm("You're sure to delete current group? (Y/N) $ "))
	{
		XMLElementPtr current = g_passDoc.GetCurrent();
		XMLElementPtr parent = GetParentNode(current);
		g_passDoc.SetCurrent(parent);
		GetPresentWorkingDirectory(g_pwd);
		DeleteNode(current);
		cnsl::InsertText(MESSAGE_COLOR, "Current group removed!\n");
	}
	else
		cnsl::InsertText(MESSAGE_COLOR, "Nothing changed.\n");

	return 0;
}

DEC_CMD(rm)
{
	std::string path;

	if (_remove_parse_args(argc, argv, path) != 0)
	{
		_remove_usage();
		return 1;
	}

	XMLElementPtr node = GetNodeByPath(path);
	if (!node)
	{
		cnsl::InsertText(ERROR_COLOR, "No such thing to delete.\n");
		return 2;
	}
	else if (node == g_passDoc.GetRoot())
		return _remove_root();
	else if (node == g_passDoc.GetCurrent())
		return _remove_current();

	if (!IsParent(g_passDoc.GetCurrent(), node))
	{
		cnsl::InsertText(ERROR_COLOR, "Do not attempt to remove parent or self!\n");
		return 3;
	}

	if (_remove_confirm("You're sure? (Y/N) $ "))
	{
		cnsl::InsertText(MESSAGE_COLOR,  "Group \"%s\" and its contents are deleted permanently!\n",
			IsGroup(node) ? "Group" : "Password item", path.c_str());
		DeleteNode(node);
	}
	else
		cnsl::InsertText(MESSAGE_COLOR, "Nothing changed.\n");

	g_passDoc.Mark();

	return true;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** mv <src node> <dest group>
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static void _move_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: mv <src group|item> <dest group>\n");
}

static int _move_parse_args(int argc, char* argv[], std::string& src, std::string& dest)
{
	return _parse_args(argc, argv, src, dest);
}

DEC_CMD(move)
{
	std::string srcPath;
	std::string destPath;

	if (_move_parse_args(argc, argv, srcPath, destPath) != 0)
	{
		_move_usage();
		return 1;
	}

	XMLElementPtr srcNode = GetNodeByPath(srcPath);
	XMLElementPtr destGroup = GetNodeByPath(destPath);

	if (!srcNode)
	{
		cnsl::InsertText(ERROR_COLOR, "Source doesn't exist!\n");
		return 2;
	}
	if (!destGroup)
	{
		cnsl::InsertText(ERROR_COLOR, "Destination group doesn't exist!\n");
		return 3;
	}
	if (!IsGroup(destGroup))
	{
		cnsl::InsertText(ERROR_COLOR, "Destination must be a group!\n");
		return 4;
	}
	if (srcNode == destGroup)
	{
		cnsl::InsertText(ERROR_COLOR, "Source and destination must not be the same!\n");
		return 5;
	}

	/*
	** 2022/01/19 TS:
	** Fixed name duplicated bug.
	*/
	if (GetDirectChildNode(destGroup, GetNodeName(srcNode)))
	{
		cnsl::InsertText(ERROR_COLOR, "Name exists in destination group!\n");
		return 6;
	}

	// See tinyxml2, it will automatically move from old place.
	std::string oldPath;
	std::string newPath;
	GetNodeDirectory(srcNode, oldPath);
	AddChildNode(destGroup, srcNode);
	GetNodeDirectory(srcNode, newPath);
	cnsl::InsertText(MESSAGE_COLOR, "Moved \"%s\" to \"%s\".\n",
		oldPath.c_str(), newPath.c_str());

	g_passDoc.Mark();

	return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** save
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DEC_CMD(save)
{
	if (g_passDoc.Save(g_password))
	{
		cnsl::InsertText(MESSAGE_COLOR, "Password successfully saved!\n");
		return 0;
	}
	else
	{
		cnsl::InsertText(ERROR_COLOR, "Failed to save password!\n");
		cnsl::InsertText(ERROR_COLOR, "No possible solution!\n");
		return 1;
	}
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** cat <item name>
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static void _cat_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: cat <item name>\n");
}

static int _cat_parse_args(int argc, char* argv[], std::string& path)
{
	return _parse_args(argc, argv, path);
}

DEC_CMD(cat)
{
	std::string path;

	if (_cat_parse_args(argc, argv, path) != 0)
	{
		_cat_usage();
		return 1;
	}

	XMLElementPtr node = GetNodeByPath(path);
	if (!node)
	{
		cnsl::InsertText(ERROR_COLOR, "Password item doesn't exist!\n");
		return 2;
	}
	if (!IsItem(node))
	{
		cnsl::InsertText(ERROR_COLOR, "You can only reveal a password item!\n");
		_cat_usage();
		return 3;
	}

	_show_item(node, nullptr);

	return STATUS();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** rename <src> <dest>
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static void _rename_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: rename <old> <new>\n");
}

static int _rename_parse_args(int argc, char* argv[], std::string& src, std::string& dest)
{
	return _parse_args(argc, argv, src, dest);
}

DEC_CMD(rename)
{
	std::string srcPath;
	std::string destName;

	if (_rename_parse_args(argc, argv, srcPath, destName) != 0)
	{
		_rename_usage();
		return 1;
	}

	XMLElementPtr node = GetNodeByPath(srcPath);
	if (!node)
	{
		cnsl::InsertText(ERROR_COLOR, "Node doesn't exist!\n");
		return 2;
	}
	if (node == g_passDoc.GetRoot())
	{
		cnsl::InsertText(ERROR_COLOR, "You can not rename root!\n");
		return 3;
	}
	if (!IsLegalNodeName(destName))
	{
		cnsl::InsertText(ERROR_COLOR, "Illegal node name! No '/' allowed.\n");
		return 4;
	}

	XMLElementPtr parent = GetParentNode(node);
	if (GetDirectChildNode(parent, destName.c_str()))
	{
		cnsl::InsertText(ERROR_COLOR, "New name already exist!\n");
		return 5;
	}
	
	std::string oldPath;
	std::string newPath;
	GetNodeDirectory(node, oldPath);
	node->SetAttribute("name", destName.c_str());
	GetNodeDirectory(node, newPath);
	
	cnsl::InsertText(MESSAGE_COLOR, "Rename \"%s\" to \"%s\".\n",
		oldPath.c_str(), newPath.c_str());

	g_passDoc.Mark();

	return 0;
}

DEC_CMD(unknown)
{
	cnsl::InsertText(ERROR_COLOR, "\"%s\" is not a command.\n", argv[1]);
	cnsl::InsertText(MESSAGE_COLOR, "Use \"help\" for more information.\n");

	return 0;
}

DEC_CMD(quit)
{
	return 66;
}