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

/** SDL_RWops wrapper.
 * @file    JRW.h
 * @author  Juan Carlos Seijo Pérez
 * @date    23/Mar/2005
 * @version 0.0.1 - 23/03/2005 - First version.
 */

#ifndef _JRW_INCLUDED
#define _JRW_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JObject.h>
#include <zlib.h>
#include <SDL.h>
#include <SDL_endian.h>

/** Encapsulates a SDL_RWops object. It's a simple wrapper.
 */
class JRW : public JObject
{
 public:
	SDL_RWops *rwops;                     /**< Pointer to the SDL_RWops object. */
	
	/** Creates an empty object. One of the Create methods must be called in order to use it.
	 */
	JRW() : rwops(0)
	{}

	/** Creates (and destroys any existing) SDL_RWops structure to access a file.
	 * @param  file Name of the file.
	 * @param  mode Open mode (as fopen() of stdio.h)
	 * @return <b>true</b> if it succeeds, <b>false</b> if not.
	 */
	bool Create(const char *file, const char *mode)
	{
    Destroy();
    
		return (0 != (rwops = SDL_RWFromFile(file, mode)));
	}

	/** Creates (and destroys any existing) SDL_RWops structure to access a file.
	 * @param  fp File pointer.
	 * @param  autoclose Close flag when destroyed.
	 * @return <b>true</b> if it succeeds, <b>false</b> if not.
	 */
	bool Create(FILE *fp, int autoclose = 0)
	{
    Destroy();
		
		return (0 != (rwops = SDL_RWFromFP(fp, autoclose)));
	}

	/** Creates (and destroys any existing) SDL_RWops structure to access a memory block.
	 * @param  mem Pointer to the memory block.
	 * @param  size Size in bytes of the block.
	 * @return <b>true</b> if it succeeds, <b>false</b> if not.
	 */
	bool Create(void *mem, int size)
	{
    Destroy();
		
		return (0 != (rwops = SDL_RWFromMem(mem, size)));
	}

	/** Creates (and destroys any existing) SDL_RWops structure to access a memory block.
	 * @param  mem Pointer to the memory block.
	 * @param  size Size in bytes of the block.
	 * @return <b>true</b> if it succeeds, <b>false</b> if not.
	 */
	bool Create(const void *mem, int size)
	{
    Destroy();

		return (0 != (rwops = SDL_RWFromConstMem(mem, size)));
	}

	/** Destroys the object and frees the allocated resources.
	 */
	void Destroy()
	{
		if (rwops)
		{
      SDL_RWclose(rwops);
			rwops = 0;
		}
	}

	/** Destroys the object.
	 */
	virtual ~JRW()
	{ 
		Destroy();
	}

	/** Seeks to the given offset.
	 * @param  offset Bytes to seek.
	 * @param  whence Position from where to start counting (SEEK_SET, SEEK_CUR, SEEK_END)
	 * @return Reached offset (note that could be less than the requested one).
	 */
	s32 Seek(s32 offset, s32 whence) {return SDL_RWseek(rwops, offset, whence);}

	/** Reurns the given position.
	 * @return Position (offset) from the beginning of the object.
	 */
	s32 Tell() {return SDL_RWtell(rwops);}

	/** Reads data.
	 * @param  ptr Read destination.
	 * @param  size Number of bytes to read.
	 * @param  maxnum Number of times to read size bytes (by default 1)
	 * @return Number of read objects or -1 if failed.
	 */
	s32 Read(void *ptr, int size, int maxnum = 1) {return SDL_RWread(rwops, ptr, size, maxnum);}

  /** Reads from the source to the given pointer, uncompressing the data. The first 4 bytes read are
	 * the uncompressed size. The next 4 bytes are the compressed size to read. Then comes the compressed data.
   * @param  buff Buffer to fill with the read data uncompressed.
   * @return Uncompressed size of the data. 
   */
	u32 ZRead(void **buff);

  /** Reads a bool data. The bool is stored as a single byte.
   * @param  buff Variable with the result.
   * @return Number of bytes read or 0 (zero) if an error occured. 
   */
	u32 ReadBool(bool *buff) {u8 b; if (0 < SDL_RWread(rwops, &b, 1, 1)) {*buff = (b != 0) ? true:false; return 1;} return 0;}

