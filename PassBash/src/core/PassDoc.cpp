/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : PassDoc.cpp                               *
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

#include "../../inc/core/PassDoc.h"
#include "../../inc/core/Global.h"
#include "../../inc/common/Logger.h"
#include "../../inc/utility/Auxilliary.h"

#include <tea.h>
#include <algorithm>
#include <stack>


PassDoc::PassDoc() : m_root(nullptr), m_current(nullptr), m_modified(false) {}

PassDoc::~PassDoc()
{
	if (m_modified)
		Save(g_password.c_str());
}

bool PassDoc::Load(const char* password)
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

	fseek(input, 0, SEEK_END);
	char* xml = new char[ftell(input) / 2 + 128];
	fseek(input, 0, SEEK_SET);

	tea::TEAFileReader* reader = new tea::TEAFileReader(input);
	tea::TEABufferWriter* writer = new tea::TEABufferWriter(xml);
	tea::decode(reader, writer, password);
	delete reader;
	delete writer;

	// Now xml file is in xml buffer!
	if (!m_file.Parse(xml))
	{
		LOG_ERROR("\t|- Perhaps wrong password?");
		LOG_ERROR("Failed to load data!");
		return false;
	}
	delete[] xml;

	m_current = m_root = m_file.GetRoot();

	return true;
}

void PassDoc::UnLoad()
{
	m_file.UnLoad();
}


bool PassDoc::Save(const char* password)
{
	if (!IsLoaded())
	{
		LOG_ERROR("Attempt to save before load!");
		return false;
	}

	XMLPrinter stream(0, true, 0);
	m_file.Doc().Print(&stream);

	FILE* output;
	if (fopen_s(&output, g_DATA_FILE, "wb") != 0)
	{
		LOG_ERROR("Failed to open file \"%s\"", g_DATA_FILE);
		return false;
	}

	tea::TEABufferReader* reader = new tea::TEABufferReader(stream.CStr());
	tea::TEAFileWriter* writer = new tea::TEAFileWriter(output);
	tea::encode(reader, writer, password);
	delete reader;
	delete writer;

	m_modified = false;

	return true;
}

bool PassDoc::Save()
{
#ifdef PASH_DEBUG
	m_file.Save("debug.xml");
	return true;
#else
	LOG_ERROR("A pathetic attempt.");
	return false;
#endif
}


bool PassDoc::IsLoaded() const
{
	return m_file.IsLoaded();
}

XMLElementPtr PassDoc::SetCurrent(XMLElementPtr current)
{
	if (!current)
		return m_current;

	const char* name = current->Name();
	if (_STR_DIFF(name, GROUP_TAG))
		return nullptr;

	XMLElementPtr ret = m_current;
	m_current = current;

	return ret;
}

XMLElementPtr PassDoc::NewElement(const char* name)
{
	if (!IsLoaded())
		return nullptr;

	return m_file.Doc().NewElement(name);
}

void PassDoc::DeleteElement(XMLElementPtr node)
{
	m_file.Doc().DeleteNode(node);
}

void PassDoc::GetPresentWorkingDirectory(std::string& path)
{
	GetWorkingDirectory(m_current, path);
}

void PassDoc::GetWorkingDirectory(XMLElementPtr node, std::string& path)
{
	std::stack<XMLElementPtr> chain;
	
	while (node != m_root)
	{
		chain.push(node);
		node = GetNodeParent(node);
	}
	chain.push(node);

	path = "";
	while (!chain.empty())
	{
		node = chain.top();
		chain.pop();
		path += GetNodeName(node);
		if (IsGroup(node))
			path += '/';
	}
}

