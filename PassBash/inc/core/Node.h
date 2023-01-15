/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Node.h                                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 15, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Password structure.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _NODE_H_
#define _NODE_H_

#include "entry.h"

#include <queue>
#include <string>


/********************************************************************
** The structure is the same as file structure. Root node is created
** by default.
** Root
**   |- .
**   |- ..
**   |- Group
**   |    |- .
**   |    |- ..
**   |    |- Item
**   |- Item
*/

/*
** Both group and item are node, but Node shouldn't be instanciated.
** First two children are itself and its parent and shouldn't be deleted.
*/
class Node
{
	friend class CompareNodePtr;

public:
	const std::string& Name() const { return m_name; }
	
	void Destroy();
	void AddChild();

protected:
	Node();
	~Node();

protected:
	std::string m_name;
	std::priority_queue<Node*, std::vector<Node*>, CompareNodePtr> m_children;
};

class CompareNodePtr
{
public:
	using NodePtr = Node*;
	bool operator()(const NodePtr& lhs, const NodePtr& rhs)
	{
		return lhs->m_name < rhs->m_name;
	};
};


#endif
