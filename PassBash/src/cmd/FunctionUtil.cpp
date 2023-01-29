/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : FunctionUtil.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 25, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Some utility functions.                                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/FunctionUtil.h"

char* optarg;
const char* optmsg;
int opterr;
int optopt;
static int optind;

void resetopt()
{
	optind = 0;
	opterr = 0;
	optarg = nullptr;
	optmsg = nullptr;
}

static int _parseopt(const char* arg)
{
	if (arg[0] == '-' && arg[1] != '\0')
		return arg[1];
	else
		return 0;
}

// parse arg in opt. e.g. pthread from -lpthread
// Must checked by _parseopt first. 
static char* _parsearg(char* arg)
{
	if (arg[2] != '\0')
		return &arg[2];
	else
		return nullptr;
}

static inline void _initopt()
{
	optarg = nullptr;
	optmsg = nullptr;
	opterr = 0;
	optopt = '?';
	optind++;
}

int getopt(int argc, char* argv[], const char* pattern)
{
	_initopt();

	if (optind >= argc)
	{
		resetopt();
		return 0;
	}

	int opt = _parseopt(argv[optind]);
	if (!opt)	// not an option
	{
		opt = optopt = '!';
		optarg = argv[optind];
	}
	else		// is an option
	{
		optopt = opt;

		auto pos = strchr(pattern, opt);
		if (!pos)	// not valid option
		{
			opt = '?';
			opterr = 1;
			optmsg = "illegal argument";
		}
		else
		{
			if (pos[1] == ':')	// has argument
			{
				optarg = _parsearg(argv[optind]);
				if (!optarg)	// argument not compact
				{
					if (((optind < argc - 1) && _parseopt(argv[optind + 1])) || \
						(optind == argc - 1))
					{
						opterr = 2;
						optmsg = "missing argument";
					}
					else
						optarg = argv[++optind];
				}
			}
		}
	}

	return opt;
}

int _parse_optional_args(int argc, char* argv[], std::string& _1)
{
	int opt;
	bool flag = false;
	bool err = false;
	int tag = false;
	while (opt = getopt(argc, argv, ""))
	{
		switch (opt)
		{
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
				_1 = optarg;
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

int _parse_args(int argc, char* argv[], std::string& _1)
{
	int opt;
	bool flag = false;
	bool err = false;
	bool tag = false;
	while (opt = getopt(argc, argv, ""))
	{
		switch (opt)
		{
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
				_1 = optarg;
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

	if (!flag)	// no args
		return 2;

	return 0;
}

int _parse_args(int argc, char* argv[], std::string& _1, std::string& _2)
{
	int opt;
	int cnt = 0;
	bool err = false;
	bool tag = false;
	while (opt = getopt(argc, argv, ""))
	{
		switch (opt)
		{
		case '!':
			cnt++;
			if (cnt > 2)
				err = true;
			else
			{
				if (cnt == 1)
					_1 = optarg;
				else
					_2 = optarg;
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

	if (cnt < 2)
		cnsl::InsertText(ERROR_COLOR, "Too few arguments!\n");
	else if (cnt > 2)
		cnsl::InsertText(ERROR_COLOR, "Too many arguments!\n");

	if (err)
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		return 1;
	}

	return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Common operations.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
int _show_item(XMLElementPtr node, const char* key, WORD color)
{
	EntryList list;
	if (!GetEntries(node, list))
	{
		cnsl::InsertText(ERROR_COLOR, "I... I can't see it!\n");
		return 1;
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

	return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Password operations.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void _format_password(char* password)
{
	char* p;
	for (p = password; *p; p++)
		continue;
	while (p - password < g_PASSWORD_LENGTH)
		*(p++) = '`';
	*p = '\0';
}

void _format_password(const char* buffer, char* password)
{
	memset(password, 0, sizeof(char) * g_PASSWORD_LENGTH);
	const char* p;
	char* q = password;
	for (p = buffer; *p; p++)
		*(q++) = *p;
	while (q - password < g_PASSWORD_LENGTH)
		*(q++) = '`';
	*q = '\0';
}