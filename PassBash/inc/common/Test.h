/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Test.h                                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 15, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For test info.                                                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _TEST_H_
#define _TEST_H_

#include "Macros.h"

#ifdef PASH_TEST

// Ahh... #define can not nest #define
#define TEST(PROJ) PASH_TEST_##PROJ

#define PASH_TEST_ORDERED_LIST 0	// Passed
#define PASH_TEST_TEA          0	// Passed
#define PASH_TEST_LOGGER       0	// Passed
#define PASH_TEST_PASSDOC      0	// Passed
#define PASH_TEST_CONFIG       0	// Passed

#define PASH_TEST_MAIN         1

#endif

#endif