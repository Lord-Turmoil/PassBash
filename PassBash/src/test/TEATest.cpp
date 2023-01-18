/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : TEATest.cpp                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Test for TEA algorithm.                                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/common/Test.h"

#if TEST(TEA)

#include <tea.h>
#include <cnsl.h>
#include <cstdio>

#pragma warning(disable: 4996)

char ORIGINAL_FILE[32];
char ENCODE_FILE[32];
char DECODE_FILE[32];

const char PASSWORD[24] = "Passworld";

bool encode();
bool decode();
bool bufferDecode();
int main()
{
	cnsl::InitConsole(110);
	cnsl::SetTitle("TEA Utility");
	cnsl::Print();

	cnsl::InsertText("Original file: ");
	cnsl::GetString(ORIGINAL_FILE, 1, 30);
	cnsl::InsertText("\n  Encode file: ");
	cnsl::GetString(ENCODE_FILE, 1, 30);
	cnsl::InsertText("\n  Decode file: ");
	cnsl::GetString(DECODE_FILE, 1, 30);
	cnsl::InsertNewLine();
	encode();
	decode();
	// bufferDecode();

	return 0;
}

bool encode()
{
	FILE* input = fopen(ORIGINAL_FILE, "rb");
	if (!input)
	{
		cnsl::InsertText("Failed to open %s\n", ORIGINAL_FILE);
		return false;
	}
	FILE* output = fopen(ENCODE_FILE, "wb");
	if (!output)
	{
		cnsl::InsertText("Failed to open %s\n", ENCODE_FILE);
		return false;
	}

	tea::TEAFileReader* reader = new tea::TEAFileReader(input);
	tea::TEAFileWriter* writer = new tea::TEAFileWriter(output);
	tea::encode(reader, writer, PASSWORD);
	delete reader;
	delete writer;

	cnsl::InsertText("Encryption complete!\n");

	return true;
}

bool decode()
{
	FILE* input = fopen(ENCODE_FILE, "rb");
	if (!input)
	{
		cnsl::InsertText("Failed to open %s\n", ENCODE_FILE);
		return false;
	}
	FILE* output = fopen(DECODE_FILE, "wb");
	if (!output)
	{
		cnsl::InsertText("Failed to open %s\n", DECODE_FILE);
		return false;
	}

	tea::TEAFileReader* reader = new tea::TEAFileReader(input);
	tea::TEAFileWriter* writer = new tea::TEAFileWriter(output);
	tea::decode(reader, writer, PASSWORD);
	delete reader;
	delete writer;

	cnsl::InsertText("Decryption complete!\n");

	return true;
}

bool bufferDecode()
{
	FILE* input = fopen(ENCODE_FILE, "rb");
	if (!input)
	{
		cnsl::InsertText("Failed to open %s\n", ENCODE_FILE);
		return false;
	}
	fseek(input, 0, SEEK_END);
	size_t size = ftell(input) + 128;
	char* buffer = new char[size];
	fseek(input, 0, SEEK_SET);

	tea::TEAFileReader* reader = new tea::TEAFileReader(input);
	tea::TEABufferWriter* writer = new tea::TEABufferWriter(buffer);
	tea::decode(reader, writer, PASSWORD);
	delete reader;
	delete writer;

	cnsl::InsertText("Decryption complete!\n");
	for (char* p = buffer; *p; p++)
		cnsl::InsertChar(*p);
	delete buffer;

	return true;
}

#endif
