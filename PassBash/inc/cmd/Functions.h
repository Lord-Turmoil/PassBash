/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Functions.h                               *
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

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "../common/Macros.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Ordinary Commands
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
// cd <group name>
DEC_CMD(cd);
// ls [group name]
DEC_CMD(ls);
// touch <item name>
DEC_CMD(touch);
// mkdir <group name>
DEC_CMD(mkdir);
// rm <node name>
DEC_CMD(rm);
// move <src> <dest>
DEC_CMD(move);
// save
DEC_CMD(save);
// tree [group name]
DEC_CMD(tree);
// cat <item name>
DEC_CMD(cat);
// rename <old> <new>
DEC_CMD(rename);
// clear
DEC_CMD(clear);
// pwd
DEC_CMD(pwd);
// help
DEC_CMD(help);
// find [-d] [-s] [-r root] <pattern>
DEC_CMD(find);
// edit <item name>
DEC_CMD(edit);

// tea [-u] <src> <dst> <password>
DEC_CMD(encrypt);

// version, ver
DEC_CMD(version);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Hidden Command
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DEC_CMD(unknown);
DEC_CMD(quit);
// order
DEC_CMD(order);

#endif
