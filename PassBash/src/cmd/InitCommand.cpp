/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : InitCommand.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 25, 2023                          *
 *                                                                            *
 *                    Last Update : March 10, 2023                            *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Initialize commands.                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/cmd/Functions.h"
#include "../../inc/cmd/Command.h"
#include "../../inc/cmd/Editor.h"
#include "../../inc/cmd/Service.h"


#define HOOKC(FACTORY, CMD) FACTORY##.hook(#CMD, CMD)
#define HOOKD(FACTORY, DESCR, CMD) FACTORY##.hook(DESCR, CMD)

void init_factory()
{
	// general
	HOOKC(g_generalFactory, cd);
	HOOKC(g_generalFactory, ls);
	HOOKD(g_generalFactory, "l", ls);
	HOOKC(g_generalFactory, touch);
	HOOKC(g_generalFactory, mkdir);
	HOOKC(g_generalFactory, rm);
	HOOKC(g_generalFactory, move);
	HOOKD(g_generalFactory, "mv", move);
	HOOKC(g_generalFactory, save);
	HOOKD(g_generalFactory, "s", save);
	HOOKC(g_generalFactory, tree);
	HOOKC(g_generalFactory, cat);
	HOOKC(g_generalFactory, rename);
	HOOKD(g_generalFactory, "rn", rename);
	HOOKC(g_generalFactory, clear);
	HOOKD(g_generalFactory, "cls", clear);
	HOOKD(g_generalFactory, "c", clear);
	HOOKC(g_generalFactory, pwd);
	HOOKC(g_generalFactory, help);
	HOOKD(g_generalFactory, "h", help);
	HOOKC(g_generalFactory, find);
	HOOKD(g_generalFactory, "f", find);
	HOOKC(g_generalFactory, edit);
	HOOKD(g_generalFactory, "mod", edit);
	HOOKC(g_generalFactory, quit);
	HOOKD(g_generalFactory, "q", quit);
	HOOKC(g_generalFactory, order);
	HOOKC(g_generalFactory, change);
	HOOKD(g_generalFactory, "tea", encrypt);
	HOOKC(g_generalFactory, version);
	HOOKD(g_generalFactory, "ver", version);

	// editor
	HOOKC(g_editorFactory, _edit);
	HOOKD(g_editorFactory, "clear", _edit_clear);
	HOOKD(g_editorFactory, "cls", _edit_clear);
	HOOKD(g_editorFactory, "c", _edit_clear);
	HOOKD(g_editorFactory, "help", _edit_help);
	HOOKD(g_editorFactory, "h", _edit_help);
	HOOKD(g_editorFactory, "see", _edit_see);
	HOOKD(g_editorFactory, "s", _edit_see);
	HOOKD(g_editorFactory, "set", _edit_set);
	HOOKD(g_editorFactory, "st", _edit_set);
	HOOKD(g_editorFactory, "setk", _edit_setk);
	HOOKD(g_editorFactory, "sk", _edit_setk);
	HOOKD(g_editorFactory, "setv", _edit_setv);
	HOOKD(g_editorFactory, "sv", _edit_setv);
	HOOKD(g_editorFactory, "setw", _edit_setw);
	HOOKD(g_editorFactory, "sw", _edit_setw);
	HOOKD(g_editorFactory, "unset", _edit_unset);
	HOOKD(g_editorFactory, "ust", _edit_unset);
	HOOKD(g_editorFactory, "quit", _edit_quit);
	HOOKD(g_editorFactory, "q", _edit_quit);
	HOOKD(g_hiddenFactory, "_edit_unknown", _edit_unknown);

	// hidden
	HOOKC(g_hiddenFactory, start);
	HOOKC(g_hiddenFactory, reg);
	HOOKC(g_hiddenFactory, login);
	HOOKC(g_hiddenFactory, host);
	HOOKC(g_hiddenFactory, unknown);
}
