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


#define DECLARE_CLASS(CLASS) \
	class CLASS;             \
	typedef CLASS* CLASS##Ptr;

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

// For unit test.
#define PASH_TEST

#endif