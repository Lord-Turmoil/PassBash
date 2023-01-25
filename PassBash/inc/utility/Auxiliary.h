/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Auxiliary.h                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 15, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Auxiliary functions.                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _AUXILIARY_H_
#define _AUXILIARY_H_

#include <cstring>

#define _STR_SAME(STR1, STR2) (strcmp(STR1, STR2) == 0)
#define _STR_DIFF(STR1, STR2) (strcmp(STR1, STR2) != 0)

#define _STR_NSAME(STR1, STR2) (strncmp(STR1, STR2, strlen(STR2)) == 0)
#define _STR_NDIFF(STR1, STR2) (strncmp(STR1, STR2, strlen(STR2)) != 0)


#endif