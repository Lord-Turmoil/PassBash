/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Sheduler.cpp                              *
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

#include "../../inc/cmd/Scheduler.h"

void Sheduler::AddTask(CommandPtr cmd, ArgListPtr args)
{
	m_queue.push(Task{ cmd, args });
}

void Sheduler::Run()
{
	while (!m_queue.empty())
	{
		Task task = m_queue.front();

		if (task.cmd)
		{
			task.cmd->OnStart();
			task.cmd->Handle(task.args);
			task.cmd->OnEnd();
		}

		_RecycleTask(task);
		m_queue.pop();
	}
}

void Sheduler::_RecycleTask(Task& task)
{
	if (task.cmd)
		delete task.cmd;
	if (task.args)
		delete task.args;
}