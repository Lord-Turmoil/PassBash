/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : HostCommand.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Host command is the host of all commands.                                *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/FunctionUtil.h"


static char HOST_IGNORE[] = " ";
static char HOST_WAITING[] = R"(-\|/)";

static void _host_greet()
{
	/*
	** 2022/01/20 TS: Commented animation.
	cnsl::InsertText(MESSAGE_COLOR, "Initializing Pash Host...  ");

	for (int i = 0; i < 20; i++)
	{
		cnsl::InsertBackspace();
		cnsl::InsertChar(HOST_WAITING[i % 4]);
		Sleep(40);
	}
	cnsl::InsertDelete();
	cnsl::InsertText(GREETING_COLOR, "\nPash Host fully operational!\n");
	*/

	// cnsl::Clear();
	// cnsl::Print();
	cnsl::InsertText(GREETING_COLOR, "Pash Host fully operational!\n");
	cnsl::InsertText(MESSAGE_COLOR, "Use \"help\" for more information.\n");
	cnsl::InsertSplitLine('_');
	
	cnsl::FlushInput();
}

static int _host_load_data()
{
	if (g_passDoc.Load(g_password))
		return 0;
	else
		return 1;
}

static char _buffer[CMD_BUFFER_SIZE + 1];
static char* _cmd;
static int _argc;
static char* _argv[CMD_ARG_SIZE];

static int _host_peek_command()
{
	_cmd = nullptr;

	cnsl::InsertText(PWD_COLOR, "%s", g_pwd.c_str());
	cnsl::InsertText(PROMPT_COLOR, "\b$ ");
	cnsl::GetStringInterruptible(_buffer, 0, CMD_BUFFER_SIZE);
	
	char* context = nullptr;
	char* token = strtok_s(_buffer, HOST_IGNORE, &context);
	if (!token)	// nothing.
		return 0;

	// first arg is command name
	_argc = 0;
	_cmd = token;
	_argv[_argc++] = _cmd;
	
	while (token = strtok_s(nullptr, HOST_IGNORE, &context))
		_argv[_argc++] = token;
	_argv[_argc] = nullptr;

	return 0;
}

int host(int argc, char* argv[])
{
	if (_host_load_data())
	{
		LOG_ERROR("\t|- Data file crashed!");
		LOG_ERROR("Pash Host has encountered critical error!");
		return 1;
	}

	_host_greet();

	while (_host_peek_command() == 0)
	{
		cnsl::InsertNewLine();
		if (!_cmd)
			continue;

		int ret = g_generalFactory.execv(_cmd, _argv);
		if (ret == -1)			// unknown
			g_hiddenFactory.execl("unknown", "unknown", _cmd, nullptr);
		else if (ret == 66)	// exit
			break;
		else if (ret != 0)
			LOG_ERROR("\"%s\" -- Error Code: %d", _cmd, ret);
	}

	return 0;
}