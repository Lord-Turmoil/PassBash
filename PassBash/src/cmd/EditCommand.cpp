/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : EditCommand.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For password edit.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/CommandHeader.h"


void EditCommand::OnStart()
{
	m_item = nullptr;
}

bool EditCommand::Handle(const ArgListPtr args)
{
	if (!args || (args->size() != 1))
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		cnsl::InsertText(MESSAGE_COLOR, "Usage: mod <item name>");
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
		cnsl::InsertText(ERROR_COLOR, "You can only modify a password item!\n");
		cnsl::InsertText(MESSAGE_COLOR, "Usage: mod <item name>\n");
		return false;
	}
	
	m_item = node;

	cnsl::InsertHeaderLine("Edit Mode", '-');
	_EditPrompt();

	g_passDoc.Mark();

	return STATUS();
}

char* EditCommand::_SkipLeadingWhiteSpace(char* buffer)
{
	for (char* p = buffer; *p; p++)
	{
		if (isprint(*p))
			return p;
	}

	return nullptr;
}

void EditCommand::_EditPrompt()
{
	char buffer[128];
	int ret;
	
	cnsl::InsertText(PROMPT_COLOR, " %s> ", GetNodeName(m_item));
	while ((ret = cnsl::GetStringInterruptable(buffer, 0, 127)) != -1)
	{
		cnsl::InsertNewLine();
		if (ret > 0)
		{
			char* cmd = _SkipLeadingWhiteSpace(buffer);
			if (_STR_NSAME(cmd, "help") || _STR_NSAME(cmd, "h"))
				_Help();
			else if (_STR_SAME(cmd, "clear") || _STR_SAME(cmd, "c"))
				_Clear();
			else if (_STR_SAME(cmd, "see") || _STR_SAME(cmd, "s"))
				_See();
			else if (_STR_NSAME(cmd, "set") || _STR_NSAME(cmd, "st"))
				_SetPrompt(cmd);
			else if (_STR_NSAME(cmd, "setk") || _STR_NSAME(cmd, "sk"))
				_SetKeyPrompt(cmd);
			else if (_STR_NSAME(cmd, "setv") || _STR_NSAME(cmd, "sv"))
				_SetValuePrompt(cmd);
			else if (_STR_NSAME(cmd, "setw") || _STR_NSAME(cmd, "sw"))
				_SetWeightPrompt(cmd);
			else if (_STR_NSAME(cmd, "unset") || _STR_NSAME(cmd, "u"))
				_UnSetPrompt(cmd);
			else if (_STR_SAME(cmd, "quit") || _STR_SAME(cmd, "q"))
				return;
			else
				_UnRecognized(cmd);
		}
			
		cnsl::InsertText(PROMPT_COLOR, " %s> ", GetNodeName(m_item));
	}

	cnsl::InsertNewLine();
}

void EditCommand::_Help()
{
	WORD old = cnsl::SetTextForeground(MESSAGE_COLOR);

	cnsl::InsertHeaderLine("Edit Help", '_');
	cnsl::InsertNewLine();
	cnsl::InsertText("         help, h -- show help\n\n");
	cnsl::InsertText("        clear, c -- clear screen\n\n");
	cnsl::InsertText("             see -- show current password item\n\n");
	cnsl::InsertText("             set -- create or overwrite entry\n");
	cnsl::InsertText("                    usage(no space): set:key:value:weight\n\n");
	cnsl::InsertText("            setk -- reset key field, old-key must exist\n");
	cnsl::InsertText("                    usage(no space): setk:old-key:new-key\n\n");
	cnsl::InsertText("            setv -- reset value field, key must exist\n");
	cnsl::InsertText("                    usage(no space): set:key:value\n\n");
	cnsl::InsertText("            setw -- reset weight field, key must exist\n");
	cnsl::InsertText("                    usage(no space): set:key:weight\n\n");
	cnsl::InsertText("           unset -- delete entry, key must exist\n");
	cnsl::InsertText("                    usage(no space): unset:key\n");
	cnsl::InsertText("  quit, q, <ESC> -- quit edit mode\n");
	cnsl::InsertSplitLine('_');

	cnsl::SetTextForeground(old);
}

void EditCommand::_Clear()
{
	cnsl::Clear();
	cnsl::Print();
}

void EditCommand::_See()
{
	EntryList list;

	if (!GetEntries(m_item, list))
	{
		cnsl::InsertText(ERROR_COLOR, "I... I can't see it!\n");
		return;
	}

	if (list.empty())
		cnsl::InsertHeaderLine("Nothing", ' ');
	else
	{
		cnsl::InsertText(MESSAGE_COLOR, "%20s | %20s | %s\n", "Key", "Value", "Weight");
		for (auto& it : list)
			cnsl::InsertText("%20s | %20s | %-6d\n", it.key, it.value, it.weight);
	}
}

void EditCommand::_SetPrompt(char* cmd) {}
void EditCommand::_SetKeyPrompt(char* cmd) {}
void EditCommand::_SetValuePrompt(char* cmd) {}
void EditCommand::_SetWeightPrompt(char* cmd) {}
void EditCommand::_SetKey(const char* oldKey, const char* newKey) {}
void EditCommand::_SetEntry(const char* key, const char* value, int* weight) {}
void EditCommand::_UnSetPrompt(const char* cmd) {}
void EditCommand::_UnSet(const char* key) {}

void EditCommand::_UnRecognized(const char* cmd)
{
	cnsl::InsertText(ERROR_COLOR, "\"%s\" is not an edit command.\n", cmd);
	cnsl::InsertText(MESSAGE_COLOR, "Use \"help\" for more information.\n");
}