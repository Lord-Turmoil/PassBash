/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Document.cpp                              *
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

#if 0

#include "../../inc/core/Document.h"
#include "../../inc/core/Global.h"
#include "../../inc/common/Logger.h"
#include "../../inc/utility/Auxilliary.h"

#include <tea.h>

#include <stack>


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Node
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

void Node::Destroy()
{
	_Destroy(0);
}

void Node::_Destroy(int depth)
{
	for (auto child : m_children.Values())
		child->_Destroy(depth + 1);
	for (auto entry : m_entries.Values())
		m_doc->FreeEntry(entry);
	if (depth == 0)
		m_parent->RemoveChild(this->Name());
	m_doc->FreeNode(this);
}

/********************************************************************
** Get direct child with given name. If no such child, nullptr will
** be returned.
*/
NodePtr Node::GetChild(const std::string& name)
{
	for (auto child : m_children.Values())
	{
		if (child->Name() == name)
			return child;
	}
	return nullptr;
}

/********************************************************************
** If child with the name already exsists, it won't add one and will
** return nullptr.
*/
NodePtr Node::AddChild(const std::string& name, NodeType type)
{
	if (!IsExpandable())
	{
		LOG_ERROR(LOG_LOCATION "Cannot add child to non-expandable node!",
			m_name.c_str());
		return nullptr;
	}
	if (GetChild(name) != nullptr)
	{
		LOG_ERROR(LOG_LOCATION "Duplicated child node name - \"%s\"!",
			m_name.c_str(), name.c_str());
		return nullptr;
	}
	NodePtr node = m_doc->NewNode();
	node->_SetName(name);
	node->_SetParent(this);
	node->_SetType(type);
	node->_InitXMLElement();

	m_children.Insert(node);

	return node;
}

bool Node::RemoveChild(const std::string& name)
{
	NodePtr child = GetChild(name);
	if (!child)
	{
		LOG_ERROR(LOG_LOCATION "No such child named \"%s\" to remove",
			m_name.c_str(), name.c_str());
		return false;
	}
	if (child->_IsReserved())
	{
		LOG_ERROR(LOG_LOCATION "Cannot remove reserved node \"%s\"!",
			m_name.c_str(), name.c_str());
		return false;
	}
	m_elem->DeleteChild(GetElementByAttrName(m_elem, "name", name.c_str()));
	m_children.Erase(child);

	return true;
}

EntryPtr Node::GetEntry(const std::string& key)
{
	for (auto entry : m_entries.Values())
	{
		if (entry->Key() == key)
			return entry;
	}

	return nullptr;
}

EntryPtr Node::AddEntry(const std::string& key, const std::string& value, int weight)
{
	if (GetEntry(key) != nullptr)
	{
		LOG_ERROR(LOG_LOCATION "Duplicated entry key - \"%s\"",
			m_name.c_str(), key.c_str());
		return nullptr;
	}

	EntryPtr entry = m_doc->NewEntry();
	entry->Key(key);
	entry->Value(value);
	entry->Weight(weight);
	m_entries.Insert(entry);
	
	XMLElement* elem = m_doc->NewXMLElement("Entry");
	elem->SetAttribute("key", key.c_str());
	elem->SetAttribute("value", value.c_str());
	m_elem->InsertEndChild(elem);

	return entry;
}

bool Node::RemoveEntry(const std::string& key)
{
	EntryPtr entry = GetEntry(key);
	if (!entry)
	{
		LOG_ERROR(LOG_LOCATION "No such entry named \"%s\" to remove",
			m_name.c_str(), key.c_str());
		return false;
	}

	m_elem->DeleteChild(GetElementByAttrName(m_elem, "key", key.c_str()));
	m_entries.Erase(entry);

	return true;
}

bool Node::IsExpandable() const
{
	if (m_type == NodeType::GROUP)
		return true;
	else
		return false;
}

void Node::GetAbsoluteName(std::string& path)
{
	std::stack<NodePtr> chain;
	NodePtr p = this;

	while (p->m_parent != p)
	{
		chain.push(p);
		p = p->m_parent;
	}
	chain.push(p);

	path = "";
	while (!chain.empty())
	{
		p = chain.top();
		chain.pop();
		path += p->Name();
		if (p->IsExpandable())
			path += '/';
	}
}

bool Node::_IsReserved() const
{
	static const std::string RESERVED[] = { ".", ".." };

	for (int i = 0; i < 2; i++)
	{
		if (m_name == RESERVED[i])
			return true;
	}
	
	return false;
}

