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

#include "../../inc/cmd/CommandHeader.h"

#include <regex>


void FindCommand::OnStart()
{
	m_deep = false;
	m_strict = false;
	m_pattern = "";
}

bool FindCommand::Handle(const ArgListPtr args)
{
	bool argumentIllegal = false;
	if (!args)
		argumentIllegal = true;
	else
	{
		for (auto it = args->begin(); it != args->end(); it++)
		{
			if (!m_deep && (*it) == "-d")
				m_deep = true;
			if (!m_strict && (*it) == "-s")
				m_strict = true;
			else
				m_pattern = (*it);
		}
	}
	if (m_pattern == "")
		argumentIllegal = true;
	if (argumentIllegal)
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		cnsl::InsertText(MESSAGE_COLOR, "Usage: find [options] <regular expression>\n");
		cnsl::InsertText(MESSAGE_COLOR, "  -d -- enable deep search inside password item\n");
		cnsl::InsertText(MESSAGE_COLOR, "  -s -- enable strict search with regex, no \".*\" added around\n");
		return false;
	}

	XMLElementPtrList list;
	_Find(list);

	if (list.empty())
		cnsl::InsertText(MESSAGE_COLOR, "Nothing...\n");
	else
	{
		std::string path;
		for (auto it : list)
		{
			cnsl::InsertText(IsGroup(it) ? GROUP_COLOR : ITEM_COLOR,
				"\t%s\n", GetNodePath(it, path));
		}
	}

	return true;
}

static std::regex pattern;

void FindCommand::_Find(XMLElementPtrList& list)
{
	if (m_strict)
		pattern.assign(m_pattern, std::regex::icase);
	else
		pattern.assign(".*" + m_pattern + ".*", std::regex::icase);
	_Search(g_passDoc.GetCurrent(), list);
}

void FindCommand::_Search(XMLElementPtr root, XMLElementPtrList& list)
{
	XMLElementPtr it = root->FirstChildElement();
	while (it)
	{	
		if (IsGroup(it))
		{
			if (std::regex_match(GetNodeName(it), pattern))
				list.push_back(it);
			_Search(it, list);
		}
		else
			_SearchItem(it, list);

		it = it->NextSiblingElement();
	}
}

void FindCommand::_SearchItem(XMLElementPtr root, XMLElementPtrList& list)
{
	if (!IsItem(root))
		return;

	if (std::regex_match(GetNodeName(root), pattern))
	{
		list.push_back(root);
		return;
	}

	if (!m_deep)
		return;

	XMLElementPtr it = root->FirstChildElement();
	while (it)
	{
		if (std::regex_match(GetNodeAttr(it, "key"), pattern))
		{
			list.push_back(root);
			return;
		}
		it = it->NextSiblingElement();
	}
}