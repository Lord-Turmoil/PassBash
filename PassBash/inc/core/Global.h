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

#include "PassDoc.h"

#include <string>


/********************************************************************
** I know it's better not to use global variables, but... I don't
** want to bother to wrap them, and Read Alert and CNC did so, too.
*/

extern const int g_PASSWORD_LENGH;
extern const std::string g_default;
extern std::string g_password;
extern char g_encodedPassword[];
extern char g_decodedPassword[];

extern std::string g_pwd;

extern PassDoc g_passDoc;

extern const char g_TITLE[];
extern const char g_COPYRIGHT[];
extern const char g_AUTHOR[];

extern const char g_DATA_FILE[];
extern const char g_CONFIG_FILE[];

extern const char g_DEFAULT_DATA[];

class Config;
extern Config g_config;

#endif