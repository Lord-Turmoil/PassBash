/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : PassDocTest.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   PassDoc test.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/common/Test.h"

#if TEST(PASSDOC)

#include "../../inc/core/PassDoc.h"
#include "../../inc/core/Global.h"


const char PASSWORD[] = "Passworld";
const char DATA_FILE[] = "data";
const char DECODE_FILE[] = "decode.xml";
int main()
{
	PassDoc doc;

	doc.Load(g_default.c_str());
	doc.Save();

	return 0;
}

#endif