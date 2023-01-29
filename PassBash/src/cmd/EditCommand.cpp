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
 *                    Last Update : January 25, 2023                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For password edit.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/Editor.h"
#include "../../inc/cmd/FunctionUtil.h"

static const char EDIT_IGNORE[] = "|";
static const int EDIT_KEY_MAX_LENGTH = 30;
static const int EDIT_VALUE_MAX_LENGTH = 45;
static const int EDIT_WEIGHT_MAX_LENGTH = 9;

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** edit <item name>
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

static void _edit_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: mod <item name>\n");
}

static int _edit_parse_args(int argc, char* argv[], std::string& path)
{
	return _parse_args(argc, argv, path);
}

// The item to edit.
static XMLElementPtr item;
static std::string item_path;

DEC_CMD(edit)
{
	std::string path;

	if (_edit_parse_args(argc, argv, path) != 0)
	{
		_edit_usage();
		return 1;
	}

	XMLElementPtr node = GetNodeByPath(path);
	if (!node)
	{
		cnsl::InsertText(ERROR_COLOR, "Password item doesn't exist!\n");
		node = CreateItemNodeByPath(path);
		cnsl::InsertText(MESSAGE_COLOR, "Password item \"%s\" created.\n",
			GetNodeDirectory(node, path));
	}
	else if (!IsItem(node))
	{
		cnsl::InsertText(ERROR_COLOR, "You can only modify a password item!\n");
		cnsl::InsertText(MESSAGE_COLOR, "Usage: mod <item name>\n");
		return 2;
	}
	
	item = node;
	
	if (g_editorFactory.execl("_edit", nullptr) != 0)
	{
		cnsl::InsertText(ERROR_COLOR, "Failed to launch password editor!\n");
		return 3;
	}

	g_passDoc.Mark();

	return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** edit host
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static char* _edit_strip_white_space(char* buffer)
{
	int length = (int)strlen(buffer);
	char* begin = buffer;
	char* end = buffer + length;

	while (begin < end)
	{
		if (isspace(*begin))
			begin++;
		else
			break;
	}
	if (begin == end)	// all is space
		return nullptr;
	
	while (begin < --end)
	{
		if (isspace(*end))
			end--;
		else
			break;
	}
	*(end + 1) = '\0';

	return begin;
}

static void _edit_parse_cmd(char* cmd, char*& type, char*& arg)
{
	char* p = cmd;
	while (*p && isspace(*p))
		p++;
	type = p;
	while (*p && !isspace(*p))
		p++;

	while (*p && isspace(*p))
		*(p++) = '\0';
	arg = p;
	
	p = cmd + strlen(cmd) - 1;
	while (p > arg && isspace(*p))
		p--;
	*(p + 1) = '\0';
}

static void _edit_print_prompt()
{
	cnsl::InsertText("[Edit Mode]");
	cnsl::InsertText(PWD_COLOR, "%s", item_path.c_str());
	cnsl::InsertText(PROMPT_COLOR, "> ");
}

DEC_CMD(_edit)
{
	// begin
	cnsl::InsertHeaderLine("Password Editor 1.1.0", '-');
	cnsl::InsertText(MESSAGE_COLOR, "Use \"help\" for more information.\n");

	// main loop
	char buffer[CMD_BUFFER_SIZE + 1];
	int ret;
	GetNodeDirectory(item, item_path);

	_edit_print_prompt();
	while ((ret = cnsl::GetStringInterruptible(buffer, 0, CMD_BUFFER_SIZE)) != -1)
	{
		cnsl::InsertNewLine();
		if (ret > 0)
		{
			char* cmd = _edit_strip_white_space(buffer);
			char* type;
			char* arg;
			_edit_parse_cmd(cmd, type, arg);

			int ret = g_editorFactory.execl(type, type, arg, nullptr);
			if (ret == -1)
				g_hiddenFactory.execl("_edit_unknown", "_edit_unknown", type, nullptr);
			else if (ret == 66)
				break;
			else if (ret != 0)
				LOG_ERROR("Editor \"%s\" -- Error Code: %d", type, ret);
		}
		_edit_print_prompt();
	}

	//end
	cnsl::InsertNewLine();
	cnsl::InsertHeaderLine("Edit End", '-');

	return 0;
}

// help
DEC_CMD(_edit_help)
{
	WORD old = cnsl::SetTextForeground(MESSAGE_COLOR);

	cnsl::InsertHeaderLine("Edit Help", '_');
	cnsl::InsertNewLine();
	cnsl::InsertText(" Tips: There shall not be extra space around '|'\n\n");
	cnsl::InsertText("         help, h -- show help\n\n");
	cnsl::InsertText("   clear, cls, c -- clear screen\n\n");
	cnsl::InsertText("          see, s -- show current password item\n\n");
	cnsl::InsertText("         set, st -- create or overwrite entry\n");
	cnsl::InsertText("                    usage: set key|value[|weight=auto]\n\n");
	cnsl::InsertText("        setk, sk -- reset key field, old-key must exist\n");
	cnsl::InsertText("                    usage: setk id|new-key\n\n");
	cnsl::InsertText("        setv, sv -- reset value field, key must exist\n");
	cnsl::InsertText("                    usage: setv id|value\n\n");
	cnsl::InsertText("        setw, sw -- reset weight field, key must exist\n");
	cnsl::InsertText("                    usage: setw id|weight\n\n");
	cnsl::InsertText("      unset, ust -- delete entry, key must exist\n");
	cnsl::InsertText("                    usage: unset id\n\n");
	cnsl::InsertText("  quit, q, <ESC> -- quit edit mode\n");
	cnsl::InsertSplitLine('_');
	cnsl::InsertNewLine();

	cnsl::SetTextForeground(old);

	return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** clear
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DEC_CMD(_edit_clear)
{
	cnsl::Clear();

	return 0;
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** see
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DEC_CMD(_edit_see)
{
	_show_item(item, nullptr);
	
	return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Utils
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static int _set_key(const char* idStr, const char* newKey)
{
	int id;
	if (!tinyxml2::XMLUtil::ToInt(idStr, &id))
	{
		cnsl::InsertText(ERROR_COLOR, "Invalid id format!\n");
		return 1;
	}

	XMLElementPtr entry = GetEntryNode(item, id);
	if (!entry)
	{
		cnsl::InsertText(ERROR_COLOR, "Entry with ID \"%d\" doesn't exist!\n", id);
		return 2;
	}

	if (strlen(newKey) > EDIT_KEY_MAX_LENGTH)
	{
		cnsl::InsertText(ERROR_COLOR,
			"New key is too long! No longer than %d characters!\n",
			EDIT_KEY_MAX_LENGTH);
		return 3;
	}
	entry->SetAttribute("key", newKey);

	_show_item(item, newKey, ENTRY_MODIFY_COLOR);

	return 0;
}

static int _set_value(const char* idStr, const char* value)
{
	int id;
	if (!tinyxml2::XMLUtil::ToInt(idStr, &id))
	{
		cnsl::InsertText(ERROR_COLOR, "Invalid id format!\n");
		return 1;
	}

	XMLElementPtr entry = GetEntryNode(item, id);
	if (!entry)
	{
		cnsl::InsertText(ERROR_COLOR, "Entry with ID \"%d\" doesn't exist!\n", id);
		return 2;
	}

	if (strlen(value) > EDIT_VALUE_MAX_LENGTH)
	{
		cnsl::InsertText(ERROR_COLOR,
			"Value is too long! No longer than %d characters!\n",
			EDIT_VALUE_MAX_LENGTH);
		return 3;
	}
	entry->SetAttribute("value", value);

	_show_item(item, entry->Attribute("key"), ENTRY_MODIFY_COLOR);

	return 0;
}

static int _set_weight(const char* idStr, const char* weightStr)
{
	int id;
	if (!tinyxml2::XMLUtil::ToInt(idStr, &id))
	{
		cnsl::InsertText(ERROR_COLOR, "Invalid id format!\n");
		return 1;
	}
	
	if (strlen(weightStr) > EDIT_WEIGHT_MAX_LENGTH)
	{
		cnsl::InsertText(ERROR_COLOR,
			"Weight is too long! No longer than %d characters!\n",
			EDIT_VALUE_MAX_LENGTH);
		return 2;
	}
	int weight;
	if (!tinyxml2::XMLUtil::ToInt(weightStr, &weight))
	{
		cnsl::InsertText(ERROR_COLOR, "Invalid weight format!\n");
		return 3;
	}

	XMLElementPtr entry = GetEntryNode(item, id);
	if (!entry)
	{
		cnsl::InsertText(ERROR_COLOR, "Entry with ID \"%d\" doesn't exist!\n", id);
		return 4;
	}
	entry->SetAttribute("weight", weight);

	_show_item(item, entry->Attribute("key"), ENTRY_MODIFY_COLOR);

	return 0;
}

static int _set_entry(const char* key, const char* value, const char* weightStr)
{
	if (!key)
	{
		cnsl::InsertText(ERROR_COLOR, "Missing entry key!\n");
		return 1;
	}

	XMLElementPtr entry;
	if (key && value && weightStr)	// can create
		entry = GetOrCreateEntryNode(item, key);
	else
		entry = GetEntryNode(item, key);
	if (!entry)
	{
		cnsl::InsertText(ERROR_COLOR, "Entry with key \"%s\" doesn't exist!\n", key);
		return 2;
	}

	if (key && (strlen(key) > EDIT_KEY_MAX_LENGTH))
	{
		cnsl::InsertText(ERROR_COLOR,
			"Key is too long! No longer than %d characters!\n",
			EDIT_KEY_MAX_LENGTH);
		return 3;
	}
	if (value)
	{
		if (strlen(value) > EDIT_VALUE_MAX_LENGTH)
		{
			cnsl::InsertText(ERROR_COLOR,
				"Value is too long! No longer than %d characters!\n",
				EDIT_VALUE_MAX_LENGTH);
			return 4;
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
			return 5;
		}
		int weight = -1;
		int w;
		if (tinyxml2::XMLUtil::ToInt(weightStr, &w))
			weight = w;
		if (weight == -1)	// auto
		{
			EntryList list;
			GetEntries(item, list);
			if (list.empty())
				weight = 4;
			else
				weight = list.back().weight + 4;
		}
		entry->SetAttribute("weight", weight);
	}

	_show_item(item, key, ENTRY_MODIFY_COLOR);

	return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** set
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static int _edit_parse_params(char* arg, int argc, const char* params[])
{
	if (arg == nullptr)
		return 0;

	char* context = nullptr;
	char* token = strtok_s(arg, EDIT_IGNORE, &context);
	int cnt = 0;
	while (token)
	{
		cnt++;
		if (cnt <= argc)
			params[cnt - 1] = token;
		token = strtok_s(nullptr, EDIT_IGNORE, &context);
	}
	
	return cnt;
}

// set key|value|weight
static void _set_usage()
{
	cnsl::InsertText(ERROR_COLOR, "Usage: set key|value[|weight=auto]\n");
}

DEC_CMD(_edit_set)
{
	const char* params[3] = { nullptr, nullptr, "-1" };
	int ret = _edit_parse_params(argv[1], 3, params);

	if (ret < 2 || ret > 3)
	{
		_set_usage();
		return 1;
	}

	return _set_entry(params[0], params[1], params[2]);
}


// setk id|newKey
static void _setk_usage()
{
	cnsl::InsertText(ERROR_COLOR, "Usage: setk id|new-key\n");
}

DEC_CMD(_edit_setk)
{
	const char* params[2] = { nullptr, nullptr };
	int ret = _edit_parse_params(argv[1], 2, params);

	if (ret != 2)
	{
		_setk_usage();
		return 1;
	}

	return _set_key(params[0], params[1]);
}


// setv id|value
static void _setv_usage()
{
	cnsl::InsertText(ERROR_COLOR, "Usage: setv id|value\n");
}

DEC_CMD(_edit_setv)
{
	const char* params[2] = { nullptr, nullptr };
	int ret = _edit_parse_params(argv[1], 2, params);

	if (ret != 2)
	{
		_setv_usage();
		return 1;
	}

	return _set_value(params[0], params[1]);
}


// setw id|weight
static void _setw_usage()
{
	cnsl::InsertText(ERROR_COLOR, "Usage: setw key|weight\n");
}

DEC_CMD(_edit_setw)
{
	const char* params[2] = { nullptr, nullptr };
	int ret = _edit_parse_params(argv[1], 2, params);

	if (ret != 2)
	{
		_setw_usage();
		return 1;
	}

	return _set_weight(params[0], params[1]);
}

// unset id
static void _unset_usage()
{
	cnsl::InsertText(ERROR_COLOR, "Usage: ustid id\n");
}

DEC_CMD(_edit_unset)
{
	const char* params[1] = { nullptr };
	int ret = _edit_parse_params(argv[1], 1, params);

	if (ret != 1)
	{
		_unset_usage();
		return 1;
	}

	int id;
	if (!tinyxml2::XMLUtil::ToInt(params[0], &id))
	{
		cnsl::InsertText("Invalid ID format!\n");
		return 2;
	}
	XMLElementPtr entry = GetEntryNode(item, id);
	if (!entry)
	{
		cnsl::InsertText(ERROR_COLOR, "Entry with ID \"%d\" doesn't exist!\n", id);
		return 3;
	}

	_show_item(item, entry->Attribute("key"), ENTRY_DELETE_COLOR);
	DeleteNode(entry);

	return 0;
}


// missing
DEC_CMD(_edit_unknown)
{
	cnsl::InsertText(ERROR_COLOR, "\"%s\" is not an edit command.\n", argv[1]);
	cnsl::InsertText(MESSAGE_COLOR, "Use \"help\" for more information.\n");
	
	return 0;
}


// quit
DEC_CMD(_edit_quit)
{
	return 66;
}