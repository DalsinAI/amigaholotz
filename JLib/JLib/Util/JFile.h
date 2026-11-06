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

/** File base class.
 * @file    JFile.h
 * @author  Juan Carlos Seijo Pérez
 * @date    15/06/2003
 * @version 0.0.1 - 15/06/2003 - First versio.
 */

#ifndef _JFILE_INCLUDED
#define _JFILE_INCLUDED

#include <JLib/Util/JTypes.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <SDL_endian.h>
#include <zlib.h>
#include <JLib/Util/JCompatibility.h>

/*** Maximum block size by default */
#define JFILE_DEF_MAX_BLOCK_SIZE                  2097152

/** Encapsulates a file. Allows to read, write, resize, insert, compress, etc.
 */
class JFile
{
protected:
  FILE *file;                                     /**< File pointer */
  s8 *buff;                                       /**< Auxilliary read buffer */
  s8 *name;                                       /**< File name */
	bool exists;                                    /**< Existence flag */
  struct stat statInfo;                           /**< File information */
  u32 size;                                       /**< Size of the file */
  u32 buffSize;                                   /**< Size of the auxilliary buffer */
	static u32 MAX_BLOCK_SIZE;                      /**< Max size of copy block buffer */

	/** Determines if the given file is of the given type.
   * @param  name Name of the file to check.
   * @param  mode Type of file.
   * @return <b>true</b> if so, <b>false</b> if not.
   */
	static bool IsOfType(const s8 *name, mode_t mode);

public:
  
  /** Creates an empty file object.
   * @param  filename Name of the file (optional).
   */
  JFile(const s8 *filename = 0);

  /** Destroys the object and frees allocated resources.
   */
  virtual ~JFile();

  /** Gathers again the file information, (size, permissions, etc.).
   * @return 0 (zero) if success, -1 if error and -2 if a name was not given.
   */
	s32 Refresh();

  /** Opens the file. 
   * @param  filename Name of the file to open.
   * @param  mode Opening modes in fopen(): {w|r|a}[+][{b|t}].
   * by default is "r+b" (read-write, but don't create it if it doesn't exist and binario).
   */
  virtual bool Open(const s8 *filename = 0, const s8 *mode = "r+b");
  
  /** Reads to the internal buffer.
   * @param  readSize Size in bytes to read from the file. If zero, the whole file is read.
   * @return Number of bytes read or zero if an error occurred. 
   */
  virtual u32 Read(u32 readSize = 0);
  
  /** Reads from the file to the internal buffer, uncompressing the read data.
   * @return Number of bytes of the uncompressed buffer or zero if an error ocurred. 
   */
  virtual u32 ZRead();
  
  /** Reads from the file to the given buffer.
   * @param  _buff Buffer to fill with the data read.
   * @param  _size Size in bytes to read. If zero, the whole file is read.
   * @return Number of bytes read or zero if an error occurred. 
   */
  virtual u32 Read(void *_buff, u32 _size);

  /** Reads a boolean. WARNING: It is read as a single byte.
   * @param  _buff Variable to store the result.
   * @return Number of bytes read or zero if an error ocurred. 
   */
  virtual u32 ReadBool(bool *_buff);

  /** Reads a byte.
   * @param  _buff Variable to store the result.
   * @return Number of bytes read or zero if an error ocurred. 
   */
  virtual u32 Read8(u8 *_buff);

  /** Reads a byte.
   * @param  _buff Variable to store the result.
   * @return Number of bytes read or zero if an error ocurred. 
   */
  virtual u32 Read8(s8 *_buff);

  /** Reads a data in little-endian format.
   * @param  _buff Variable to store the result in the machine's endianness.
   * @return Number of bytes read or zero if an error ocurred. 
   */
  virtual u32 ReadLE16(u16 *_buff);

  /** Reads a data in little-endian format.
   * @param  _buff Variable to store the result in the machine's endianness.
   * @return Number of bytes read or zero if an error ocurred. 
   */
  virtual u32 ReadLE16(s16 *_buff);

  /** Reads a data in big-endian format.
   * @param  _buff Variable to store the result in the machine's endianness.
   * @return Number of bytes read or zero if an error ocurred. 
   */
  virtual u32 ReadBE16(u16 *_buff);

