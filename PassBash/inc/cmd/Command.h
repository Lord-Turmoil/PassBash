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
#include "../common/Logger.h"
#include "../utility/xml.h"

#include <vector>
#include <string>

#include <Windows.h>


typedef std::vector<std::string> ArgList;
typedef ArgList* ArgListPtr;

DECLARE_SMART_CLASS(Command)
DECLARE_CLASS(Scheduler)

struct Task
{
	CommandPtr cmd;
	ArgListPtr args;
};

class Command
{
public:
	Command(const std::string& name) : m_name(name) {}
	virtual ~Command() {}

	const std::string& Name() const { return m_name; }

	virtual void OnStart() {}
	virtual void OnEnd() {}

	virtual bool Handle(const ArgListPtr args) = 0;

private:
	std::string m_name;
};


#define DEFINE_CMD_CTOR(CMD) \
	CMD##Command() : Command(#CMD) {}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Special Commands.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
// Start command is the first to do initialization.
class StartCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Start)

	virtual void OnStart();
	virtual bool Handle(const ArgListPtr args);

private:
	void _InitConsole();
	void _InitConfig();
};

// First use for setting master password.
class RegisterCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Register)

	virtual void OnStart();
	virtual bool Handle(const ArgListPtr args);

private:
	void _PrintGreeting();
	void _ReceiveNewPassword();
	void _ReEncryptData();
};

// Login before use.
class LoginCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Login)

	virtual void OnStart();
	virtual bool Handle(const ArgListPtr args);

private:
	bool _ReceivePassword();
	bool _Validate(const char* key);
};

// Host for other commands.
class HostCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Host)

	virtual void OnStart();
	virtual bool Handle(const ArgListPtr args);

private:
	bool _LoadData();

	bool _PeekTask(Task& task);
	void _UnRecognized(const char* cmd);

	void _RecycleTask(Task& task);
};

// Change password command. Called by host.
class ChangePasswordCommand : public Command
{
public:
	DEFINE_CMD_CTOR(ChangePassword);

	virtual void OnStart();
	virtual bool Handle(const ArgListPtr args);

private:
	bool _Authorize();
	bool _ReceivePassword();
	bool _ReEncryptData();
};


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Ordinary Commands
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
// cd <group name>
class ChangeDirectoryCommand : public Command
{
public:
	DEFINE_CMD_CTOR(ChangeDirectory)

	virtual bool Handle(const ArgListPtr args);

private:
	static XMLElementPtr m_last;
};

// ls [group name]
class ListCommand : public Command
{
public:
	DEFINE_CMD_CTOR(List)

	virtual bool Handle(const ArgListPtr args);
};

// touch <item name>
class TouchCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Touch)

	virtual bool Handle(const ArgListPtr args);
};

// mkdir <group name>
class MkdirCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Mkdir)

	virtual bool Handle(const ArgListPtr args);
};

// rm <node name>
class RemoveCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Remove)

	virtual bool Handle(const ArgListPtr args);
private:
	bool _Confirm(const char* prompt);
	bool _DeleteRoot();
	bool _DeleteCurrent();
};

// move <src> <dest>
class MoveCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Move)

	virtual bool Handle(const ArgListPtr args);

private:
};

// save
class SaveCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Save)

	virtual bool Handle(const ArgListPtr args);
};

// tree [group name]
class TreeCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Tree)

	virtual bool Handle(const ArgListPtr args);

private:
	void _Tree(XMLElementPtr node, const std::string& leading);
};

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** cat <item name>
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
class CatCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Cat)

	virtual bool Handle(const ArgListPtr args);

private:
	void _See(XMLElementPtr item);
};

// rename <old> <new>
class RenameCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Rename)

	virtual bool Handle(const ArgListPtr args);
};


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Minor Commands
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
class ClearCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Clear)

	virtual bool Handle(const ArgListPtr args);
};

class PwdCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Pwd)

	virtual bool Handle(const ArgListPtr args);
};

class HelpCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Help)

	virtual bool Handle(const ArgListPtr args);
};

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Advanced Command
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

/********************************************************************
** Its command is mod, instead of edit, because I think "edit" is not
** that easy to type in as a result of "e" and "d".
*/
class EditCommand : public Command
{
public:
	EditCommand() : Command("Edit"), m_item(nullptr) {}
	
	virtual void OnStart();
	virtual void OnEnd();
	virtual bool Handle(const ArgListPtr args);

private:
	char* _SkipLeadingWhiteSpace(char* buffer);
	void _ParseType(const char* cmd, char* type);
	char* _ParseCommand(char* cmd);
	void _EditPrompt();

	// help  abbr. h
	void _Help();

	// clear  abbr. c
	void _Clear();

	// see  abbr. s
	void _See();
	void _See(const char* key, WORD color);	// highlight one entry

	// set:key:value:weight
	// setk:old key:new key
	// setv:key:value
	// setw:key:weight
	void _SetPrompt(char* cmd);
	void _SetKeyPrompt(char* cmd);
	void _SetValuePrompt(char* cmd);
	void _SetWeightPrompt(char* cmd);
	bool _SetKey(const char* oldKey, const char* newKey);
	// nullptr for remain.
	bool _SetEntry(const char* key, const char* value, const char* weightStr);

	// ustd
	void _UnSetIDPrompt(char* cmd);

	// unset:key  abbr. u
	void _UnSetPrompt(char* cmd);
	void _UnSet(const char* key);

	void _UnRecognized(const char* cmd);
	
	XMLElementPtr m_item;
};


class FindCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Find)

	void OnStart();
	virtual bool Handle(const ArgListPtr args);

private:
	void _Find(XMLElementPtrList& list);
	void _Search(XMLElementPtr root, XMLElementPtrList& list);
	void _SearchItem(XMLElementPtr item, XMLElementPtrList& list);

	bool m_deep;
	bool m_strict;
	std::string m_pattern;
};

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Hidden command. This will export passwords.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
// order [66]
class CheatCommand : public Command
{
public:
	DEFINE_CMD_CTOR(Cheat)

	virtual bool Handle(const ArgListPtr args);
};

#endif