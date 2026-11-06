/*
 *  JLib - Jacob's Library.
 *  Copyright (C) 2003, 2004  Juan Carlos Seijo Pérez
 * 
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 * 
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 * 
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 *  Juan Carlos Seijo Pérez
 *  jacob@mainreactor.net
 */

/** Definitions for cross-platform compatibility.
 * @file    JCompatibility.h
 * @author  Juan Carlos Seijo Pérez
 * @date    09/01/2004
 * @version 0.0.1 - 09/01/2004 - First version.
 */

#ifndef _JCOMPATIBILITY_INCLUDED
#define _JCOMPATIBILITY_INCLUDED

#include <JLib/Util/JTypes.h>
#ifdef _WIN32
#define sleep       _sleep
#define mode_t      u16
#ifndef S_IFBLK
#define S_IFBLK     0
#endif
#ifndef S_IFLNK
#define S_IFLNK     0
#endif
#endif

#endif  // _JCOMPATIBILITY_INCLUDED
