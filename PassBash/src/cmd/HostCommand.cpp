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

#include "../../inc/cmd/CommandHeader.h"
#include "../../inc/utility/Auxilliary.h"

#include <cstring>


static char HOST_IGNORE[] = " ";
static char HOST_WAITING[] = R"(-\|/)";

void HostCommand::OnStart()
{
	cnsl::Clear();
	cnsl::Print();
	cnsl::InsertText(MESSAGE_COLOR, "Initializing Pash Host...  ");
	
	// initializing... ?
	g_pwd = "./";
	//

	for (int i = 0; i < 20; i++)
	{
		cnsl::InsertBackspace();
		cnsl::InsertChar(HOST_WAITING[i % 4]);
		Sleep(40);
	}
	cnsl::InsertDelete();
	cnsl::InsertText(GREETING_COLOR, "\nPash Host fully operational!\n");
	cnsl::InsertText(MESSAGE_COLOR, "User \"help\" for more information.\n\n");

	cnsl::FlushInput();
}

bool HostCommand::Handle(const ArgListPtr args)
{
	if (!_LoadData())
	{
		LOG_ERROR("\t|- Data file crashed!");
		LOG_ERROR("Pash Host has encountered critical error!");
		return false;
	}

	Task task;
	while (_PeekTask(task))
	{
		cnsl::InsertNewLine();
		if (task.cmd)
		{
			task.cmd->OnStart();
			task.cmd->Handle(task.args);
			task.cmd->OnEnd();
		}
		_RecycleTask(task);	
	}
	cnsl::InsertNewLine();

	return !HAS_ERROR();
}

bool HostCommand::_LoadData()
{
	return g_passDoc.Load(g_password.c_str());
}

/********************************************************************
** If no command detected, or is invalid command, return nullptr.
*/
bool HostCommand::_PeekTask(Task& task)
{
	static char buffer[256];
	static char* context = nullptr;

	task.cmd = nullptr;
	task.args = nullptr;
	
	cnsl::InsertText(PWD_COLOR, "%s", g_pwd.c_str());
	cnsl::InsertText(PROMPT_COLOR, "\b$ ");
	cnsl::GetString(buffer, 1, 255);

	char* token = strtok_s(buffer, HOST_IGNORE, &context);
	if (!token)	// nothing.
		return true;
	else if (_STR_SAME(token, "quit") || _STR_SAME(token, "q"))
		return false;
	
	task.cmd = CommandFactory::Spawn(token);
	if (!task.cmd)
		_UnRecognized(token);
	else
	{
		token = strtok_s(nullptr, HOST_IGNORE, &context);
		if (token)
		{
			task.args = new ArgList();
			while (token)
			{
				task.args->push_back(token);
				token = strtok_s(nullptr, HOST_IGNORE, &context);
			}
		}
	}	

	return true;
}

void HostCommand::_UnRecognized(const char* cmd)
{
	cnsl::InsertNewLine();
	cnsl::InsertText(ERROR_COLOR, "\"%s\" is not a command.", cmd);
}

void HostCommand::_RecycleTask(Task& task)
{
	if (task.args)
		delete task.args;
}
