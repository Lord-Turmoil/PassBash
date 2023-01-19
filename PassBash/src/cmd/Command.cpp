/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Command.cpp                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For some tiny commands.                                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/CommandHeader.h"

#include <stack>
#include <cstdlib>


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** clear
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool ClearCommand::Handle(const ArgListPtr args)
{
	cnsl::Clear();
	// cnsl::Print();

	return true;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** pwd
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool PwdCommand::Handle(const ArgListPtr args)
{
	cnsl::InsertText("%s\n", g_pwd.c_str());
	return true;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** cd <group name>
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool ChangeDirectoryCommand::Handle(const ArgListPtr args)
{
	if (args && (args->size() != 1))
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		cnsl::InsertText(MESSAGE_COLOR, "Usage: cd <group name>");
		cnsl::InsertNewLine();
		return false;
	}

	const std::string& path = args ? (*args)[0] : "./";
	XMLElementPtr node;
	if (_STR_SAME(path.c_str(), ROOT_DIR_NAME))
		node = g_passDoc.GetRoot();
	else
		node = GetNodeByPath((*args)[0]);
	if (!node)
	{
		cnsl::InsertText(ERROR_COLOR, "Group doesn't exist!\n");
		return false;
	}
	if (!IsGroup(node))
	{
		cnsl::InsertText(ERROR_COLOR, "You can only enter a group!\n");
		cnsl::InsertText(MESSAGE_COLOR, "Usage: cd <group name>\n");
		return false;
	}

	g_passDoc.SetCurrent(node);
	g_passDoc.GetPresentWorkingDirectory(g_pwd);

	return true;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** ls [group name]
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool ListCommand::Handle(const ArgListPtr args)
{
	if (args && (args->size() > 1))
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		cnsl::InsertText(MESSAGE_COLOR, "Usage: ls [group name]\n");
		cnsl::InsertNewLine();
		return false;
	}

	XMLElementPtr current = g_passDoc.GetCurrent();
	XMLElementPtrList list;
	if (!args || args->empty())
		GetChildren(current, list);
	else
	{
		const std::string& path = (*args)[0];
		XMLElementPtr node = GetNodeByPath(path);
		if (!node)
		{
			cnsl::InsertText(ERROR_COLOR, "Group doesn't exist!\n");
			return false;
		}
		else if (!IsGroup(node))
		{
			cnsl::InsertText(ERROR_COLOR, "You can only list a group.\n");
			return false;
		}
		GetChildren(node, list);
	}

	if (list.empty())
		cnsl::InsertText(MESSAGE_COLOR, "Nothing...\n");
	else
	{
		for (auto it : list)
		{
			if (IsGroup(it))
				cnsl::InsertText(GROUP_COLOR, "\t%s/\n", GetNodeAttr(it, "name"));
			else
				cnsl::InsertText(ITEM_COLOR, "\t%s\n", GetNodeAttr(it, "name"));
		}
	}

	return true;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** touch <item name>
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool TouchCommand::Handle(const ArgListPtr args)
{
	if (!args || (args->size() != 1))
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		cnsl::InsertText(MESSAGE_COLOR, "Usage: touch <item name>");
		cnsl::InsertText(MESSAGE_COLOR, "Item name shall not contain white space. Use '-' instead.\n");
		cnsl::InsertNewLine();
		return false;
	}

	const std::string& path = (*args)[0];
	std::string name;
	GetBaseName(path, name);
	if (name.empty())
	{
		cnsl::InsertText(ERROR_COLOR, "No name? You must specify a name!\n");
		return false;
	}
	XMLElementPtr node = GetNodeByPath(path);
	if (node)
	{
		if (IsGroup(node))
			cnsl::InsertText(ERROR_COLOR, "Group with name \"%s\" already exists!\n", path.c_str());
		else
			cnsl::InsertText(ERROR_COLOR, "Password item with name \"%s\" already exists!\n", path.c_str());
		return false;
	}
	node = CreateItemNodeByPath(path, name.c_str());
	if (node)
		cnsl::InsertText(MESSAGE_COLOR, "Password item \"%s\" created.\n", path.c_str());
	else
	{
		cnsl::InsertText(ERROR_COLOR, "Failded to create group \"%s\".\n", path.c_str());
		return false;
	}

	g_passDoc.Mark();

	return true;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** mkdir <group name>
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool MkdirCommand::Handle(const ArgListPtr args)
{
	if (!args || (args->size() != 1))
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		cnsl::InsertText(MESSAGE_COLOR, "Usage: mkdir <group name>\n");
		cnsl::InsertText(MESSAGE_COLOR, "Group name shall not contain white space. Use '-' instead.\n");
		return false;
	}

	const std::string& path = (*args)[0];
	XMLElementPtr node = GetNodeByPath(path);
	if (node)
	{
		if (IsGroup(node))
			cnsl::InsertText(ERROR_COLOR, "Group with name \"%s\" already exists!\n", path.c_str());
		else
			cnsl::InsertText(ERROR_COLOR, "Password item with name \"%s\" already exists!\n", path.c_str());
		return false;
	}
	node = CreateGroupNodeByPath(path);
	if (node)
		cnsl::InsertText(MESSAGE_COLOR, "Group \"%s\" created.\n", path.c_str());
	else
	{
		cnsl::InsertText(ERROR_COLOR, "Failded to create group \"%s\".\n", path.c_str());
		return false;
	}

	g_passDoc.Mark();

	return true;
}


bool RemoveCommand::Handle(const ArgListPtr args)
{
	if (!args || (args->size() != 1))
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		cnsl::InsertText(MESSAGE_COLOR, "Usage: rm <group name>\n");
		return false;
	}

	const std::string& path = (*args)[0];
	XMLElementPtr node;

	// Remove root or self.
	node = GetNodeByPath(path);
	if (!node)
	{
		cnsl::InsertText(ERROR_COLOR, "No such thing to delete.\n");
		return false;
	}
	else if (node == g_passDoc.GetRoot())
		return _DeleteRoot();
	else if (node == g_passDoc.GetCurrent())
		return _DeleteCurrent();

	node = GetChildNodeByPath(path);
	if (!node)
	{
		cnsl::InsertText(ERROR_COLOR, "Do not attemp to remove parent or self!\n");
		return false;
	}

	if (_Confirm("You're sure? (Y/N) $ "))
	{
		if (IsGroup(node))
			cnsl::InsertText(MESSAGE_COLOR, "Group \"%s\" and its contents are deleted permanently!\n", path.c_str());
		else
			cnsl::InsertText(MESSAGE_COLOR, "Password item \"%s\" deleted!\n", path.c_str());
		DeleteNode(node);
	}
	else
		cnsl::InsertText(MESSAGE_COLOR, "Nothing changed.\n");

	g_passDoc.Mark();

	return true;
}

