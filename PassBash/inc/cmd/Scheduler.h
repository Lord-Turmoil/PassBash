/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Sheduler.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Command shedualer.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _SHEDULER_H_
#define _SHEDULER_H_

#include "Command.h"

#include <queue>


/********************************************************************
** This is a lame sheduler. Just for parallel commands. Hmm... Like
** interface manager. Singleton used.
*/
class Scheduler
{
public:
	static Scheduler* GetInstance()
	{
		static Scheduler instance;
		
		return &instance;
	}

	void AddTask(CommandPtr cmd, ArgListPtr args);
	void Run();

private:
	Scheduler() {}
	Scheduler(const Scheduler&) = delete;
	Scheduler& operator=(const Scheduler&) = delete;

	void _RecycleTask(Task& task);

	std::queue<Task> m_queue;
};

#endif