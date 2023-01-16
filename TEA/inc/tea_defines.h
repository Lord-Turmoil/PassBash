/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : TEA                                       *
 *                                                                            *
 *                      File Name : tea_defines.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Basic defines.                                                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _TEA_DEFINES_H_
#define _TEA_DEFINES_H_

#define _TEA_BEGIN	namespace tea {
#define _TEA_END	}
#define _TEA		::tea::

_TEA_BEGIN

typedef unsigned long DATA;

const size_t NCHAR = 2 * sizeof(long);	//  64 bit =  8 Byte =  8 char
const size_t KCHAR = 2 * NCHAR;		// 128 bit = 16 Byte = 16 char

_TEA_END

#endif