bool RemoveCommand::_Confirm(const char* prompt)
{
	char buffer[4];

	cnsl::InsertText(ERROR_COLOR, prompt);
	cnsl::GetString(buffer, 1, 1);
	cnsl::InsertNewLine();

	return (tolower(buffer[0]) == 'y');
}

bool RemoveCommand::_DeleteRoot()
{
	if (_Confirm("You're sure to delete ALL passwords? (Y/N) $ "))
	{
		g_passDoc.SetCurrent(g_passDoc.GetRoot());
		g_passDoc.GetPresentWorkingDirectory(g_pwd);
		DeleteChildren(g_passDoc.GetRoot());
	}

	return true;
}

bool RemoveCommand::_DeleteCurrent()
{
	if (_Confirm("You're sure to delete current group? (Y/N) $ "))
	{
		XMLElementPtr current = g_passDoc.GetCurrent();
		XMLElementPtr parent = GetParentNode(current);
		g_passDoc.SetCurrent(parent);
		g_passDoc.GetPresentWorkingDirectory(g_pwd);
		DeleteNode(current);
	}

	return true;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** mv <src node> <dest group>
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool MoveCommand::Handle(const ArgListPtr args)
{
	if (!args || (args->size() != 2))
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		cnsl::InsertText(MESSAGE_COLOR, "Usage: mv <src group|item> <dest group>\n");
		return false;
	}

	const std::string& srcPath = (*args)[0];
	const std::string& destPath = (*args)[1];
	XMLElementPtr srcNode = GetNodeByPath(srcPath);
	XMLElementPtr destGroup = GetNodeByPath(destPath);

	if (!srcNode)
	{
		cnsl::InsertText(ERROR_COLOR, "Source doen't exist!\n");
		return false;
	}
	if (!destGroup)
	{
		cnsl::InsertText(ERROR_COLOR, "Destination doesn't exist!\n");
		return false;
	}
	if (!IsGroup(destGroup))
	{
		cnsl::InsertText(ERROR_COLOR, "Destination must be a group!\n");
		return false;
	}
	if (srcNode == destGroup)
	{
		cnsl::InsertText(ERROR_COLOR, "Source and destination must not be the same!\n");
		return false;
	}

	/*
	** 2022/01/19 TS:
	** Fixed name duplicated bug.
	*/
	if (GetDirectChildNode(destGroup, GetNodeName(srcNode)))
	{
		cnsl::InsertText(ERROR_COLOR, "Name exists in destination group!\n");
		return false;
	}

	// See tinyxml2, it will automatically move from old place.
	std::string src;
	std::string dest;
	g_passDoc.GetWorkingDirectory(srcNode, src);
	AddChildNode(destGroup, srcNode);
	g_passDoc.GetWorkingDirectory(srcNode, dest);
	cnsl::InsertText(MESSAGE_COLOR,
		"Moved \"%s\" to \"%s\".\n",
		src.c_str(), dest.c_str());

	g_passDoc.Mark();

	return true;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** save
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool SaveCommand::Handle(const ArgListPtr args)
{
	if (g_passDoc.Save(g_password.c_str()))
	{
		cnsl::InsertText(MESSAGE_COLOR, "Password successfully saved!\n");
		return true;
	}
	else
	{
		LOG_ERROR("Failed to save password!");
		cnsl::InsertText(ERROR_COLOR, "Failed to save password!\n");
		cnsl::InsertText(ERROR_COLOR, "No possible solutions!\n");
		return false;
	}
}

bool CatCommand::Handle(const ArgListPtr args)
{
	if (!args || (args->size() != 1))
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		cnsl::InsertText(MESSAGE_COLOR, "Usage: cat <item name>\n");
		cnsl::InsertNewLine();
		return false;
	}

	const std::string& path = (*args)[0];
	XMLElementPtr node;
	if (_STR_SAME(path.c_str(), ROOT_DIR_NAME))
		node = g_passDoc.GetRoot();
	else
		node = GetNodeByPath((*args)[0]);
	if (!node)
	{
		cnsl::InsertText(ERROR_COLOR, "Password item doesn't exist!\n");
		return false;
	}
	if (!IsItem(node))
	{
		cnsl::InsertText(ERROR_COLOR, "You can only reveal a password item!\n");
		cnsl::InsertText(MESSAGE_COLOR, "Usage: cat <item name>\n");
		return false;
	}

	_See(node);

	return STATUS();
}

