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
 *  License along with this library; if not, write to the Free
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 *  Juan Carlos Seijo Pérez
 *  jacob@mainreactor.net
 */

/** JLib's types definition file.
 * @file JTypes.h
 * @author Juan Carlos Seijo Pérez
 * @date 01/04/2003
 * @version 0.0.1 - 01/04/2003 - First version.
 */
#ifndef _JTYPES_INCLUDED
#define _JTYPES_INCLUDED

#include  <limits.h>

#ifdef		_WIN32
/** Macro para los tipos de 64 bits en constante para windows. */
#define	S64(X)	X##i64
/** Macro para los tipos de 64 bits en constante para windows. */
#define	U64(X)	X##ui64
#else
/** Macro para los tipos de 64 bits en constante para no-windows. */
#define	S64(X)	X##LL
/** Macro para los tipos de 64 bits en constante para no-windows. */
#define	U64(X)	X##ULL
#endif

typedef           char    s8;
typedef unsigned  char    u8;
typedef           short   s16;
typedef unsigned  short   u16;
typedef           int     s32;
typedef unsigned  int     u32;

#ifdef _WIN32
typedef           __int64 s64;
typedef unsigned  __int64 u64;
#else
typedef          long long int s64;
typedef unsigned long long int u64;
#endif

#define S8_MIN           ((s8)(0x80))
#define S8_MAX           ((s8)(0x7F))
#define U8_MIN           ((u8)(0x00))
#define U8_MAX           ((u8)(0xFF))
#define S16_MIN          ((s16)(0x8000))
#define S16_MAX          ((s16)(0x7FFF))
#define U16_MIN          ((u16)(0x0000))
#define U16_MAX          ((u16)(0xFFFF))
#define S32_MIN          ((s32)(0x80000000))
#define S32_MAX          ((s32)(0x7FFFFFFF))
#define U32_MIN          ((u32)(0x00000000))
#define U32_MAX          ((u32)(0xFFFFFFFF))
#define S64_MIN          (S64(0x8000000000000000))
#define S64_MAX          (S64(0x7FFFFFFFFFFFFFFF))
#define U64_MIN          (U64(0x0000000000000000))
#define U64_MAX          (U64(0xFFFFFFFFFFFFFFFF))

// Casts to and from void * avoiding compiler warnings
// This is absolutely tricky and a different approach should be used if possible
// It's intended to be used when you want to pass a void* that contain a value instead
// of and address (i.e. as parameter of callback functions that accept a void* as additional data).
// Be aware of the fact that YOU CAN ONLY USE THE MACROS WITH A NUMBER LOWER OR EQUAL TO THE
// BITS OF THE UNDERLYING ARCHITECTURE YOU COMPILE FOR. That is, if you intend to compile only for a 32-bits
// architecture, you can use all of them. If your target architecture is 16-bit based, you could only
// use the 8bit and 16bit macros listed here, and using any of the 32-bit macros in this case could
// translate in a segmentation fault during execution. Notice that he 'double' and 'long' types are normally 
// twice the size of the architecture
#define JCAST_8_TO_VOIDPTR(val)  ((void *)size_t(*((u8 *)&(val))))
#define JCAST_16_TO_VOIDPTR(val) ((void *)size_t(*((u16 *)&(val))))
#define JCAST_32_TO_VOIDPTR(val) ((void *)size_t(*((u32 *)&(val))))
#define JCAST_64_TO_VOIDPTR(val) ((void *)size_t(*((u64 *)&(val))))

#define JCAST_S8_TO_VOIDPTR(val) JCAST_8_TO_VOIDPTR((val))
#define JCAST_U8_TO_VOIDPTR(val) JCAST_8_TO_VOIDPTR((val))
#define JCAST_S16_TO_VOIDPTR(val) JCAST_16_TO_VOIDPTR((val))
#define JCAST_U16_TO_VOIDPTR(val) JCAST_16_TO_VOIDPTR((val))
#define JCAST_S32_TO_VOIDPTR(val) JCAST_32_TO_VOIDPTR((val))
#define JCAST_U32_TO_VOIDPTR(val) JCAST_32_TO_VOIDPTR((val))
#define JCAST_FLOAT_TO_VOIDPTR(val) JCAST_32_TO_VOIDPTR((val))
#define JCAST_S64_TO_VOIDPTR(val) JCAST_64_TO_VOIDPTR((val))
#define JCAST_U64_TO_VOIDPTR(val) JCAST_64_TO_VOIDPTR((val))
#define JCAST_DOUBLE_TO_VOIDPTR(val) JCAST_64_TO_VOIDPTR((val))

#define JCAST_VOIDPTR_TO_TYPE(p, type) (*((type *)&(p)))
#define JCAST_VOIDPTR_TO_S8(p) JCAST_VOIDPTR_TO_TYPE(p, s8)
#define JCAST_VOIDPTR_TO_U8(p) JCAST_VOIDPTR_TO_TYPE(p, u8)
#define JCAST_VOIDPTR_TO_S16(p) JCAST_VOIDPTR_TO_TYPE(p, s16)
#define JCAST_VOIDPTR_TO_U16(p) JCAST_VOIDPTR_TO_TYPE(p, u16)
#define JCAST_VOIDPTR_TO_S32(p) JCAST_VOIDPTR_TO_TYPE(p, s32)
#define JCAST_VOIDPTR_TO_U32(p) JCAST_VOIDPTR_TO_TYPE(p, u32)
#define JCAST_VOIDPTR_TO_S64(p) JCAST_VOIDPTR_TO_TYPE(p, s64)
#define JCAST_VOIDPTR_TO_U64(p) JCAST_VOIDPTR_TO_TYPE(p, u64)
#define JCAST_VOIDPTR_TO_FLOAT(p) JCAST_VOIDPTR_TO_TYPE(p, float)
#define JCAST_VOIDPTR_TO_DOUBLE(p) JCAST_VOIDPTR_TO_TYPE(p, double)

#endif // _JTYPES_INCLUDED
