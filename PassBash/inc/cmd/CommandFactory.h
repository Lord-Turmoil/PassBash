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

#ifndef _COMMAND_FACTORY_H_
#define _COMMAND_FACTORY_H_

#include "../../inc/common/Macros.h"

DECLARE_SMART_CLASS(Command)

class CommandFactory
{
public:
	// This is for basic command line operations.
	static CommandPtr Spawn(const char* cmd);

	// This is for advanced commands which act as interface.
	static CommandPtr SpawnSpecial(const char* cmd);
};

#endif
