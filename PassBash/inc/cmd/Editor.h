/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Editor.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 25, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For editor.                                                              *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../common/Macros.h"

// edit host
DEC_CMD(_edit);

// clear
DEC_CMD(_edit_clear);
// help
DEC_CMD(_edit_help);
// see
DEC_CMD(_edit_see);
// set
DEC_CMD(_edit_set);
DEC_CMD(_edit_setk);
DEC_CMD(_edit_setv);
DEC_CMD(_edit_setw);
// unset
DEC_CMD(_edit_unset);

// hidden
DEC_CMD(_edit_unknown);
DEC_CMD(_edit_quit);