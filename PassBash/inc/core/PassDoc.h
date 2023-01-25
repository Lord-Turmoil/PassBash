/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : PassDoc.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Structure of password document.                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _PASS_DOC_H_
#define _PASS_DOC_H_

#include "../utility/xml.h"
#include "../utility/Auxiliary.h"

#include <vector>
#include <string>


const char GROUP_TAG[] = "Group";
const char ITEM_TAG[]  = "Item";
const char ENTRY_TAG[] = "Entry";

const char PARENT_DIR_NAME[] = "..";
const char SELF_DIR_NAME[]   = ".";
const char ROOT_DIR_NAME[]   = "./";

class PassDoc
{
public:
	PassDoc();
	~PassDoc();

	bool Load(const char* password);
	void UnLoad();
	bool Save(const char* password);

	// For debug purpose, import and export plain text to debug.xml.
	bool Load();
	bool Save();

	bool IsLoaded() const;

	XMLElementPtr GetRoot() { return m_root; }
	XMLElementPtr GetCurrent() { return m_current; }

	/*
	** Only group can be set to current, and it returns last current.
	** If not, it returns nullptr, and do nothing.
	*/
	XMLElementPtr SetCurrent(XMLElementPtr current);

	XMLElementPtr NewElement(const char* name);
	void DeleteElement(XMLElementPtr node);

	// Mark modified.
	void Mark();

private:
	// Generate default data.
	bool _GenerateData();

private:
	XMLFile m_file;

	XMLElementPtr m_root;
	XMLElementPtr m_current;

	bool m_modified;
};


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Some utility functions for XMLElement info.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
inline bool IsGroup(XMLElementPtr node)
{
	return _STR_SAME(node->Name(), GROUP_TAG);
}

inline bool IsItem(XMLElementPtr node)
{
	return _STR_SAME(node->Name(), ITEM_TAG);
}

inline const char* GetNodeName(XMLElementPtr node)
{
	return node->Attribute("name");
}

inline const char* GetNodeAttr(XMLElementPtr node, const char* attr)
{
	return node->Attribute(attr);
}

inline XMLElementPtr GetParentNode(XMLElementPtr node)
{
	return node->Parent()->ToElement();
}

inline bool IsLegalNodeName(const std::string& name)
{
	return (
		(name.find('/') == std::string::npos) &&
		(name.find(' ') == std::string::npos));
}

struct XMLElementPtrCompare
{
	// Must be group or item! Both have name.
	bool operator()(const XMLElementPtr& lhs, const XMLElementPtr& rhs)
	{
		const char* lhsName = GetNodeName(lhs);
		const char* rhsName = GetNodeName(rhs);
		if (!(lhsName && rhsName))
			return false;
		bool lhsGroup = IsGroup(lhs);
		bool rhsGroup = IsGroup(rhs);
		// same type
		if ((lhsGroup && rhsGroup) || (!lhsGroup && !rhsGroup))
			return strcmp(lhsName, rhsName) < 0;
		// group in front
		return lhsGroup;
	};
};

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Group Item and Entry
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
XMLElementPtr CreateNode(const char* tag);
void DeleteNode(XMLElementPtr node);

XMLElementPtr AddChildNode(XMLElementPtr node, XMLElementPtr child);

const char* GetNodeDirectory(XMLElementPtr node, std::string& path);
const char* GetPresentWorkingDirectory(std::string& path);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Group and Item
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
XMLElementPtr GetDirectChildNode(XMLElementPtr node, const char* name);
bool GetChildren(XMLElementPtr node, XMLElementPtrList& nodes);

// Can return parent or self.
XMLElementPtr GetNodeByPath(const std::string& path);
// Can not return parent or self.
XMLElementPtr GetChildNodeByPath(const std::string& path);

XMLElementPtr GetOrCreateChildNode(XMLElementPtr node, const char* tag, const char* name);

void DeleteChildNode(XMLElementPtr node, const char* name);
void DeleteChildren(XMLElementPtr node);

bool IsParent(XMLElementPtr parent, XMLElementPtr child);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Group
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
XMLElementPtr CreateGroupNodeByPath(const std::string& path);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Item
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
XMLElementPtr CreateItemNodeByPath(const std::string& path);
void GetBaseName(const std::string& path, std::string& name);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Item Entry
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
struct Entry
{
	const char* key;
	const char* value;
	int weight;

	Entry() : key(nullptr), value(nullptr), weight(0) {}
	Entry(const char* _k, const char* _v, int _w) : 
		key(_k), value(_v), weight(_w) {}
};

struct EntryCompare
{
	bool operator()(const Entry& lhs, const Entry& rhs)
	{
		if (lhs.weight == rhs.weight)
			return strcmp(lhs.key, rhs.key) < 0;
		else
			return lhs.weight < rhs.weight;
	};
};


typedef std::vector<Entry> EntryList;

// These must be called on an item node.

// if entry is not null, it will store the result.
bool GetEntry(XMLElementPtr node, const char* key, Entry* entry);
bool GetEntries(XMLElementPtr node, EntryList& entries);

XMLElementPtr GetEntryNode(XMLElementPtr node, const char* key);
XMLElementPtr GetEntryNode(XMLElementPtr node, int id);
XMLElementPtr GetOrCreateEntryNode(XMLElementPtr node, const char* key);

// If exists, it will override current one.
bool SetEntry(XMLElementPtr node, const Entry& entry);

#endif