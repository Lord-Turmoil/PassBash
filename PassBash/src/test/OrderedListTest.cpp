/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : OrderedListTest.cpp                       *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 15, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For unit test.                                                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/common/Test.h"

#if TEST(ORDERED_LIST)

#include "../../inc/utility/OrderedList.h"

#include <cnsl.h>
#include <vector>
#include <queue>


DECLARE_CLASS(Item)

class Item : private IntrusiveListValue<ItemPtr>
{
	friend class ItemPtrCompare;
	friend class ItemPtrEqual;
	friend class IntrusiveList<ItemPtr>;
public:
	Item(int value, int tag) : m_value(value), m_tag(tag) {}
	~Item()
	{
		cnsl::InsertText("~Item[%d]\n", m_tag);
	}

	void Print()
	{
		cnsl::InsertText("(%2d, %2d) ", m_value, m_tag);
	}

private:
	int m_value;
	int m_tag;
};

class ItemPtrCompare
{
public:
	bool operator()(const ItemPtr& lhs, const ItemPtr& rhs)
	{
		return lhs->m_value < rhs->m_value;
	};
};

class ItemPtrEqual
{
public:
	bool operator()(const ItemPtr& lhs, const ItemPtr& rhs)
	{
		return lhs->m_value == rhs->m_value;
	};
};

int main()
{
	cnsl::InitConsole(110);
	cnsl::SetTitle("Ordered List Test");
	cnsl::Print();
	cnsl::OverflowReprint(false);

	std::vector<ItemPtr> items;

	items.push_back(new Item(2, 1));
	items.push_back(new Item(8, 2));
	items.push_back(new Item(1, 3));
	items.push_back(new Item(9, 4));
	items.push_back(new Item(3, 5));
	items.push_back(new Item(7, 6));
	items.push_back(new Item(4, 7));
	items.push_back(new Item(6, 8));
	items.push_back(new Item(5, 9));
	items.push_back(new Item(3, 10));
	items.push_back(new Item(3, 11));

	IntrusiveList<ItemPtr> list;
	for (auto& item : items)
		list.PushBack(item);
	for (auto it : list)
		it->Print();
	cnsl::InsertNewLine();

	OrderedList<ItemPtr, ItemPtrCompare> olist;
	for (auto& item : items)
		olist.Insert(item);
	for (auto& item : olist.Values())
		item->Print();
	cnsl::InsertNewLine();

	UniqueOrderedList<ItemPtr, ItemPtrCompare, ItemPtrEqual> uolist;
	for (auto& item : items)
	{
		if (!uolist.Insert(item))
		{
			cnsl::InsertText("!");
			item->Print();
		}
	}
	for (auto& item : uolist.Values())
		item->Print();
	cnsl::InsertNewLine();

	cnsl::InsertSplitLine('-');

	// List must be cleared before elements are deleted!
	list.Clear();
	olist.Clear();
	uolist.Clear();

	for (auto& item : items)
		delete item;

	return 0;
}

#endif	// PASH_TEST
