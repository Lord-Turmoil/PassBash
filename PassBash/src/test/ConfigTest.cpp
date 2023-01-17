/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : ConfigTest.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Config test.                                                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/common/Test.h"

#if TEST(CONFIG)

#include "../../inc/core/Config.h"
#include "../../inc/core/Global.h"
#include "../../inc/common/Logger.h"

#include <cnsl.h>

int main()
{
	cnsl::InitConsole(110);
	cnsl::SetTitle("Config Test");
	cnsl::Print();

	g_config.Load();
	PRINT_ERROR();

	return 0;
}

#endif