void PassDoc::Mark()
{
	m_modified = true;
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


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Utilities
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
XMLElementPtr CreateNode(const char* tag)
{
	return g_passDoc.NewElement(tag);
}

XMLElementPtr GetDirectChildNode(XMLElementPtr node, const char* name)
{
	if (!name)
		return nullptr;
	if (!IsGroup(node))
		return nullptr;

	XMLElementPtr p = node->FirstChildElement();
	while (p)
	{
		if (_STR_SAME(GetNodeName(p), name))
			return p;
		p = p->NextSiblingElement();
	}

	return nullptr;
}

bool GetChildren(XMLElementPtr node, XMLElementPtrList& nodes)
{
	if (node->NoChildren())
		return true;

	nodes.clear();

	XMLElementPtr p = node->FirstChildElement();
	while (p)
	{
		nodes.push_back(p);
		p = p->NextSiblingElement();
	}
	
	std::sort(nodes.begin(), nodes.end(), XMLElementPtrCompare());

	return true;
}

XMLElementPtr AddChildNode(XMLElementPtr node, XMLElementPtr child)
{
	node->InsertEndChild(child);
	return child;
}

XMLElementPtr GetNodeByPath(const std::string& path)
{
	char* buffer = _strdup(path.c_str());
	char* context = nullptr;

	if (_STR_SAME(buffer, ROOT_DIR_NAME))
		return g_passDoc.GetRoot();

	// Absolute or relative.
	XMLElementPtr node;
	if (buffer[0] == '.' && buffer[1] == '/')
		node = g_passDoc.GetRoot();
	else
		node = g_passDoc.GetCurrent();

	char* token = strtok_s(buffer, "/", &context);
	if (!token)
		return nullptr;

	while (node)
	{
		if (_STR_SAME(token, PARENT_DIR_NAME))
		{
			if (node != g_passDoc.GetRoot())
				node = GetNodeParent(node);
		}
		else if (_STR_SAME(token, SELF_DIR_NAME))
			node = node;
		else
			node = GetDirectChildNode(node, token);
		token = strtok_s(nullptr, "/", &context);
		if (!token)
			break;
	}
	free(buffer);	// strdup

	return node;
}

XMLElementPtr GetChildNodeByPath(const std::string& path)
{
	char* buffer = _strdup(path.c_str());
	char* context = nullptr;

	if (_STR_SAME(buffer, ROOT_DIR_NAME))
		return nullptr;

	// Absolute or relative.
	bool isAbsolute = false;
	if (buffer[0] == '.' && buffer[1] == '/')
		isAbsolute = true;
	
	XMLElementPtr current = g_passDoc.GetCurrent();
	XMLElementPtr node = isAbsolute ? g_passDoc.GetRoot() : current;

	char* token = strtok_s(buffer, "/", &context);
	if (!token)
	{
		free(buffer);
		return nullptr;
	}

	bool isChild = !isAbsolute;
	while (node)
	{
		if (_STR_SAME(token, PARENT_DIR_NAME))
		{
			node = nullptr;
			break;
		}
		else if (_STR_SAME(token, SELF_DIR_NAME))
			node = node;
		else
		{
			if (isAbsolute && (node == current))
				isChild = true;
			node = GetDirectChildNode(node, token);
		}

		token = strtok_s(nullptr, "/", &context);
		if (!token)
			break;
	}
	free(buffer);	// strdup

	if (isChild)
		return node;
	else
		return nullptr;
}


XMLElementPtr GetOrCreateChildNode(XMLElementPtr node, const char* tag, const char* name)
{
	XMLElementPtr child = GetDirectChildNode(node, name);
	if (child)
		return child;

	child = g_passDoc.NewElement(tag);
	child->SetAttribute("name", name);
	node->InsertEndChild(child);

	return child;
}

XMLElementPtr CreateGroupNodeByPath(const std::string& path)
{
	char* buffer = _strdup(path.c_str());
	char* context = nullptr;

	if (_STR_SAME(buffer, ROOT_DIR_NAME))
		return g_passDoc.GetRoot();

	// Absolute or relative.
	XMLElementPtr node;
	if (buffer[0] == '.' && buffer[1] == '/')
		node = g_passDoc.GetRoot();
	else
		node = g_passDoc.GetCurrent();

	char* token = strtok_s(buffer, "/", &context);
	while (token)
	{
		if (_STR_SAME(token, PARENT_DIR_NAME))
		{
			if (node != g_passDoc.GetRoot())
				node = GetNodeParent(node);
		}
		else if (_STR_SAME(token, SELF_DIR_NAME))
			node = node;
		else
			node = GetOrCreateChildNode(node, GROUP_TAG, token);

		token = strtok_s(nullptr, "/", &context);
	}
	free(buffer);	// strdup

	return node;
}

void DeleteNode(XMLElementPtr node)
{
	g_passDoc.DeleteElement(node);
}



bool GetEntry(XMLElementPtr node, const char* key, Entry* entry)
{
	if (!IsItem(node))
		return false;

	XMLElementPtr p = node->FirstChildElement();
	while (p)
	{
		if (_STR_SAME(GetNodeAttr(p, "key"), key))
			break;
		p = p->NextSiblingElement();
	}

	if (p)
	{
		if (entry)
		{
			entry->key = key;
			entry->value = p->Attribute("value");
			entry->weight = p->IntAttribute("weight");
		}
		return true;
	}

	return false;
}

bool GetEntries(XMLElementPtr node, EntryList& entries)
{
	if (!IsItem(node))
		return false;

	entries.clear();

	if (node->NoChildren())
		return true;

	XMLElementPtr p = node->FirstChildElement();
	while (p)
	{
		entries.emplace_back(
			p->Attribute("key"),
			p->Attribute("value"),
			p->IntAttribute("weight"));

		p = p->NextSiblingElement();
	}

	std::sort(entries.begin(), entries.end(), EntryCompare());

	return true;
}

XMLElementPtr GetOrCreateEntryNode(XMLElementPtr node, const char* key)
{
	if (!IsItem(node))
		return nullptr;

	XMLElementPtr p = node->FirstChildElement();
	while (p)
	{
		if (_STR_SAME(GetNodeAttr(p, "key"), key))
			break;
		p = p->NextSiblingElement();
	}

	if (!p)
	{
		p = CreateNode(ENTRY_TAG);
		p->SetAttribute("key", key);
	}

	return p;
}

bool SetEntry(XMLElementPtr node, const Entry& entry)
{
	XMLElementPtr child = GetOrCreateEntryNode(node, entry.key);
	if (!child)
		return false;

	child->SetAttribute("value", entry.value);
	child->SetAttribute("weight", entry.weight);

	return true;
}