void Node::_InitXMLElement()
{
	if (m_type == NodeType::GROUP)
		m_elem = m_doc->NewXMLElement("Group");
	else
		m_elem = m_doc->NewXMLElement("Item");
	m_elem->SetAttribute("name", m_name.c_str());
	m_parent->m_elem->InsertEndChild(m_elem);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** PassDoc
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
PassDoc::~PassDoc()
{
	Save(g_password);
}

void PassDoc::Reset()
{
	m_filename = "";
	m_file.UnLoad();

	m_nodePool.Destroy();
	m_entryPool.Destroy();
}

bool PassDoc::Load(const std::string& password)
{
	FILE* input;
	if (fopen_s(&input, g_DATA_FILE, "rb") != 0)
	{
		if (!_GenerateData())
		{
			LOG_ERROR("Essential data crashed!");
			return false;
		}
		else if (fopen_s(&input, g_DATA_FILE, "rb") != 0)
		{
			LOG_ERROR("Missing file \"%s\"", g_DATA_FILE);
			return false;
		}
	}

	m_filename = g_DATA_FILE;

	fseek(input, 0, SEEK_END);
	char* xml = new char[ftell(input) / 2 + 128];
	fseek(input, 0, SEEK_SET);

	tea::TEAFileReader* reader = new tea::TEAFileReader(input);
	tea::TEABufferWriter* writer = new tea::TEABufferWriter(xml);
	tea::decode(reader, writer, password.c_str());
	delete reader;
	delete writer;

	// Now xml file is in xml buffer!
	if (!m_file.Parse(xml))
	{
		LOG_ERROR("\t|- Perhaps wrong password?");
		LOG_ERROR("Failed to load data!");
		return false;
	}

	if (!_Build())
	{
		LOG_ERROR("Failed to initialize!");
		return false;
	}

	return true;
}

bool PassDoc::Save(const std::string& password)
{
	if (m_filename == "")
	{
		LOG_ERROR("Attempt to save before load!");
		return false;
	}

	XMLPrinter stream(0, true, 0);
	m_file.Doc().Print(&stream);
	
	FILE* output;
	if (fopen_s(&output, m_filename.c_str(), "wb") != 0)
	{
		LOG_ERROR("Failed to open file \"%s\"", m_filename.c_str());
		return false;
	}

	tea::TEABufferReader* reader = new tea::TEABufferReader(stream.CStr());
	tea::TEAFileWriter* writer = new tea::TEAFileWriter(output);
	tea::encode(reader, writer, password.c_str());
	delete reader;
	delete writer;

	return true;
}

bool PassDoc::Save()	// only for debug purpose.
{
#ifdef PASH_DEBUG
	m_file.Save("debug.xml");
#endif

	return true;
}

NodePtr PassDoc::NewNode()
{
	NodePtr node = m_nodePool.Alloc();
	node->_SetDoc(this);
	return node;
}

void PassDoc::FreeNode(NodePtr pNode)
{
	m_nodePool.Free(pNode);
}

EntryPtr PassDoc::NewEntry()
{
	return m_entryPool.Alloc();
}

void PassDoc::FreeEntry(EntryPtr pEntry)
{
	m_entryPool.Free(pEntry);
}

XMLElement* PassDoc::NewXMLElement(const char* tag)
{
	return m_file.Doc().NewElement(tag);
}

NodePtr PassDoc::SetCurrent(NodePtr current)
{
	NodePtr prev = m_current;

	if (current->IsExpandable())
		m_current = current;
	else
		LOG_ERROR("Current node must be expandable!");

	return prev;
}

bool PassDoc::_GenerateData()
{
	FILE* output;
	if (fopen_s(&output, g_DATA_FILE, "wb") != 0)
	{
		LOG_ERROR("Cannot open file \"%s\"", g_DATA_FILE);
		return false;
	}

	tea::TEABufferReader* reader = new tea::TEABufferReader(g_DEFAULT_DATA);
	tea::TEAFileWriter* writer = new tea::TEAFileWriter(output);
	tea::encode(reader, writer, g_default.c_str());
	delete reader;
	delete writer;

	return true;
}

bool PassDoc::_Build()
{
	XMLElement* elem = m_file.GetRoot();

	m_root = NewNode();
	m_root->_SetParent(m_root);
	m_root->_SetName(".");
	m_root->_SetType(NodeType::GROUP);
	m_root->_SetXMLElement(elem);

	XMLElement* tag = elem->FirstChildElement();
	while (tag)
	{
		NodePtr node = _Build(tag);
		node->_SetParent(m_root);
		m_root->m_children.Insert(node);
		tag = tag->NextSiblingElement();
	}

	m_current = m_root;

	return !HAS_ERROR();
}

NodePtr PassDoc::_Build(XMLElement* root)
{
	const char* type = root->Name();
	NodePtr node = NewNode();

	if (_STR_SAME(type, "Group"))
		node->_SetType(NodeType::GROUP);
	else if (_STR_SAME(type, "Item"))
		node->_SetType(NodeType::ITEM);
	else
	{
		LOG_ERROR("Unknown tag name - \"%s\"", type);
		return nullptr;
	}

	node->_SetName(root->Attribute("name"));
	node->_SetXMLElement(root);

	if (node->m_type == NodeType::ITEM)
	{
		auto entries = GetElementsByTagName(root, "Entry");
		for (auto entry : entries)
		{
			node->AddEntry(
				entry->Attribute("key"),
				entry->Attribute("value"),
				entry->IntAttribute("weight"));
		}
	}
	else
	{
		XMLElement* tag = root->FirstChildElement();
		while (tag)
		{
			NodePtr child = _Build(tag);
			child->_SetParent(node);
			node->m_children.Insert(child);
			tag = tag->NextSiblingElement();
		}
	}

	return node;
}

#endif