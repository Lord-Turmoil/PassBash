/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Config.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update : January 20, 2023                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Well, for configuration load.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/core/Config.h"
#include "../../inc/core/Global.h"
#include "../../inc/common/Logger.h"
#include "../../inc/utility/Auxilliary.h"

#include <tea.h>

#include <cstdio>


char Config::m_internal[24];

Config::Config()
{
	GenerateInternalPassword();
}

Config::~Config()
{
	Save();
}

bool Config::Load()
{
	FILE* input;
	if (fopen_s(&input, g_CONFIG_FILE, "rb") != 0)
	{
		LOG_ERROR("Failed to open file \"%s\"", g_CONFIG_FILE);
		return false;
	}

	size_t size;
	fseek(input, 0, SEEK_END);
	size = ftell(input);
	fseek(input, 0, SEEK_SET);
	char buffer[32];

	tea::TEAFileReader* reader = new tea::TEAFileReader(input);
	tea::TEABufferWriter* writer = new tea::TEABufferWriter(g_encodedPassword);
	while (reader->Read(buffer, 8))
		writer->Write(buffer, 8);
	delete reader;
	delete writer;

	return true;
}

bool Config::Save()
{
	FILE* output;
	if (fopen_s(&output, g_CONFIG_FILE, "wb") != 0)
	{
		LOG_ERROR("Failed to open file \"%s\"", g_CONFIG_FILE);
		return false;
	}

	tea::TEABufferReader* reader = new tea::TEABufferReader(g_password.c_str());
	tea::TEAFileWriter* writer = new tea::TEAFileWriter(output);
	tea::encode(reader, writer, g_password.c_str());
	delete reader;
	delete writer;

	return true;
}

// This will hide the true password - NeWdEsIrEsTuDiOs
// Or... At least you cannot find it directly from exe file by
// open it with binary format. :P
// 2023/01/20 TS: Abandoned this method, use password to encrypt
// password.
char* Config::GenerateInternalPassword()
{
	m_internal[0]  = 'M' + 1;
	m_internal[1]  = 'd' + 1;
	m_internal[2]  = 'V' + 1;
	m_internal[3]  = 'c' + 1;
	m_internal[4]  = 'D' + 1;
	m_internal[5]  = 'r' + 1;
	m_internal[6]  = 'H' + 1;
	m_internal[7]  = 'q' + 1;
	m_internal[8]  = 'D' + 1;
	m_internal[9]  = 'r' + 1;
	m_internal[10] = 'S' + 1;
	m_internal[11] = 't' + 1;
	m_internal[12] = 'C' + 1;
	m_internal[13] = 'h' + 1;
	m_internal[14] = 'N' + 1;
	m_internal[15] = 'r' + 1;

	return m_internal;
}