  /** Reads a bool data. The bool is stored as a single byte.
   * @param  buff Variable with the result.
   * @return Number of bytes read or 0 (zero) if an error occured. 
   */
	u32 Read8(u8 *buff) {if (0 < SDL_RWread(rwops, buff, 1, 1)) return 1; return 0;}

  /** Reads a bool data. The bool is stored as a single byte.
   * @param  buff Variable with the result.
   * @return Number of bytes read or 0 (zero) if an error occured. 
   */
	u32 Read8(s8 *buff) {if (0 < SDL_RWread(rwops, buff, 1, 1)) return 1; return 0;}

  /** Reads a 16-bit unsigned data in little-endian format.
   * @param  buff Variable with the result in the machine weight.
   * @return Number of bytes read or 0 (zero) if an error occured. 
   */
	u32 ReadLE16(u16 *buff) {if (0 < SDL_RWread(rwops, buff, 2, 1)) {*buff = SDL_SwapLE16(*buff); return 2;} return 0;}

  /** Reads a 16-bit signed data in little-endian format.
   * @param  buff Variable with the result in the machine weight.
   * @return Number of bytes read or 0 (zero) if an error occured. 
   */
	u32 ReadLE16(s16 *buff) {if (0 < SDL_RWread(rwops, buff, 2, 1)) {*buff = SDL_SwapLE16(*buff); return 2;} return 0;}

  /** Reads a 16-bit unsigned data in big-endian format.
   * @param  buff Variable with the result in the machine weight.
   * @return Number of bytes read or 0 (zero) if an error occured. 
   */
  u32 ReadBE16(u16 *buff) {if (0 < SDL_RWread(rwops, buff, 2, 1)) {*buff = SDL_SwapBE16(*buff); return 2;} return 0;}

  /** Reads a 16-bit signed data in big-endian format.
   * @param  buff Variable with the result in the machine weight.
   * @return Number of bytes read or 0 (zero) if an error occured. 
   */
  u32 ReadBE16(s16 *buff) {if (0 < SDL_RWread(rwops, buff, 2, 1)) {*buff = SDL_SwapBE16(*buff); return 2;} return 0;}

  /** Reads a 32-bit unsigned data in little-endian format.
   * @param  buff Variable with the result in the machine weight.
   * @return Number of bytes read or 0 (zero) if an error occured. 
   */
  u32 ReadLE32(u32 *buff) {if (0 < SDL_RWread(rwops, buff, 4, 1)) {*buff = SDL_SwapLE32(*buff); return 4;} return 0;}

  /** Reads a 32-bit signed data in little- endian format.
   * @param  buff Variable with the result in the machine weight.
   * @return Number of bytes read or 0 (zero) if an error occured. 
   */
  u32 ReadLE32(s32 *buff) {if (0 < SDL_RWread(rwops, buff, 4, 1)) {*buff = SDL_SwapLE32(*buff); return 4;} return 0;}

  /** Reads a 32-bit unsigned data in big-endian format.
   * @param  buff Variable with the result in the machine weight.
   * @return Number of bytes read or 0 (zero) if an error occured. 
   */
  u32 ReadBE32(u32 *buff) {if (0 < SDL_RWread(rwops, buff, 4, 1)) {*buff = SDL_SwapBE32(*buff); return 4;} return 0;}

  /** Reads a 32-bit signed data in big-endian format.
   * @param  buff Variable with the result in the machine weight.
   * @return Number of bytes read or 0 (zero) if an error occured. 
   */
  u32 ReadBE32(s32 *buff) {if (0 < SDL_RWread(rwops, buff, 4, 1)) {*buff = SDL_SwapBE32(*buff); return 4;} return 0;}

  /** Imports from a file.
   * @param  filename Name of the file to import.
   * @return Number of bytes imported or 0 (zero) if an error occured. 
   */
	u32 Import(const char *filename);

	/** Writes data to the source.
	 * @param  ptr Pointer to the data to be written.
	 * @param  size Number of bytes to write.
	 * @param  maxnum Number of times to write size bytes (1 by default)
	 * @return maxnum or -1 if failed.
	 */
	s32 Write(const void *ptr, int size, int maxnum = 1) {return SDL_RWwrite(rwops, ptr, size, maxnum);}
	
