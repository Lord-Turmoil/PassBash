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

#if PASH_TEST_ORDERED_LIST

#include "../../inc/utility/OrderedList.h"

#include <cnsl.h>
#include <vector>


DECLARE_CLASS(Item)

class Item : private IntrusiveListValue<ItemPtr>
{
	friend class CompareItemPtr;
	friend class IntrusiveList<ItemPtr>;
public:
	Item(int value, int tag) : m_value(value), m_tag(tag) {}
	~Item()
	{
		cnsl::InsertText("~Item[%d]\n", m_tag);
	}

	void Print()
	{
		cnsl::InsertText("(%3d, %3d) ", m_value, m_tag);
	}

private:
	int m_value;
	int m_tag;
};

class CompareItemPtr
{
public:
	bool operator()(const ItemPtr& lhs, const ItemPtr& rhs)
	{
		return lhs->m_value < rhs->m_value;
	};
};
int main()
{
	cnsl::InitConsole(110);
	cnsl::SetTitle("Ordered List Test");
	cnsl::OverflowReprint(false);

	std::vector<ItemPtr> items;

	items.push_back(new Item(1, 1));
	items.push_back(new Item(9, 2));
	items.push_back(new Item(2, 3));
	items.push_back(new Item(8, 4));
	items.push_back(new Item(3, 5));
	items.push_back(new Item(7, 6));
	items.push_back(new Item(4, 7));
	items.push_back(new Item(6, 8));
	items.push_back(new Item(5, 9));

	IntrusiveList<ItemPtr> list;

	for (auto& item : items)
		list.PushBack(item);
	for (auto it : list)
		it->Print();

	cnsl::InsertNewLine();
	cnsl::InsertSplitLine('-');

	// List must be cleared before elements are deleted!
	list.Clear();

	for (auto& item : items)
		delete item;

	return 0;
}

#endif
