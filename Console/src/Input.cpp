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
 *                    Last Update :                                           *
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

#include <ctime>
#include <cctype>

#ifdef max
#undef max
#endif

_CNSL_BEGIN

static const char INPUT_TERMINATOR[] = "\n\r";
char buffer[INPUT_BUFFER_SIZE];

static bool IsTerminator(char ch)
{
	for (const char* p = INPUT_TERMINATOR; *p; p++)
	{
		if (*p == ch)
			return true;
	}

	return false;
}

void GetString(char* buffer)
{
	GetString(buffer, 1, INPUT_BUFFER_SIZE - 1);
}

void GetString(char* buffer, int minLen, int maxLen)
{
	minLen = std::max(minLen, 1);
	GetStringInterruptable(buffer, minLen, maxLen, false);
}

int GetStringInterruptable(char* buffer, bool enable)
{
	return GetStringInterruptable(buffer, 1, INPUT_BUFFER_SIZE - 1, enable);
}

int GetStringInterruptable(char* buffer, int minLen, int maxLen, bool enable)
{
	int length = 0;
	int pos = 0;
	char ch;

	for (; ;)
	{
		ch = _getch();
		if (IsTerminator(ch))
		{
			if (length >= minLen)
				return length;
			else if (enable)
				return -1;
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
			if (length >= minLen)
				return length;
			else if (enable)
				return -1;
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