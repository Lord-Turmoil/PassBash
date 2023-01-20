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

static const char EDIT_IGNORE[] = "|";
static const int EDIT_KEY_MAX_LENGTH = 30;
static const int EDIT_VALUE_MAX_LENGTH = 45;
static const int EDIT_WEIGHT_MAX_LENGTH = 9;

void EditCommand::OnStart()
{
}

void EditCommand::OnEnd()
{
	m_item = nullptr;
}

bool EditCommand::Handle(const ArgListPtr args)
{
	if (!args || (args->size() != 1))
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		cnsl::InsertText(MESSAGE_COLOR, "Usage: mod <item name>\n");
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
		cnsl::InsertText(MESSAGE_COLOR, "Password item \"%s\" created.\n", path.c_str());
		std::string name;
		GetBaseName(path, name);
		node = CreateItemNodeByPath(path, name.c_str());
	}
	else if (!IsItem(node))
	{
		cnsl::InsertText(ERROR_COLOR, "You can only modify a password item!\n");
		cnsl::InsertText(MESSAGE_COLOR, "Usage: mod <item name>\n");
		return false;
	}
	
	m_item = node;

	cnsl::InsertHeaderLine("Edit Mode", '-');
	cnsl::InsertText(MESSAGE_COLOR, "Use \"help\" for more information.\n");
	_EditPrompt();
	cnsl::InsertNewLine();
	cnsl::InsertHeaderLine("Edit End", '-');

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

void EditCommand::_ParseType(const char* cmd, char* type)
{
	for (const char* p = cmd; *p && (*p != ' '); p++)
		*(type++) = *p;
	*type = '\0';
}

char* EditCommand::_ParseCommand(char* cmd)
{
	char* p = cmd;
	while (*p && (*p != ' '))
		p++;
	while (*p && (*p == ' '))
		p++;

	return p;
}


void EditCommand::_EditPrompt()
{
	char buffer[128];
	char type[128];
	int ret;
	
	cnsl::InsertText(PROMPT_COLOR, " %s> ", GetNodeName(m_item));
	while ((ret = cnsl::GetStringInterruptible(buffer, 0, 127)) != -1)
	{
		cnsl::InsertNewLine();
		if (ret > 0)
		{
			char* cmd = _SkipLeadingWhiteSpace(buffer);
			_ParseType(cmd, type);
			cmd = _ParseCommand(cmd);
			if (_STR_NSAME(type, "h") || _STR_NSAME(type, "help"))
				_Help();
			else if (_STR_SAME(type, "c") || _STR_SAME(type, "clear"))
				_Clear();
			else if (_STR_SAME(type, "s") || _STR_SAME(type, "see"))
				_See();
			else if (_STR_SAME(type, "st") || _STR_SAME(type, "set"))
				_SetPrompt(cmd);
			else if (_STR_SAME(type, "sk") || _STR_SAME(type, "setk"))
				_SetKeyPrompt(cmd);
			else if (_STR_SAME(type, "sv") || _STR_SAME(type, "setv"))
				_SetValuePrompt(cmd);
			else if (_STR_SAME(type, "sw") || _STR_SAME(type, "setw"))
				_SetWeightPrompt(cmd);
			else if (_STR_SAME(type, "usd") || (_STR_SAME(type, "ustid")))
				_UnSetIDPrompt(cmd);
			else if (_STR_SAME(type, "ust") || _STR_SAME(type, "unset"))
				_UnSetPrompt(cmd);
			else if (_STR_SAME(type, "quit") || _STR_SAME(type, "q"))
				break;
			else
				_UnRecognized(type);
		}
			
		cnsl::InsertText(PROMPT_COLOR, " %s> ", GetNodeName(m_item));
	}
}

