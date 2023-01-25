/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Command.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 25, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Front end of command. Using factory pattern.                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _COMMAND_FACTORY_H_
#define _COMMAND_FACTORY_H_

#include "../common/Macros.h"

#include <unordered_map>
#include <string>


class CommandFactory
{
public:
	/*
	** Returns:    0 -- succeeded
	**            -1 -- no such command, or it self is failed
	**          else -- command failed
	** Warning: args must not be nullptr.
	*/
	int execl(const char* cmd, ...);	// end with nullptr
	int execv(const char* cmd, char* argv[]);
	int hook(const char* descr, Command cmd);
	int unhook(const char* descr);

private:
	/*
	** If char* is used, it will only compare pointer value, not
	** the actual string value!!!
	*/
	std::unordered_map<std::string, Command> m_records;
};

extern CommandFactory g_generalFactory;
extern CommandFactory g_editorFactory;
extern CommandFactory g_hiddenFactory;

void init_factory();

#endif