  /** Reads a data in big-endian format.
   * @param  _buff Variable to store the result in the machine's endianness.
   * @return Number of bytes read or zero if an error ocurred. 
   */
  virtual u32 ReadBE16(s16 *_buff);

  /** Reads a data in little-endian format.
   * @param  _buff Variable to store the result in the machine's endianness.
   * @return Number of bytes read or zero if an error ocurred. 
   */
  virtual u32 ReadLE32(u32 *_buff);

  /** Reads a data in little-endian format.
   * @param  _buff Variable to store the result in the machine's endianness.
   * @return Number of bytes read or zero if an error ocurred. 
   */
  virtual u32 ReadLE32(s32 *_buff);

  /** Reads a data in big-endian format.
   * @param  _buff Variable to store the result in the machine's endianness.
   * @return Number of bytes read or zero if an error ocurred. 
   */
  virtual u32 ReadBE32(u32 *_buff);

  /** Reads a data in big-endian format.
   * @param  _buff Variable to store the result in the machine's endianness.
   * @return Number of bytes read or zero if an error ocurred. 
   */
  virtual u32 ReadBE32(s32 *_buff);

  /** Reads from the file to the given buffer, uncompressing the data. The first 4 bytes read are
	 * the uncompressed size. The next 4 are the size to read compressed. Then the data comes.
   * @param  _buff Buffer to fill with the uncompressed data. The memory is reserved using operator new from inside,
   * the caller should delete it after using it.
   * @return Uncompressed size. 
   */
  virtual u32 ZRead(void * &_buff);

  /** Reads from the file to the given buffer and leaves the poiter in its original position.
   * @param  _buff Buffer to fill with the data read.
   * @param  _size Size in bytes to read from the file. If zero, reads the whole file.
   * @return Number of bytes read or zero if an error occurred. 
   */
  virtual u32 Peep(void *_buff, u32 _size);

  /** Loads the whole file. Is the same as Open() + Read(0).
   * @param  filename Name of the file to load.
   * @param  mode Opening mode as that of fopen(): {w|r|a}[+][{b|t}]. By default, "rb" is used.
   * @return <b>true</b> if loaded successfuly, <b>false</b> if not.
   */
  virtual bool Load(const s8 *filename, const s8 *mode = "rb");
  
  /** Writes in the file.
   * @param  _buff Buffer with the data to write.
   * @param  _size Size in bytes to write.
   * @return Number of bytes written or zero if an error occurred.
   */
  virtual u32 Write(const void *_buff, u32 _size);

  /** Writes a boolean. It is stored as a single byte.
   * @param  _buff Variable with the value to store.
   * @return Number of bytes written or zero if an error ocurred. 
   */
	u32 WriteBool(bool *_buff);

  /** Writes a byte.
   * @param  _buff Variable with the value to store.
   * @return Number of bytes written or zero if an error ocurred. 
   */
	u32 Write8(u8 *_buff);

  /** Writes a byte.
   * @param  _buff Variable with the value to store.
   * @return Number of bytes written or zero if an error ocurred. 
   */
	u32 Write8(s8 *_buff);

  /** Writes a data in little-endian format.
   * @param  _buff Variable with the value to store in the machine's endianness.
   * @return Number of bytes written or zero if an error ocurred. 
   */
	u32 WriteLE16(u16 *_buff);

  /** Writes a data in little-endian format.
   * @param  _buff Variable with the value to store in the machine's endianness.
   * @return Number of bytes written or zero if an error ocurred. 
   */
	u32 WriteLE16(s16 *_buff);

  /** Writes a data in big-endian format.
   * @param  _buff Variable with the value to store in the machine's endianness.
   * @return Number of bytes written or zero if an error ocurred. 
   */
	u32 WriteBE16(u16 *_buff);
	
  /** Writes a data in big-endian format.
   * @param  _buff Variable with the value to store in the machine's endianness.
   * @return Number of bytes written or zero if an error ocurred. 
   */
	u32 WriteBE16(s16 *_buff);
	
