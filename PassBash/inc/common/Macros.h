/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Macros.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 15, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Macros.                                                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _MACROS_H_
#define _MACROS_H_

#include <memory>

#define DECLARE_CLASS(CLASS) \
	class CLASS;             \
	typedef CLASS* CLASS##Ptr;

#define DECLARE_SMART_CLASS(CLASS) \
	class CLASS;                   \
	typedef std::shared_ptr<CLASS> CLASS##Ptr;


#define BIT(X) (1 << (X))

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Command
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
typedef int (*Command)(int, char* []);
#define DEC_CMD(CMD) int CMD##(int argc, char* argv[])

const int CMD_BUFFER_SIZE = 256;
const int CMD_ARG_SIZE = 64;		// maximum 64 arguments

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Debug related.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
#if defined(DEBUG) || defined(_DEBUG)
#define PASH_DEBUG
#endif

#ifdef PASH_DEBUG
#include <cassert>
#endif

#ifdef PASH_DEBUG
#define PASH_ASSERT(...) assert(__VA_ARGS__)
#else
#define PASH_ASSERT(...)
#endif

// For cheat save.
#define PASH_CHEAT

// For unit test.
#define PASH_TEST 0


#endif