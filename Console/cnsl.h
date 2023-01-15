/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : Console                                   *
 *                                                                            *
 *                      File Name : cnsl.h                                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 1, 2023                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Well, provide everything needed for console lib.                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _CNSL_H_
#define _CNSL_H_

#include "inc/Console.h"
#include "inc/Output.h"
#include "inc/Input.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "console_b.lib")
#else
#pragma comment(lib, "console.lib")
#endif

#endif
