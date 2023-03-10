/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : Console                                   *
 *                                                                            *
 *                      File Name : Input.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 1, 2023                           *
 *                                                                            *
 *                    Last Update : March 10, 2023                            *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For input control.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../inc/Input.h"
#include "../inc/Common.h"

#pragma warning(disable: 4996)

#include <ctime>
#include <cctype>
#include <cstring>
#include <vector>
#include <string>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

_CNSL_BEGIN

static const char INPUT_TERMINATOR[] = "\n\r";
char buffer[INPUT_BUFFER_SIZE];

InputHistory::InputHistory()
{
	_record.push_back(_strdup(""));
}

InputHistory::~InputHistory()
{
	for (auto it : _record)
		delete it;
}

void InputHistory::Push(const char* history, size_t pos)
{
	if (!((_record.size() > 1) && (strcmp(*(_record.end() - 2), history) == 0)))
		_record.insert(_record.end() - 1, _strdup(history));
}

static bool IsTerminator(char ch)
{
	for (const char* p = INPUT_TERMINATOR; *p; p++)
	{
		if (*p == ch)
			return true;
	}

	return false;
}

static bool IsNullOrEmpty(char* str)
{
	return !str || !strlen(str);
}

static void ResetInput(SHORT beginX, int maxLen, int& pos, int& length, char* input, char* history)
{
	if (!history)
		return;

	cnsl::Clear(beginX, beginX + length);
	cnsl::InsertText("%.*s", maxLen, history);
	length = std::min(maxLen, (int)strlen(history));
	pos = length;
	
	strncpy(input, history, length);
	input[length] = '\0';
}

int GetString(char* buffer, const InputOptions& options)
{
	int minLen = std::max(options.minLen, 0);
	int maxLen = std::max(options.maxLen, INPUT_BUFFER_SIZE - 1);
	char decoy = options.decoy;
	bool interruptible = options.interruptible;

	char** history_end = nullptr;
	char** history_begin = nullptr;
	char** history = nullptr;
	if (options.history)
	{
		auto raw = options.history->_record.data();
		history_end = raw;
		history_begin = raw + (options.history->_record.size() - 1);
		history = history_begin;
	}

	int length = 0;
	int pos = 0;
	char ch;

	COORD origin = GetCursorPosition();

	FlushInput();

	buffer[0] = '\0';
	for (; ;)
	{
		ch = _getch();
		if (IsTerminator(ch))
		{
			if (length >= minLen)
				break;	// goto normal exit (keep history)
			else if (interruptible)
				return length;	// abandon history
		}
		else if ((ch == ESCAPE) && interruptible)
		{
			return -1;
		}
		else if (ch == SPECIAL_LEADING)
		{
			ch = _getch();
			switch (ch)
			{
			case SPECIAL_ARROW_LEFT:
				if (pos > 0)
				{
					InsertBackspace();
					pos--;
				}
				break;
			case SPECIAL_ARROW_RIGHT:
				if (pos < length)
				{
					InsertChar(decoy ? decoy : buffer[pos]);
					pos++;
				}
				break;
			case SPECIAL_ARROW_UP:
				if (history)
				{
					if (history > history_end)
						history--;
					ResetInput(origin.X, maxLen, pos, length, buffer, *history);
				}
				break;
			case SPECIAL_ARROW_DOWN:
				if (history)
				{
					if (history < history_begin)
						history++;
					ResetInput(origin.X, maxLen, pos, length, buffer, *history);
				}
				break;
			case SPECIAL_DELETE:
				if (pos < length)
				{
					for (int i = pos + 1; i < length; i++)
					{
						buffer[i - 1] = buffer[i];
						InsertChar(decoy ? decoy : buffer[i]);
					}
					InsertChar(SPACE);
					InsertBackspace(length - pos);
					length--;
					buffer[length] = '\0';
				}
				break;
			case SPECIAL_HOME:
				InsertBackspace(pos);
				pos = 0;
				break;
			case SPECIAL_END:
				for (int i = pos; i < length; i++)
					InsertChar(decoy ? decoy : buffer[i]);
				pos = length;
				break;
			default:
				break;
			}
		}
		else if (ch == BACKSPACE)
		{
			if (pos > 0)
			{
				InsertDelete();
				for (int i = pos; i < length; i++)
				{
					InsertChar(decoy ? decoy : buffer[i]);
					buffer[i - 1] = buffer[i];
				}
				InsertChar(SPACE);
				InsertBackspace(length - pos + 1);
				pos--;
				length--;
				buffer[length] = '\0';
			}
		}
		else if (length < maxLen)
		{
			if (isprint(ch))
			{
				for (int i = length; i > pos; i--)
					buffer[i] = buffer[i - 1];
				buffer[pos] = ch;
				length++;
				buffer[length] = '\0';
				for (int i = pos; i < length; i++)
					InsertChar(decoy ? decoy : buffer[i]);
				pos++;
				InsertBackspace(length - pos);
			}
		}
	}

	if (history && !IsNullOrEmpty(buffer))
	{
		options.history->Push(buffer, history - history_end);
	}

	return length;
}

void GetString(char* buffer)
{
	GetString(buffer, 1, INPUT_BUFFER_SIZE - 1);
}

void GetString(char* buffer, int minLen, int maxLen)
{
	minLen = std::max(minLen, 1);
	GetStringInterruptible(buffer, minLen, maxLen, false);
}

int GetStringInterruptible(char* buffer, bool enable)
{
	return GetStringInterruptible(buffer, 1, INPUT_BUFFER_SIZE - 1, enable);
}

