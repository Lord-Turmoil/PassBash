/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : MemPool.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 15, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Memory pool for allocation.                                              *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _MEM_POOL_H_
#define _MEM_POOL_H_

#include "../common/Macros.h"

#include <queue>
#include <vector>


template<typename _Ty>
class MemPool
{
public:
	using value_type = _Ty;
	using pointer    = _Ty*;
	using reference  = _Ty&;

	MemPool() { m_pBlock = _AllocBlock(); }

	~MemPool()
	{
		Destroy();
	}

	// Free all memory.
	void Destroy()
	{
		while (!m_unused.empty())
			m_unused.pop();
		for (auto pool : m_pool)
			_FreeBlock(pool);
		m_pool.clear();
		m_pBlock = m_pItem = nullptr;
	}

	pointer Alloc()
	{
		pointer alloc = nullptr;
		if (m_unused.empty())
		{
			alloc = m_pItem++;
			if (m_pItem - m_pBlock >= BLOCK_SIZE)
				m_pBlock = _AllocBlock();
		}
		else
		{
			alloc = m_unused.front();
			m_unused.pop();
		}

		return alloc;
	}

	// Do not free an item twice!
	void Free(pointer pItem) { m_unused.push(pItem); }

private:
	pointer _AllocBlock()
	{
		pointer pBlock = new value_type[BLOCK_SIZE];
		m_pItem = pBlock;
		m_pool.push_back(pBlock);
		return pBlock;
	}

	void _FreeBlock(pointer pBlock)
	{
		delete[] pBlock;
	}

	// Using 4K as a block.
	enum { BLOCK_SIZE = 4 * 1024 / sizeof(value_type) };

	std::vector<pointer> m_pool;
	std::queue<pointer> m_unused;
	pointer m_pBlock;	// current block
	pointer m_pItem;	// current item to allocate
};

#endif