/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : TeaCommand.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : February 10, 2023                         *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For tea encryption service.                                              *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/FunctionUtil.h"

#include <tea.h>
#include <io.h>


// 0 for encode, 1 for decode.
static int mode = 0;
static std::string password{ "" };
static char buffer[g_PASSWORD_BUFFER_SIZE];

static void _encrypt_init()
{
	mode = 0;
	password = "";
}

static void _encrypt_usage()
{
	cnsl::InsertText(MESSAGE_COLOR, "Usage: tea [-d] <input file> <output file> [-p password]\n");
	cnsl::InsertText(MESSAGE_COLOR, "  -d -- to decode file\n");
	cnsl::InsertText(MESSAGE_COLOR, "This is an extra tool, which makes real world effects to your files.\n");
}

static int _encrypt_parse_args(int argc, char* argv[], std::string& src, std::string& dst)
{
	int opt;
	int cnt = 0;
	int err = 0;
	while (opt = getopt(argc, argv, "dp:"))
	{
		switch (opt)
		{
		case 'd':
			mode = 1;
			break;
		case 'p':
			if (opterr == 0)
			{
				if (password == "")
					password = optarg;
				else
					err = 1;
			}
			else
				cnsl::InsertText(ERROR_COLOR, "Error: %s.\n", optmsg);
			break;
		case '!':
			cnt++;
			if (cnt == 1)
				src = optarg;
			else if (cnt == 2)
				dst = optarg;
			break;
		case '?':
			cnsl::InsertText(ERROR_COLOR, "Unknown parameter \"-%c\"\n", optopt);
			break;
		default:
			break;
		}
	}
	if (err || (cnt != 2))
	{
		cnsl::InsertText(ERROR_COLOR, ARGUMENTS_ILLEGAL);
		cnsl::InsertNewLine();
		return 1;
	}

	return 0;
}

static int _open_files(FILE** pfin, const char* src, FILE** pfout, const char* dst)
{
	if (_access(dst, 00) == 0)
	{
		char response[4];
		cnsl::InsertText(MESSAGE_COLOR, "Output file already exists, overwrite? (Y/N)\n");
		cnsl::InsertText(PROMPT_COLOR, "$ ");
		cnsl::GetString(response, 1, 1);
		cnsl::InsertNewLine();
		if (tolower(response[0]) != 'y')
			return -1;	// abort
	}

	if (fopen_s(pfin, src, "rb") != 0)
	{
		cnsl::InsertText(ERROR_COLOR, "Could not open input file!\n");
		return 2;
	}
	if (fopen_s(pfout, dst, "wb") != 0)
	{
		cnsl::InsertText(ERROR_COLOR, "Could not open output file!\n");
		return 3;
	}

	return 0;
}

static void _close_files(FILE* fin, FILE* fout)
{
	if (fin)
		fclose(fin);
	if (fout)
		fclose(fout);
}

// Resouce management should be applied. Currently, this is not safe.
static int _enctypt(FILE* fin, FILE* fout)
{
	tea::TEAFileReader* reader = new tea::TEAFileReader(fin);
	tea::TEAFileWriter* writer = new tea::TEAFileWriter(fout);
	if (mode == 0)
		tea::encode(reader, writer, buffer);
	else if (mode == 1)
		tea::decode(reader, writer, buffer);
	else
		return 1;
	delete reader;
	delete writer;

	return 0;
}

DEC_CMD(encrypt)
{
	std::string src;
	std::string dst;

	_encrypt_init();
	if (_encrypt_parse_args(argc, argv, src, dst) != 0)
	{
		_encrypt_usage();
		return 1;
	}
	if (src == dst)
	{
		cnsl::InsertText(ERROR_COLOR, "Input and output must not be the same!\n");
		return 2;
	}

	// initialize password
	memset(buffer, ' ', sizeof(buffer));
	strncpy_s(buffer, password.c_str(), g_PASSWORD_BUFFER_SIZE);

	// open files
	FILE* fin = nullptr;
	FILE* fout = nullptr;

	int ret = _open_files(&fin, src.c_str(), &fout, dst.c_str());
	if (ret > 0)
	{
		_close_files(fin, fout);

		cnsl::InsertText(ERROR_COLOR, "Failed to open files!\n");
		cnsl::InsertText(MESSAGE_COLOR, "Tips: You may need absolute path.\n");
		
		return 3;
	}
	else if (ret == -1)
	{
		_close_files(fin, fout);
		cnsl::InsertText(MESSAGE_COLOR, "Encryption aborted.\n");
		return 0;
	}

	if (_enctypt(fin, fout) != 0)
	{
		cnsl::InsertText(ERROR_COLOR, "Failed to %s file.\n", (mode == 0) ? "encrypt" : "decrypt");
		_close_files(fin, fout);
		return 4;
	}

	cnsl::InsertText(MESSAGE_COLOR, "%s complete.\n", (mode == 0) ? "Encryption" : "Decryption");
	
	// if succeeded, fin and fout will be closed by tea reader and writer.

	return 0;
}