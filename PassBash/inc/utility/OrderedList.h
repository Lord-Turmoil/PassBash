/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : OrderedList.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 15, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Ordered list. Using intrusive linked list.                               *
 * -------------------------------------------------------------------------- *
 * Reference:                                                                 *
 *   https://www.data-structures-in-practice.com/intrusive-linked-lists/      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _ORDERED_LIST_H_
#define _ORDERED_LIST_H_

#include "IntrusiveList.h"

// Ordered list is based on intrusive list.
template<typename _PtrTy, class _Pr>
class OrderedList
{
public:
	using value_type    = typename std::pointer_traits<_PtrTy>::pointer;
	using pointer       = value_type*;
	using reference     = value_type&;
	using value_compare = _Pr;
	using container     = IntrusiveList<_PtrTy>;

	void Insert(reference child)
	{
		if (!m_list.IsEmpty())
		{
			for (auto& p : m_list)
			{
				if (value_compare(child, p))
				{
					m_list.InsertBefore(child, p);
					return;
				}
			}
		}
		m_list.PushBack(child);
	}

	void Erase(reference child)
	{
		m_list.Erase(child);
	}

	const container& Values() const { return m_list; }

protected:
	container m_list;
};

#endif