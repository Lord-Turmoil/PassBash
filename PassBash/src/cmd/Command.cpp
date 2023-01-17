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
	cnsl::Print();

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
		cnsl::InsertText(MESSAGE_COLOR, "Usage: ls <item name>");
		cnsl::InsertNewLine();
		return false;
	}

	XMLElementPtr current = g_passDoc.GetCurrent();
	XMLElementPtrList list;
	if (!args || args->empty())
		GetChildren(current, list);
	else
	{
		const std::string& name = (*args)[0];
		XMLElementPtr node = GetDirectChildNode(current, name.c_str());
		if (!node)
		{
			cnsl::InsertText(ERROR_COLOR, "Group doesn't exist!");
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

	const std::string& name = (*args)[0];
	XMLElementPtr current = g_passDoc.GetCurrent();
	if (GetDirectChildNode(current, name.c_str()))	// already has!
	{
		cnsl::InsertText(ERROR_COLOR, "Name already exists!");
		cnsl::InsertNewLine();
		return false;
	}

	XMLElement* node = g_passDoc.NewElement(ITEM_TAG);
	node->SetAttribute("name", name.c_str());
	AddChildNode(current, node);

	cnsl::InsertText(MESSAGE_COLOR, "Password item \"%s\" created.\n", name.c_str());

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

	const std::string& name = (*args)[0];
	CreateGroupNodeByPath(name);

	cnsl::InsertText(MESSAGE_COLOR, "Group \"%s\" created or already exists.\n", name.c_str());

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
		cnsl::InsertText(MESSAGE_COLOR, "Group name shall not contain white space. Use '-' instead.\n");
		return false;
	}

	const std::string& name = (*args)[0];
	XMLElementPtr node = GetChildNodeByPath(name);
	if (!node)
	{
		cnsl::InsertText(ERROR_COLOR, "Do not attemp to remove parent or self!\n");
		return false;
	}

	if (_Confirm())
	{
		if (IsGroup(node))
			cnsl::InsertText(MESSAGE_COLOR, "Group \"%s\" and its contents are deleted permanently!\n", name.c_str());
		else
			cnsl::InsertText(MESSAGE_COLOR, "Password item \"%s\" deleted!\n", name.c_str());
		DeleteNode(node);
	}
	else
		cnsl::InsertText(MESSAGE_COLOR, "Nothing changed.\n");

	g_passDoc.Mark();

	return true;
}

bool RemoveCommand::_Confirm()
{
	char buffer[4];

	cnsl::InsertText(ERROR_COLOR, "You're sure? (Y/N) $ ");
	cnsl::GetString(buffer, 1, 1);
	cnsl::InsertNewLine();

	return (tolower(buffer[0]) == 'y');
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