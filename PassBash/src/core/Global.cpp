/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Global.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For all global values.                                                   *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/core/Global.h"
#include "../../inc/core/Config.h"

const char g_PASH_HOST_VERSION[] = "2.1.0";

const char g_default[g_PASSWORD_BUFFER_SIZE] = "null";
char g_password[g_PASSWORD_BUFFER_SIZE];

char g_decodedPassword[g_PASSWORD_BUFFER_SIZE];
char g_encodedPassword[g_PASSWORD_BUFFER_SIZE];

std::string g_pwd { "./" };

PassDoc g_passDoc{};

const char g_TITLE[]     { "PassBash" };
const char g_COPYRIGHT[] { "Tony's Studio 2020 - 2023" };
const char g_AUTHOR[]    { "Tony Skywalker" };

const char g_DATA_FILE[]   { "data" };
const char g_CONFIG_FILE[] { "config" };

Config g_config{};

const char g_DEFAULT_DATA[]
{
R"(<?xml version="1.0" encoding="UTF-8"?>
<Group name=".">
</Group>
)"
};