void CatCommand::_See(XMLElementPtr item)
{
	EntryList list;

	if (!GetEntries(item, list))
	{
		cnsl::InsertText(ERROR_COLOR, "I... I can't see it!\n");
		return;
	}

	if (list.empty())
		cnsl::InsertHeaderLine("Nothing", ' ');
	else
	{
		int total = 100;
		int maxKey = 0;
		int maxValue = 0;
		int maxWeight = 0;
		for (auto& it : list)
		{
			maxKey = std::max(maxKey, (int)strlen(it.key));
			maxValue = std::max(maxValue, (int)strlen(it.value));
		}
		maxKey = std::max(maxKey, 20);
		maxValue = std::max(maxValue, 20);
		maxWeight = std::min(total - maxKey - maxValue, 12);
		cnsl::InsertText(MESSAGE_COLOR, "%4s | %*s | %*s | %*s\n",
			"ID",
			maxKey, "Key",
			maxValue, "Value",
			maxWeight, "Weight");
		int id = 0;
		for (auto& it : list)
		{
			cnsl::InsertText("%4d | %*s | %*s | %*d\n", id++,
				maxKey, it.key, maxValue, it.value, maxWeight, it.weight);
		}
	}
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** rename <
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool RenameCommand::Handle(const ArgListPtr args)
{
	if (!args || (args->size() != 2))
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		cnsl::InsertText(MESSAGE_COLOR, "Usage: rename <old> <new>\n");
		return false;
	}

	const std::string& srcPath = (*args)[0];
	const std::string& destName = (*args)[1];
	XMLElementPtr node = GetNodeByPath(srcPath);

	if (!node)
	{
		cnsl::InsertText(ERROR_COLOR, "Node doen't exist!\n");
		return false;
	}
	if (node == g_passDoc.GetRoot())
	{
		cnsl::InsertText(ERROR_COLOR, "You can not rename root!\n");
		return false;
	}
	if (!IsLegalNodeName(destName))
	{
		cnsl::InsertText(ERROR_COLOR, "Illegal node name!\n");
		return false;
	}

	XMLElementPtr parent = GetParentNode(node);
	if (GetDirectChildNode(parent, destName.c_str()))
	{
		cnsl::InsertText(ERROR_COLOR, "New name already exist!\n");
		return false;
	}
	
	std::string oldPath;
	std::string newPath;
	g_passDoc.GetWorkingDirectory(node, oldPath);
	node->SetAttribute("name", destName.c_str());
	g_passDoc.GetWorkingDirectory(node, newPath);
	
	cnsl::InsertText(MESSAGE_COLOR,
		"Rename \"%s\" to \"%s\".\n",
		oldPath.c_str(), newPath.c_str());

	g_passDoc.Mark();

	return true;
}