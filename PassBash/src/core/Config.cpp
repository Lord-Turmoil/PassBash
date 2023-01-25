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
#include "../../inc/utility/Auxiliary.h"

#include <tea.h>

#include <cstdio>


Config::Config() {}

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

	tea::TEABufferReader* reader = new tea::TEABufferReader(g_password);
	tea::TEAFileWriter* writer = new tea::TEAFileWriter(output);
	tea::encode(reader, writer, g_password);
	delete reader;
	delete writer;

	return true;
}