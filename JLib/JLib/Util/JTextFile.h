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

/** Class to manipulate text files.
 * @file    JTextFile.h.
 * @author  Juan Carlos Seijo Pérez
 * @date    15/06/2003
 * @version 0.0.1 - 15/06/2003 - First version.
 */

#ifndef _JTEXTFILE_INCLUDED
#define _JTEXTFILE_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JFile.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>

/** Max line length.
 */
#define MAX_LINE_LENGTH 1024

/** This class allows to manipulate text files. Contains methods to load, save, write formatted strings,
 * and search words among others.
 */
class JTextFile : public JFile
{
protected:
  s8 *ptr;                              /**< Seek pointer */
  static s8 line;                       /**< End of line character */

public:

  /** Creates a new file with the given name.
   * @param  _name Name of the file.
   */
  JTextFile(const s8 *_name = 0);

  /** Frees allocated resources and closes the file.
   */
  ~JTextFile();

  /** Seeks to the first character of the next occurrence of the given string.
   * @param  str String to look for.
   * @param  jump Indicates whether it must jump to the beginning of the file if the end of the file is reached.
   * @return <b>true</b> if the word was found, <b>false</b> if not.
   */
  bool FindNext(const s8 *str, bool jump = false);

  /** Reads the file as in the base class but assigns the seek pointer also.
   * @param  readSize Size to read or zeor to read the whole file.
   * @return Number of bytes read or 0 (zero) if an error occurred. 
   */
  virtual u32 Read(u32 readSize = 0);

  /** Loads the whole file as in the base class but assigns the seek pointer.
   * @param  filename Name of the file to load.
   * @param  mode Open mode as in fopen() (like {w|r|a}[+][{b|t}]).
   * by default is "rb" (read, binary).
   * @return <b>true</b> if succeeded, <b>false</b> otherwise.
   */
  virtual bool Load(const char *filename, const char *mode = "rb");

	/** Advances the seek pointer to the next word.
   * @return <b>true</b> if another word existed, <b>false</b> if not.
   */
  bool SkipNextWord();

  /** Reads the next word and moves the pointer.
   * @param  str Buffer where to put the read word.
   * @return <b>true</b> if another word existed, <b>false</b> if not.
   */
  bool ReadWord(s8 *str);

  /** Reads the next double-quoted word and moves the pointer.
   * @param  str Buffer where to put the read double-quoted word.
   * @return <b>true</b> if another word existed, <b>false</b> if not.
   */
  bool ReadQuotedWord(s8 *str);

  /** Reads the next decimal number. Skips all characters that are not numbers (or the decimal dot or a sign).
   * @param  f Variable to store the result.
   * @return <b>true</b> if there was a number, <b>false</b> if not.
   */
  bool ReadFloat(float *f);

  /** Reads the next integer. Skips all characters that are not numbers (or signs).
   * @param  i Variable to store the result.
   * @return <b>true</b> if there was a number, <b>false</b> if not.
   */
  bool ReadInteger(s32 *i);

  /** Advances the pointer to the next line.
   * @return <b>true</b> if there was next line, <b>false</b> if not.
   */
  bool NextLine();

  /** Moves the pointer to the start of the document.
   */
  void StartOfDocument();

  /** Reads the line under the actual position.
   * @param  str Variable to store the result.
   * @return <b>true</b> if there was next line, <b>false</b> if not.
   */
  bool ReadLine(s8 *str);

  /** Returns the pointer to the current position.
   * @return Pointer to the current position.
   */
  s8 * GetPos();

  /** Sets the current position.
   * @param _ptr New current position.
   * @return <b>true</b> if the position existed, <b>false</b> if not.
   */
  bool SetPos(s8 *_ptr);

  /** Writes in the file without line-feed.
   * @param  str String to write.
   * @return Number of characters written.
   */
  u32 Print(const s8 *str);

  /** Writes in the file with line-feed.
   * @param  str String to write.
   * @return Number of characters written.
   */
  u32 PrintLine(const s8 *str);

  /** Writes in the file with format and without line-feed.
   * @param  fmt Format string, printf-like.
   * @return Number of characters written.
   */
  u32 Printf(const s8 *fmt, ... );

  /** Counts the number of occurrences of the given string among the file.
   * @param  str String to search.
   * @param  init Start position.
   * @param  end End position.
   * @return Number of coincidences.
   */
  u32 CountString(const s8 *str, s8* init = 0, s8* end = 0);

	/** Reads like scanf.
	 * @param  format Format string.
	 * @param  format Format parameters.
	 * @return Number of parameters read successfuly.
	 */
	s32 Scanf(const char *format, ...);
};

#endif  // _JTEXTFILE_INCLUDED
