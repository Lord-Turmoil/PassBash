/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : LoggerTest.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Logger test.                                                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/common/Test.h"

#if TEST(LOGGER)

#include "../../inc/common/Logger.h"

#include <cnsl.h>

int main()
{
	cnsl::InitConsole(110);
	cnsl::SetTitle("Logger Test");
	cnsl::Print();

	LOG_ERROR("First error");
	LOG_ERROR("%s error", "Second");
	LOG_ERROR(LOG_LOCATION "Third error", "LoggerTest");

	PRINT_LOG();

	return 0;
}

#endif