void EditCommand::_Help()
{
	WORD old = cnsl::SetTextForeground(MESSAGE_COLOR);

	cnsl::InsertHeaderLine("Edit Help", '_');
	cnsl::InsertNewLine();
	cnsl::InsertText(" Tips: There shall not be extra space around '|'\n\n");
	cnsl::InsertText("         help, h -- show help\n\n");
	cnsl::InsertText("        clear, c -- clear screen\n\n");
	cnsl::InsertText("          see, s -- show current password item\n\n");
	cnsl::InsertText("         set, st -- create or overwrite entry\n");
	cnsl::InsertText("                    usage: set key|value[|weight=auto]\n\n");
	cnsl::InsertText("        setk, sk -- reset key field, old-key must exist\n");
	cnsl::InsertText("                    usage: setk old-key|new-key\n\n");
	cnsl::InsertText("        setv, sv -- reset value field, key must exist\n");
	cnsl::InsertText("                    usage: setv key|value\n\n");
	cnsl::InsertText("        setw, sw -- reset weight field, key must exist\n");
	cnsl::InsertText("                    usage: setw key|weight\n\n");
	cnsl::InsertText("      unset, ust -- delete entry, key must exist\n");
	cnsl::InsertText("                    usage: unset key\n\n");
	cnsl::InsertText("      ustid, usd -- delete entry by id, id must exist\n");
	cnsl::InsertText("                    usage: ustid id\n\n");
	cnsl::InsertText("  quit, q, <ESC> -- quit edit mode\n");
	cnsl::InsertSplitLine('_');
	cnsl::InsertNewLine();

	cnsl::SetTextForeground(old);
}

void EditCommand::_Clear()
{
	cnsl::Clear();
	// cnsl::Print();
}

void EditCommand::_See()
{
	_See(nullptr, 0);
}

void EditCommand::_See(const char* key, WORD color)
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
			if (key && _STR_SAME(it.key, key))
			{
				cnsl::InsertText(color, "%4d | %*s | %*s | %*d\n", id++,
					maxKey, it.key, maxValue, it.value, maxWeight, it.weight);
			}
			else
			{
				cnsl::InsertText("%4d | %*s | %*s | %*d\n", id++,
					maxKey, it.key, maxValue, it.value, maxWeight, it.weight);
			}
		}
	}
}

// cmd[set key:value:weight]
void EditCommand::_SetPrompt(char* cmd)
{
	char* context = nullptr;
	char* token = strtok_s(cmd, EDIT_IGNORE, &context);
	const char* args[3]{ nullptr, nullptr, "-1" };
	int pos;
	for (pos = 0; pos < 3; pos++)
	{
		if (token)
		{
			args[pos] = token;
			token = strtok_s(nullptr, EDIT_IGNORE, &context);
		}
		else
			break;
	}
	if (pos < 2)
	{
		// Arguments illegal!
		cnsl::InsertText(ERROR_COLOR, "Usage: set key|value[|weight=auto]\n");
		return;
	}

	_SetEntry(args[0], args[1], args[2]);
}

// setk old-key:new-key
void EditCommand::_SetKeyPrompt(char* cmd)
{
	char* context = nullptr;
	char* token = strtok_s(cmd, EDIT_IGNORE, &context);
	const char* args[2]{ nullptr, nullptr };
	int pos;
	for (pos = 0; pos < 2; pos++)
	{
		if (token)
		{
			args[pos] = token;
			token = strtok_s(nullptr, EDIT_IGNORE, &context);
		}
		else
			break;
	}
	if (pos != 2)
	{
		// Arguments illegal!
		cnsl::InsertText(ERROR_COLOR, "Usage: setk old-key|new-key\n");
		return;
	}

	_SetKey(args[0], args[1]);
}

// setv key:value
void EditCommand::_SetValuePrompt(char* cmd)
{
	char* context = nullptr;
	char* token = strtok_s(cmd, EDIT_IGNORE, &context);
	const char* args[2]{ nullptr, nullptr };
	int pos;
	for (pos = 0; pos < 2; pos++)
	{
		if (token)
		{
			args[pos] = token;
			token = strtok_s(nullptr, EDIT_IGNORE, &context);
		}
		else
			break;
	}
	if (pos != 2)
	{
		// Arguments illegal!
		cnsl::InsertText(ERROR_COLOR, "Usage: set key|value\n");
		return;
	}

	_SetEntry(args[0], args[1], nullptr);
}

// setw key:weight
void EditCommand::_SetWeightPrompt(char* cmd)
{
	char* context = nullptr;
	char* token = strtok_s(cmd, EDIT_IGNORE, &context);
	const char* args[2]{ nullptr, nullptr };
	int pos;
	for (pos = 0; pos < 2; pos++)
	{
		if (token)
		{
			args[pos] = token;
			token = strtok_s(nullptr, EDIT_IGNORE, &context);
		}
		else
			break;
	}
	if (pos != 2)
	{
		// Arguments illegal!
		cnsl::InsertText(ERROR_COLOR, "Usage: set key|weight\n");
		return;
	}

	_SetEntry(args[0], nullptr, args[1]);
}