  /** Writes in the file, compressing the data. The format is: 
	 * [size uncompressed][size compressed][compressed data].
   * @param  buff Pointer to the data to be written compressed.
   * @param  size Size in bytes of the data to write.
   * @param  level Compression level, from 1 (less compression) to 9 (best compression).
   * @return Number of bytes written or 0 if an error occured.
   */
	u32 ZWrite(const void *buff, u32 size, s32 level);

  /** Writes a bool data. The bool is stored as a single byte.
   * @param  buff Variable with the data.
   * @return Number of bytes written or 0 (zero) if an error occured. 
   */
	u32 WriteBool(bool *buff) {u8 b = (*buff)? 1 : 0; return SDL_RWwrite(rwops, &b, 1, 1);}

  /** Writes a byte.
   * @param  buff Variable with the data.
   * @return Number of bytes written or 0 (zero) if an error occured. 
   */
	u32 Write8(u8 *buff) {return SDL_RWwrite(rwops, buff, 1, 1);}

  /** Writes a byte.
   * @param  buff Variable with the data.
   * @return Number of bytes written or 0 (zero) if an error occured. 
   */
	u32 Write8(s8 *buff) {return SDL_RWwrite(rwops, buff, 1, 1);}

  /** Writes a 16-bit unsigned data in little-endian format.
   * @param  buff Variable with the data in the machine weight.
   * @return Number of bytes written or 0 (zero) if an error occured. 
   */
	u32 WriteLE16(u16 *buff) {u16 v = SDL_SwapLE16(*buff); return SDL_RWwrite(rwops, &v, 2, 1);}

  /** Writes a 16-bit signed data in little-endian format.
   * @param  buff Variable with the data in the machine weight.
   * @return Number of bytes written or 0 (zero) if an error occured. 
   */
	u32 WriteLE16(s16 *buff) {s16 v = SDL_SwapLE16(*buff); return SDL_RWwrite(rwops, &v, 2, 1);}

  /** Writes a 16-bit unsigned data in big-endian format.
   * @param  buff Variable with the data in the machine weight.
   * @return Number of bytes written or 0 (zero) if an error occured. 
   */
  u32 WriteBE16(u16 *buff) {u16 v = SDL_SwapBE16(*buff); return SDL_RWwrite(rwops, &v, 2, 1);}

  /** Writes a 16-bit signed data in big-endian format.
   * @param  buff Variable with the data in the machine weight.
   * @return Number of bytes written or 0 (zero) if an error occured. 
   */
  u32 WriteBE16(s16 *buff) {s16 v = SDL_SwapBE16(*buff); return SDL_RWwrite(rwops, &v, 2, 1);}

  /** Writes a 32-bit unsigned data in little-endian format.
   * @param  buff Variable with the data in the machine weight.
   * @return Number of bytes written or 0 (zero) if an error occured. 
   */
  u32 WriteLE32(u32 *buff) {u32 v = SDL_SwapLE32(*buff); return SDL_RWwrite(rwops, &v, 4, 1);}

  /** Writes a 32-bit signed data in little-endian format.
   * @param  buff Variable with the data in the machine weight.
   * @return Number of bytes written or 0 (zero) if an error occured. 
   */
  u32 WriteLE32(s32 *buff) {s32 v = SDL_SwapLE32(*buff); return SDL_RWwrite(rwops, &v, 4, 1);}

  /** Writes a 32-bit unsigned data in big-endian format.
   * @param  buff Variable with the data in the machine weight.
   * @return Number of bytes written or 0 (zero) if an error occured. 
   */
  u32 WriteBE32(u32 *buff) {u32 v = SDL_SwapBE32(*buff); return SDL_RWwrite(rwops, &v, 4, 1);}

  /** Writes a 32-bit signed data in big-endian format.
   * @param  buff Variable with the data in the machine weight.
   * @return Number of bytes written or 0 (zero) if an error occured. 
   */
  u32 WriteBE32(s32 *buff) {s32 v = SDL_SwapBE32(*buff); return SDL_RWwrite(rwops, &v, 4, 1);}

  /** Export to file.
   * @param  filename Name of the file to export to.
   * @param  size Size of the data to export.
   * @return Number of bytes written, 0 (zero) if an error occured. 
   */
	u32 Export(const char *filename, u32 size);

	/** Close the file. Only for a JRW created from a file.
	 */
	s32 Close() {s32 ret = SDL_RWclose(rwops); rwops = 0; return ret;}
};

#endif // _JRW_INCLUDED
