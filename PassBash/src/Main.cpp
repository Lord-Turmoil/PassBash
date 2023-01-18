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

#include "../inc/cmd/CommandFactory.h"
#include "../inc/cmd/Scheduler.h"


int main()
{
	Scheduler::GetInstance()
		->AddTask(CommandFactory::SpawnSpecial("Start"), nullptr);
	Scheduler::GetInstance()->Run();

#ifdef PASH_DEBUG
	PRINT_ERROR();
	PRINT_MESSAGE();
#endif

	return 0;
}

#endif