  /** Writes a data in little-endian format.
   * @param  _buff Variable with the value to store in the machine's endianness.
   * @return Number of bytes written or zero if an error ocurred. 
   */
	u32 WriteLE32(u32 *_buff);
 
  /** Writes a data in little-endian format.
   * @param  _buff Variable with the value to store in the machine's endianness.
   * @return Number of bytes written or zero if an error ocurred. 
   */
	u32 WriteLE32(s32 *_buff);
 
  /** Writes a data in big-endian format.
   * @param  _buff Variable with the value to store in the machine's endianness.
   * @return Number of bytes written or zero if an error ocurred. 
   */
	u32 WriteBE32(u32 *_buff);

  /** Writes a data in big-endian format.
   * @param  _buff Variable with the value to store in the machine's endianness.
   * @return Number of bytes written or zero if an error ocurred. 
   */
	u32 WriteBE32(s32 *_buff);

  /** Writes compressed to the file. The format is
	 * [size uncompressed][size compressed][compressed data].
   * @param  _buff Buffer with the data to write.
   * @param  _size Size of the data to write.
   * @param  level Compression level, 1 (lowest) to 9 (greatest).
   * @return Number of bytes written (including the 8 of the header) or zero if an error occurred.
   */
  virtual u32 ZWrite(const void *_buff, u32 _size, s32 level = 9);

  /** Returns the internal read buffer.
   * @return Internal buffer. NEVER must be deleted from outside the class.
   */
  virtual s8 *Buffer() {return buff;}
  
  /** Returns the size of the internal buffer. Its the size of the last Read operation to the internal buffer.
   * @return Size of the internal buffer.
   */
  virtual u32 BufferSize() {return buffSize;}
  
  /** Returns the size of the file. If the file could have changed its size since its creation, 
   * is necessary a call to Refresh.
   * @return Size in bytes of the file.
   */
  inline u32 FileSize() {return statInfo.st_size;}
  
  /** Frees the internal buffer.
   */
  virtual void FreeBuffer();

  /** Places the file pointer at the beginning of the file.
   */
  virtual void Rewind();

  /** Closes the file.
   * @return <b>true</b> if memory was freed, <b>false</b> if it was not necessary.
   */
  virtual bool Close();

  /** Returns the internal file pointer.
   * @return Internal file pointer.
   */
  virtual const FILE * File() {return file;}

  /** Returns the name of the file.
   * @return Name of the file.
   */
  virtual const s8 * Name() {return name;}

  /** Returns the current position in bytes from the beginning of the file.
   * @return Current position in bytes from the beginning of the file.
   */
  virtual s32 Pos() {return ftell(file);}

  /** Sets the current position of the file pointer.
   * @param  pos New position, in bytes.
   * @return <b>true</b> if the position waswithin the file, <b>false</b> if not.
   */
  virtual bool Pos(s32 pos) {return 0 == fseek(file, pos, SEEK_SET);}

  /** Seeks to the given position.
   * @param  pos Position to seek.
   * @param  whence Source of the seeking (SEEK_SET, SEEK_CUR, SEEK_END).
   * @return <b>true</b> if reached, <b>false</b> if not.
   */
  virtual bool Seek(s32 pos, u32 whence) {return 0 == fseek(file, pos, whence);}

	/** Determines if this file exists. If the file has been created from outside the class, 
   * Refresh() must be called.
   * @return <b>true</b> if it exists, <b>false</b> if not.
   */
	virtual bool Exists() {return exists;}

	/** Returns the size of this file in bytes.
   * @return Size in bytes of the file.
   */
	virtual u32 Size() {return statInfo.st_size;}

	/** Determines if this file is a regular file or not (link, directory, etc.)
   * @return <b>true</b> if so, <b>false</b> if not.
   */
	bool IsFile() {return 0 != (statInfo.st_mode & S_IFREG);}

	/** Determines if it's a directory.
   * @return <b>true</b> if so, <b>false</b> if not.
   */
	bool IsDir() {return 0 != (statInfo.st_mode & S_IFDIR);}

	/** Determines if it's a link to another file.
   * @return <b>true</b> if so, <b>false</b> if not.
   */
	bool IsLink() {return 0 != (statInfo.st_mode & S_IFLNK);}

