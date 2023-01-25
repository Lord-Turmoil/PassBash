/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Main.cpp                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The entrance.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../inc/common/Macros.h"

#if !PASH_TEST

#include "../inc/cmd/Command.h"
#include "../inc/common/Logger.h"


int main()
{
	init_factory();

	g_hiddenFactory.execl("start", nullptr);

#ifdef PASH_DEBUG
	LOG_PRINT_ERROR();
	LOG_PRINT_MESSAGE();
#endif

	return 0;
}

#endif