bool EditCommand::_SetKey(const char* oldKey, const char* newKey)
{
	XMLElementPtr entry = GetEntryNode(m_item, oldKey);
	if (!entry)
	{
		cnsl::InsertText(ERROR_COLOR, "Entry with key \"%s\" doesn't exists!\n", oldKey);
		return false;
	}

	if (oldKey && (strlen(oldKey) > EDIT_KEY_MAX_LENGTH))
	{
		cnsl::InsertText(ERROR_COLOR,
			"Old key is too long! No longer than %d characters!\n",
			EDIT_KEY_MAX_LENGTH);
		return false;
	}
	if (newKey)
	{
		if (strlen(newKey) > 30)
		{
			cnsl::InsertText(ERROR_COLOR,
				"New key is too long! No longer than %d characters!\n",
				EDIT_KEY_MAX_LENGTH);
			return false;
		}
		entry->SetAttribute("key", newKey);
	}

	_See(newKey, ENTRY_MODIFY_COLOR);

	return true;
}

bool EditCommand::_SetEntry(const char* key, const char* value, const char* weightStr)
{
	if (!key)
		return false;

	XMLElementPtr entry;
	if (key && value && weightStr)	// can create
		entry = GetOrCreateEntryNode(m_item, key);
	else
		entry = GetEntryNode(m_item, key);
	if (!entry)
	{
		cnsl::InsertText(ERROR_COLOR, "Entry with key \"%s\" doesn't exists!\n", key);
		return false;
	}

	if (key && (strlen(key) > EDIT_KEY_MAX_LENGTH))
	{
		cnsl::InsertText(ERROR_COLOR,
			"Key is too long! No longer than %d characters!\n",
			EDIT_KEY_MAX_LENGTH);
		return false;
	}
	if (value)
	{
		if (strlen(value) > EDIT_VALUE_MAX_LENGTH)
		{
			cnsl::InsertText(ERROR_COLOR,
				"Value is too long! No longer than %d characters!\n",
				EDIT_VALUE_MAX_LENGTH);
			return false;
		}
		entry->SetAttribute("value", value);
	}
	if (weightStr)
	{
		if (strlen(weightStr) > EDIT_WEIGHT_MAX_LENGTH)
		{
			cnsl::InsertText(ERROR_COLOR,
				"Key is too long! No longer than %d characters!\n",
				EDIT_WEIGHT_MAX_LENGTH);
			return false;
		}
		int weight = 0;
		int w;
		if (tinyxml2::XMLUtil::ToInt(weightStr, &w))
			weight = w;
		if (weight == -1)	// auto
		{
			EntryList list;
			GetEntries(m_item, list);
			weight = list.back().weight + 4;
		}
		entry->SetAttribute("weight", weight);
	}

	_See(key, ENTRY_MODIFY_COLOR);

	return true;
}

void EditCommand::_UnSetIDPrompt(char* cmd)
{
	char* context = nullptr;
	char* token = strtok_s(cmd, EDIT_IGNORE, &context);
	if (!token)
	{
		// Arguments illegal!
		cnsl::InsertText(ERROR_COLOR, "Usage: ustid id\n");
		return;
	}

	int id;
	bool flag = false;
	EntryList list;
	if (tinyxml2::XMLUtil::ToInt(token, &id))
	{
		if (id >= 0)
		{
			GetEntries(m_item, list);
			if (id < list.size())
				flag = true;
		}
	}

	if (flag)
		_UnSet(list[id].key);
	else
		cnsl::InsertText(ERROR_COLOR, "Invalid ID! Use see to check all available IDs.\n");
}

void EditCommand::_UnSetPrompt(char* cmd)
{
	char* context = nullptr;
	char* token = strtok_s(cmd, EDIT_IGNORE, &context);
	if (!token)
	{
		// Arguments illegal!
		cnsl::InsertText(ERROR_COLOR, "Usage: unset key\n");
		return;
	}

	_UnSet(token);
}

void EditCommand::_UnSet(const char* key)
{
	XMLElementPtr entry = GetEntryNode(m_item, key);
	if (!entry)
		cnsl::InsertText(ERROR_COLOR, "Entry with key \"%s\" doesn't exists!\n", key);
	else
	{
		_See(key, ENTRY_DELETE_COLOR);
		DeleteNode(entry);
	}
}

void EditCommand::_UnRecognized(const char* cmd)
{
	cnsl::InsertText(ERROR_COLOR, "\"%s\" is not an edit command.\n", cmd);
	cnsl::InsertText(MESSAGE_COLOR, "Use \"help\" for more information.\n");
}