	/** Determines if it's a character device.
   * @return <b>true</b> if so, <b>false</b> if not.
   */
	bool IsCharDev() {return 0 != (statInfo.st_mode & S_IFCHR);}

	/** Determines if it's a block device.
   * @return <b>true</b> if so, <b>false</b> if not.
   */
	bool IsBlockDev() {return 0 != (statInfo.st_mode & S_IFBLK);}

	/** Determines if a file with the given name exists.
   * @param  name Name of the file to test.
   * @return <b>true</b> if it exists, <b>false</b> if not.
   */
	static bool Exists(const s8 *name);

	/** Determines the size of the given file.
   * @param  name Name of the file.
   * @return Size in bytes of the file. If it does not exist, it returns 0 (zero).
   */
	static u32 Size(const s8 *name);

	/** Determines if it's a regular file.
   * @param  name Name of the file.
   * @return <b>true</b> if so, <b>false</b> if not.
   */
	static bool IsFile(const s8 *name);

	/** Determines if it's a directory.
   * @param  name Name of the file.
   * @return <b>true</b> if so, <b>false</b> if not.
   */
	static bool IsDir(const s8 *name);

	/** Determines if it's a link.
   * @param  name Name of the file.
   * @return <b>true</b> if so, <b>false</b> if not.
   */
	static bool IsLink(const s8 *name);

	/** Determines if it's a character device.
   * @param  name Name of the file.
   * @return <b>true</b> if so, <b>false</b> if not.
   */
	static bool IsCharDev(const s8 *name);

	/** Determines if it's a block device.
   * @param  name Name of the file.
   * @return <b>true</b> if so, <b>false</b> if not.
   */
	static bool IsBlockDev(const s8 *name);

	/** Ensures all the data are written in the physical media.
   * @return <b>true</b> if succeeded, <b>false</b> otherwise.
   */
	bool Flush() {return 0 == fflush(file);}

	/** Resizes this file, moving in disk the necessary bytes from the insert position
   * After it, the file is ready to write in the new region.
   * The opening mode must be binary and read and write (not only "wb", but "r+b").
	 * @param  byte Position of the first byte left blank.
	 * @param  numBytes Number of blank bytes to insert.
   * @return <b>true</b> if it succeeds, <b>false</b> if not.
   */
	bool ResizeAt(u32 byte, s32 numBytes);

	/** Resizes this file, moving in disk the necessary bytes from the beginning of the file.
   * After it, the file is ready to write in the new region.
   * The opening mode must be binary and read and write (not only "wb", but "r+b").
	 * @param  numBytes Number of blank bytes to insert.
   * @return <b>true</b> if it succeeds, <b>false</b> if not.
   */
	bool ResizeFront(u32 numBytes) {return ResizeAt(0, numBytes);}

	/** Changes the size of the file at the end of it (thus, no need to move any bytes).
   * The opening mode must be binary and read and write (not only "wb", but "r+b").
	 * @param  numBytes Number of blank bytes to insert.
   * @return <b>true</b> if it succeeds, <b>false</b> if not.
   */
	bool ResizeBack(u32 numBytes);

	/** Sets the maximum transfer block size for a resize operation like ResizeAt() or ResizeFront().
	 * Its default value is JFILE_DEF_MAX_BLOCK_SIZE (2MB). Ideally this value should be that of the maximum DMA
   * transfer block in order to obtain the best results (above this value, there are no significant benefits
   * and furthermore the memory needed must be previously allocated by the class so it can be a penalty).
	 * @param  max New New block size (at least 1).
   */
	static void MaxBlockSize(u32 max) {MAX_BLOCK_SIZE = (max > 0) ? max : 1;}

	/** Returns the maximum transfer block size for resize operations like ResizeAt() and ResizeFront().
	 * By default this value is JFILE_DEF_MAX_BLOCK_SIZE (2MB).
	 * @return Maximum transfer block size.
   */
	static u32 MaxBlockSize() {return MAX_BLOCK_SIZE;}
};

#endif  // _JFILE_INCLUDED
