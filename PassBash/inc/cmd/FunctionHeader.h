/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : FunctionHeader.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Include all headers for all command related cpp.                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _FUCTION_HEADER_H_
#define _FUCTION_HEADER_H_

#include "../../inc/cmd/Command.h"
#include "../../inc/cmd/FunctionUtil.h"

#include "../common/Logger.h"

#include "../core/Global.h"
#include "../core/Config.h"

#include "../utility/xml.h"

#include <cnsl.h>


#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

typedef unsigned short WORD;

// Some colors.
#define PROMPT_COLOR   FOREGROUND_LIGHT(FOREGROUND_BLUE)
#define GREETING_COLOR FOREGROUND_GREEN
#define ERROR_COLOR    FOREGROUND_RED
#define MESSAGE_COLOR  FOREGROUND_YELLOW
#define GROUP_COLOR    FOREGROUND_CYAN
#define PWD_COLOR      FOREGROUND_MAGENTA
#define ITEM_COLOR     FOREGROUND_LIGHT(FOREGROUND_MAGENTA)
#define ENTRY_MODIFY_COLOR FOREGROUND_LIGHT(FOREGROUND_GREEN)
#define ENTRY_DELETE_COLOR FOREGROUND_LIGHT(FOREGROUND_RED)

// Some errors.
#define ARGUMENTS_ILLEGAL "Arguments illegal!"

#endif