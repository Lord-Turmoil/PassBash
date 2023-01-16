/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : IntrusiveList.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 15, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Base class for intrusive list.                                           *
 * -------------------------------------------------------------------------- *
 * Reference:                                                                 *
 *   Kiwano Engine.                                                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

// Copyright (c) 2016-2018 Kiwano - Nomango
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef _INTRUSIVE_LIST_H_
#define _INTRUSIVE_LIST_H_

#include "../common/Macros.h"

#include <type_traits>
#include <iterator>
#include <stdexcept>

/********************************************************************
** Intrusive list doesn't manage memory with new and delete.
*/
template <typename _PtrTy>
class IntrusiveList
{
public:
	using value_type = _PtrTy;
	using pointer    = value_type*;
	using reference  = value_type&;

	IntrusiveList() : m_first(nullptr), m_last(nullptr) {}

	~IntrusiveList() { Clear(); }

	value_type&       GetFirst()       { m_first; }
	const value_type& GetFirst() const { return m_first; }

	value_type&       GetLast()       { return m_last; }
	const value_type& GetLast() const { return m_last; }

	bool IsEmpty() const { return m_first == nullptr; }

	void PushBack(reference child)
	{
		_Unlink(child);

		child->m_prev = m_last;
		child->m_next = nullptr;

		if (m_first) // not empty
			m_last->m_next = child;
		else
			m_first = child;
		m_last = child;
	}

	void PushFront(reference child)
	{
		_Unlink(child);

		child->m_prev = nullptr;
		child->m_next = m_first;

		if (m_first)
			m_first->m_prev = child;
		else
			m_last = child;
		m_first = child;
	}

	// Insert child before before, before exists in the list.
	void InsertBefore(reference child, reference before)
	{
		_Unlink(child);

		if (before->m_prev)
			before->m_prev->m_next = child;
		else	// before is fist
			m_first = child;

		child->m_prev = before->m_prev;
		child->m_next = before;
		before->m_prev = child;
	}

	// Insert child after after, after exists in the list.
	void InsertAfter(reference child, reference after)
	{
		_Unlink(child);

		if (after->m_next)
			after->m_next->m_prev = child;
		else
			m_last = child;

		child->m_next = after->m_next;
		child->m_prev = after;
		after->m_next = child;
	}

	void Erase(reference child)
	{
		if (child->m_next)
			child->m_next->m_prev = child->m_prev;
		else
			m_last = child->m_prev;

		if (child->m_prev)
			child->m_prev->m_next = child->m_next;
		else
			m_first = child->m_next;

		child->m_prev = nullptr;
		child->m_next = nullptr;
	}

	// Clear the list.
	void Clear()
	{
		value_type p = m_first;
		while (p)
		{
			value_type tmp = p;
			p = p->m_next;
			if (tmp)
			{
				tmp->m_next = nullptr;
				tmp->m_prev = nullptr;
			}
		}
		m_first = nullptr;
		m_last = nullptr;
	}

	// Check if list is really a list or not.
	bool CheckValid()
	{
		if (!m_first)
			return true;

		value_type p   = m_first;
		value_type tmp = p;
		do
		{
			tmp = p;
			p = p->m_next;
			if (p)
			{
				if (p->m_prev != tmp)
					return false;
			}
			else
			{
				if (tmp != m_last)
					return false;
			}
		} while (p);

		return true;
	}

public:
	template <typename _IterPtrTy>
	struct Iterator
	{
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type        = _IterPtrTy;
		using pointer           = _IterPtrTy*;
		using reference         = _IterPtrTy&;
		using difference_type   = ptrdiff_t;

		inline Iterator(value_type ptr = nullptr, bool isEnd = false)
			: m_base(ptr), m_isEnd(isEnd) {}

		inline reference operator*() const
		{
			PASH_ASSERT(m_base && !m_isEnd);
			return const_cast<reference>(m_base);
		}

