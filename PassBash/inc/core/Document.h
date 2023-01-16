/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Document.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 15, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Password Document.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#include "../common/Macros.h"
#include "../utility/OrderedList.h"
#include "../utility/xml.h"
#include "Entry.h"
#include "MemPool.h"

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

enum class NodeType
{
	UNKNOWN,
	GROUP,
	ITEM
};

DECLARE_CLASS(Node)
DECLARE_CLASS(PassDoc)

/*
** Both group and item are node, but Node shouldn't be instanciated.
** First two children are itself and its parent and shouldn't be deleted.
*/
class Node : protected IntrusiveListValue<NodePtr>
{
	friend class IntrusiveList<NodePtr>;
	friend class PassDoc;
	friend class MemPool<Node>;

public:
	class NodePtrCompare
	{
	public:
		bool operator()(const NodePtr& lhs, const NodePtr& rhs)
		{
			return lhs->m_name < rhs->m_name;
		};
	};

	class NodePtrEqual
	{
	public:
		bool operator()(const NodePtr& lhs, const NodePtr& rhs)
		{
			return lhs->m_name == rhs->m_name;
		};
	};

public:
	using child_container = UniqueOrderedList<NodePtr, NodePtrCompare, NodePtrEqual>;
	using entry_container = OrderedList<EntryPtr, Entry::EntryPtrCompare>;

	const std::string& Name() const { return m_name; }
	NodeType Type() const { return m_type; }

	void Destroy();
	
	NodePtr GetChild    (const std::string& name);
	NodePtr AddChild    (const std::string& name, NodeType type);
	bool    RemoveChild (const std::string& name);
	const auto& GetChildren() const { return m_children.Values(); }
	
	EntryPtr GetEntry    (const std::string& key);
	EntryPtr AddEntry(const std::string& key, const std::string& value, int weight = 0);
	bool     RemoveEntry (const std::string& key);
	const auto& GetEntries() const { return m_entries.Values(); }

	bool IsExpandable() const;

public:
	// Get absolute path name.
	void GetAbsoluteName(std::string& path);

private:
	// These should only be accessed by PasssDoc.
	Node() : m_parent(nullptr), m_doc(nullptr), m_elem(nullptr),
		m_type(NodeType::UNKNOWN) {}
	~Node() {}

	void _Destroy(int depth);
	void _SetDoc(PassDocPtr doc) { m_doc = doc; }
	void _SetParent(NodePtr parent) { m_parent = parent; }
	void _SetName(const std::string& name) { m_name = name; }
	void _SetType(NodeType type) { m_type = type; }
	void _SetXMLElement(XMLElement* elem) { m_elem = elem; }

	bool _IsReserved() const;

private:
	void _InitXMLElement();

protected:
	std::string m_name;
	child_container m_children;
	entry_container m_entries;

private:
	NodePtr m_parent;
	PassDocPtr m_doc;
	XMLElement* m_elem;
	NodeType m_type;
};


/********************************************************************
** Password Document. It is an XML file, and uses TEA algorithm for
** for encryption and decryption.
*/

class PassDoc
{
public:
	PassDoc() : m_root(nullptr), m_current(nullptr), m_isOperational(false) {}
	~PassDoc() {}

	// Load from encrypted file, then decode it.
	bool Load(const std::string& filename, const std::string& password);
	bool Save(const std::string& password);
	bool Save();	// only for debug purpose.

	// Memory allocate.
	NodePtr NewNode();
	void FreeNode(NodePtr pNode);
	EntryPtr NewEntry();
	void FreeEntry(EntryPtr pEntry);
	XMLElement* NewXMLElement(const char* tag);

	// Document structure.
	const NodePtr Root() const { return m_root; }
	const NodePtr Current() const { return m_current; }
	NodePtr SetCurrent(NodePtr current);

public:
	bool IsOperational() const { return m_isOperational; }
	void IsOperational(bool isOperational);

private:
	bool _Build();
	NodePtr _Build(XMLElement* root);

	std::string m_filename;
	XMLFile m_file;

	NodePtr m_root;
	NodePtr m_current;

	bool m_isOperational;

	MemPool<Node> m_nodePool;
	MemPool<Entry> m_entryPool;
};

#endif