int GetStringInterruptible(char* buffer, int minLen, int maxLen, bool enable)
{
	int length = 0;
	int pos = 0;
	char ch;

	buffer[0] = '\0';

	for (; ;)
	{
		ch = _getch();
		if (IsTerminator(ch))
		{
			if (length >= minLen || enable)
				return length;
		}
		else if ((ch == ESCAPE) && enable)
		{
			return -1;
		}
		else if (ch == SPECIAL_LEADING)
		{
			ch = _getch();
			if (ch == SPECIAL_ARROW_LEFT)
			{
				if (pos > 0)
				{
					InsertBackspace();
					pos--;
				}
			}
			else if (ch == SPECIAL_ARROW_RIGHT)
			{
				if (pos < length)
				{
					InsertChar(buffer[pos]);
					pos++;
				}
			}
			else if (ch == SPECIAL_DELETE)
			{
				if (pos < length)
				{
					for (int i = pos + 1; i < length; i++)
					{
						buffer[i - 1] = buffer[i];
						InsertChar(buffer[i]);
					}
					InsertChar(SPACE);
					InsertBackspace(length - pos);
					length--;
					buffer[length] = '\0';
				}
			}
			else if (ch == SPECIAL_HOME)
			{
				InsertBackspace(pos);
				pos = 0;
			}
			else if (ch == SPECIAL_END)
			{
				for (int i = pos; i < length; i++)
					InsertChar(buffer[i]);
				pos = length;
			}
		}
		else if (ch == BACKSPACE)
		{
			if (pos > 0)
			{
				InsertDelete();
				for (int i = pos; i < length; i++)
				{
					InsertChar(buffer[i]);
					buffer[i - 1] = buffer[i];
				}
				InsertChar(SPACE);
				InsertBackspace(length - pos + 1);
				pos--;
				length--;
				buffer[length] = '\0';
			}
		}
		else if (length < maxLen)
		{
			if (isprint(ch))
			{
				for (int i = length; i > pos; i--)
					buffer[i] = buffer[i - 1];
				buffer[pos] = ch;
				length++;
				buffer[length] = '\0';
				for (int i = pos; i < length; i++)
					InsertChar(buffer[i]);
				pos++;
				InsertBackspace(length - pos);
			}
		}
	}

	return length;
}

void GetPassword(char* buffer, char decoy)
{
	GetPassword(buffer, 1, INPUT_BUFFER_SIZE - 1, decoy);
}

void GetPassword(char* buffer, int minLen, int maxLen, char decoy)
{
	minLen = std::max(minLen, 1);
	GetPasswordInterruptable(buffer, minLen, maxLen, decoy, false);
}

int GetPasswordInterruptable(char* buffer, char decoy, bool enable)
{
	return GetPasswordInterruptable(buffer, 1, INPUT_BUFFER_SIZE - 1, decoy, enable);
}

int GetPasswordInterruptable(char* buffer, int minLen, int maxLen, char decoy, bool enable)
{
	int length = 0;
	int pos = 0;
	char ch;

	for (; ;)
	{
		ch = _getch();
		if (IsTerminator(ch))
		{
			if (length >= minLen || enable)
				return length;
		}
		else if ((ch == ESCAPE) && enable)
		{
			return -1;
		}
		else if (ch == SPECIAL_LEADING)
		{
			ch = _getch();
			if (ch == SPECIAL_ARROW_LEFT)
			{
				if (pos > 0)
				{
					InsertBackspace();
					pos--;
				}
			}
			else if (ch == SPECIAL_ARROW_RIGHT)
			{
				if (pos < length)
				{
					InsertChar(decoy);
					pos++;
				}
			}
			else if (ch == SPECIAL_DELETE)
			{
				if (pos < length)
				{
					for (int i = pos + 1; i < length; i++)
					{
						buffer[i - 1] = buffer[i];
						InsertChar(decoy);
					}
					InsertChar(SPACE);
					InsertBackspace(length - pos);
					length--;
					buffer[length] = '\0';
				}
			}
			else if (ch == SPECIAL_HOME)
			{
				InsertBackspace(pos);
				pos = 0;
			}
			else if (ch == SPECIAL_END)
			{
				for (int i = pos; i < length; i++)
					InsertChar(decoy);
				pos = length;
			}
		}
		else if (ch == BACKSPACE)
		{
			if (pos > 0)
			{
				InsertDelete();
				for (int i = pos; i < length; i++)
				{
					InsertChar(decoy);
					buffer[i - 1] = buffer[i];
				}
				InsertChar(SPACE);
				InsertBackspace(length - pos + 1);
				pos--;
				length--;
				buffer[length] = '\0';
			}
		}
		else if (length < maxLen)
		{
			if (isprint(ch))
			{
				for (int i = length; i > pos; i--)
					buffer[i] = buffer[i - 1];
				buffer[pos] = ch;
				length++;
				buffer[length] = '\0';
				for (int i = pos; i < length; i++)
					InsertChar(decoy);
				pos++;
				InsertBackspace(length - pos);
			}
		}
	}

	return length;
}


void FlushInput()
{
	static HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	FlushConsoleInputBuffer(hInput);
}

void WaitForKey(const char* prompt, char key)
{
	InsertText(prompt);

	FlushInput();

	for (int i = 0; ; i++)
	{
		char ch = _getch();
		if (ch == key)
			break;
		else if (key == (char)0)
			break;
	}
}

_CNSL_END