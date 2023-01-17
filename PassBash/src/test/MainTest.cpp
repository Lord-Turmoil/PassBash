/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : MainTest.cpp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Err... Test for main?                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/common/Test.h"

#if TEST(MAIN)

#include "../../inc/cmd/CommandFactory.h"
#include "../../inc/cmd/Scheduler.h"


int main()
{
	Scheduler::GetInstance()
		->AddTask(CommandFactory::SpawnSpecial("Start"), nullptr);
	Scheduler::GetInstance()->Run();

	PRINT_ERROR();
	PRINT_MESSAGE();

	return 0;
}

#endif