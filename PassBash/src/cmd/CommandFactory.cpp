/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : CommandFactory.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Well, Simple factory pattern.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/CommandFactory.h"
#include "../../inc/cmd/Command.h"
#include "../../inc/common/Logger.h"
#include "../../inc/utility/Auxilliary.h"

CommandPtr CommandFactory::Spawn(const char* cmd)
{
	if (!cmd)
	{
		LOG_ERROR("Seriously?");
		return nullptr;
	}

	CommandPtr ret = nullptr;
	
	if (_STR_SAME(cmd, "cd"))
		ret = CommandPtr(new ChangeDirectoryCommand());
	else if (_STR_SAME(cmd, "ls"))
		ret = CommandPtr(new ListCommand());
	else if (_STR_SAME(cmd, "pwd"))
		ret = CommandPtr(new PwdCommand());
	else if (_STR_SAME(cmd, "clear"))
		ret = CommandPtr(new ClearCommand());
	else if (_STR_SAME(cmd, "mod"))
		ret = CommandPtr(new EditCommand());
	else if (_STR_SAME(cmd, "touch"))
		ret = CommandPtr(new TouchCommand());
	else if (_STR_SAME(cmd, "mkdir"))
		ret = CommandPtr(new MkdirCommand());
	else if (_STR_SAME(cmd, "rm"))
		ret = CommandPtr(new RemoveCommand());
	else if (_STR_SAME(cmd, "tree"))
		ret = CommandPtr(new TreeCommand());
	else if (_STR_SAME(cmd, "save"))
		ret = CommandPtr(new SaveCommand());
	
	return ret;
}

CommandPtr CommandFactory::SpawnSpecial(const char* cmd)
{
	if (!cmd)
	{
		LOG_ERROR("Seriously?");
		return nullptr;
	}

	CommandPtr ret = nullptr;
	if (_STR_SAME(cmd, "Start"))
		ret = CommandPtr(new StartCommand());
	else if (_STR_SAME(cmd, "Register"))
		ret = CommandPtr(new RegisterCommand());
	else if (_STR_SAME(cmd, "Login"))
		ret = CommandPtr(new LoginCommand());
	else if (_STR_SAME(cmd, "Host"))
		ret = CommandPtr(new HostCommand());

	if (!ret)
	{
		LOG_ERROR("Unexpected special command \"%s\"", cmd);
		return nullptr;
	}

	return ret;
}