/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Entry.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 15, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Entry, emm... a key and a value.                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _ENTRY_H_
#define _ENTRY_H_

#include "../common/Macros.h"
#include "../utility/IntrusiveList.h"

#include <string>

DECLARE_CLASS(Entry)

class Entry : private IntrusiveListValue<EntryPtr>
{
	friend class EntryPtrCompare;
	friend class IntrusiveList<EntryPtr>;
public:
	class EntryPtrCompare
	{
	public:
		bool operator()(const EntryPtr& lhs, const EntryPtr& rhs)
		{
			if (lhs->m_weight == rhs->m_weight)
				return lhs->m_key < rhs->m_key;
			else
				return lhs->m_weight < rhs->m_weight;
		};
	};

public:
	Entry(const std::string& key, const std::string& value, int weight) :
		m_key(key), m_value(value), m_weight(weight) {}
	Entry(const std::string& key, const std::string& value) :
		m_key(key), m_value(value), m_weight(0) {}
	Entry() : m_weight(0) {}	// deprecated

	void Key   (const std::string& key)   { m_key = key; }
	void Value (const std::string& value) { m_value = value; }
	void Weight(int weight)               { m_weight = weight; }

	const std::string& Key()    const { return m_key; }
	const std::string& Value()  const { return m_value; }
	int                Weight() const { return m_weight; }

	void Set(const std::string& key, const std::string& value)
	{
		Key(key);
		Value(value);
	}
	void Get(std::string& key, std::string& value)
	{
		key = Key();
		value = Value();
	}

private:
	std::string m_key;
	std::string m_value;
	int m_weight;	// for sorting
};

#endif