		inline pointer operator->() const
		{
			return std::pointer_traits<pointer>::pointer_to(**this);
		}

		inline Iterator& operator++()
		{
			PASH_ASSERT(m_base && !m_isEnd);
			value_type next = m_base->Next();
			if (next)
				m_base = next;
			else
				m_isEnd = true;
			return (*this);
		}

		inline Iterator operator++(int)
		{
			Iterator old = (*this);
			++(*this);
			return old;
		}

		inline Iterator& operator--()
		{
			BASH_ASSERT(m_base);
			if (m_isEnd)
				m_isEnd = false;
			else
				m_base = m_base->Prev();
			return (*this);
		}

		inline Iterator operator--(int)
		{
			Iterator old = (*this);
			--(*this);
			return old;
		}

		inline bool operator==(const Iterator& other) const
		{
			return m_base == other.m_base && m_isEnd == other.m_isEnd;
		}

		inline bool operator!=(const Iterator& other) const
		{
			return !(*this == other);
		}

		inline operator bool() const
		{
			return m_base != nullptr && !m_isEnd;
		}

	private:
		bool m_isEnd;

		typename std::remove_const<value_type>::type m_base;
	};	// struct Iterator

public:
	using iterator               = Iterator<value_type>;
	using const_iterator         = Iterator<const value_type>;
	using reverse_iterator       = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	inline iterator       begin ()       { return iterator(m_first, m_first == nullptr); }
	inline const_iterator begin () const { return const_iterator(m_first, m_first == nullptr); }
	inline const_iterator cbegin() const { return begin(); }

	inline iterator       end ()       { return iterator(m_last, true); }
	inline const_iterator end () const { return const_iterator(m_last, true); }
	inline const_iterator cend() const { return end(); }

	inline reverse_iterator       rbegin ()       { return reverse_iterator(end()); }
	inline const_reverse_iterator rbegin () const { return const_reverse_iterator(end()); }
	inline const_reverse_iterator crbegin() const { return rbegin(); }

	inline reverse_iterator       rend ()       { return reverse_iterator(begin()); }
	inline const_reverse_iterator rend () const { return const_reverse_iterator(begin()); }
	inline const_reverse_iterator crend() const { return rend(); }

	inline value_type& front()
	{
		if (IsEmpty())
			throw std::out_of_range("front() called on empty list");
		return m_first;
	}

	inline const value_type& front() const
	{
		if (IsEmpty())
			throw std::out_of_range("front() called on empty list");
		return m_first;
	}

	inline value_type& back()
	{
		if (IsEmpty())
			throw std::out_of_range("back() called on empty list");
		return m_last;
	}

	inline const value_type& back() const
	{
		if (IsEmpty())
			throw std::out_of_range("back() called on empty list");
		return m_last;
	}

private:
	void _Unlink(reference child)
	{
		if (child->m_prev)
			child->m_prev->m_next = child->m_next;
		if (child->m_next)
			child->m_next->m_prev = child->m_prev;
	}

private:
	value_type m_first;
	value_type m_last;
};


// Element of intrusive list.
template <typename _PtrTy>
class IntrusiveListValue
{
	friend class IntrusiveList<_PtrTy>;
public:
	using value_type = _PtrTy;
	using reference  = value_type&;
	using pointer    = value_type*;

	IntrusiveListValue() : m_prev(nullptr), m_next(nullptr) {}
	IntrusiveListValue(value_type rhs) : m_prev(nullptr), m_next(nullptr)
	{
		if (rhs)
		{
			m_prev = rhs->m_prev;
			m_next = rhs->m_next;
		}
	}

	value_type&       Prev()       { return m_prev; }
	const value_type& Prev() const { return m_prev; }

	value_type&       Next()       { return m_next; }
	const value_type& Next() const { return m_next; }

private:
	value_type m_prev;
	value_type m_next;
};

#endif