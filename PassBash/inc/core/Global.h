/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Global.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For all global values.                                                   *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "Document.h"

#include <string>


/********************************************************************
** I know it's better not to use global variables, but... I don't
** want to bother to wrap them, and Read Alert and CNC did so, too.
*/

extern std::string g_password;
extern std::string g_pwd;

extern PassDoc g_passDoc;


#endif