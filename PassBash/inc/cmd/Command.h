/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : public Command.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Command...                                                               *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "../common/Macros.h"

#include <vector>
#include <string>

typedef std::vector<std::string> ArgList;
typedef ArgList* ArgListPtr;

DECLARE_CLASS(Command)

class Command
{
public:
	virtual void OnStart() {}
	virtual void OnEnd() {}

	virtual bool Handle(const ArgListPtr args) = 0;
};

// Start command is the first to do initialization.
class StartCommand : public Command
{
public:
	virtual bool Handle(const ArgListPtr args);

private:
	void _InitConsole();
	void _InitData();

};

// First use for setting master password.
class SetPasswordCommand : public Command
{
public:
	virtual bool Handle(const ArgListPtr args);
};

// Login before use.
class LoginCommand : public Command
{
public:
	virtual bool Handle(const ArgListPtr args);
};

// Host for other commands.
class HostCommand : public Command
{
public:
	virtual bool Handle(const ArgListPtr args);
};

// cd <group name>
class ChangeDirectoryCommand : public Command
{
public:
	virtual bool Handle(const ArgListPtr args);
};

// ls
class ListCommand : public Command
{
public:
	virtual bool Handle(const ArgListPtr args);
};

// touch <item name>
class TouchCommand : public Command
{
public:
	virtual bool Handle(const ArgListPtr args);
};

// mkdir <group name>
class MkdirCommand : public Command
{
public:
	virtual bool Handle(const ArgListPtr args);
};

#endif