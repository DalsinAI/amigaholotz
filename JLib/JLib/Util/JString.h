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

/** Text string class.
 * @file    JString.h
 * @author  Juan Carlos Seijo Pérez
 * @date    19/04/2003
 * @version 0.0.1 - 19/04/2003 - First version.
 */

#ifndef _JSTRING_INCLUDED
#define _JSTRING_INCLUDED

#include <JLib/Util/JTypes.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <JLib/Util/JLoadSave.h>

class JString : public JLoadSave
{
  friend bool operator==(const char *s1, const JString& s2);
  friend bool operator==(const JString& s1, const char *s2);
  friend bool operator!=(const char *s1, const JString& s2);
  friend bool operator!=(const JString& s1, const char *s2);
  friend bool operator>(const char *s1, const JString& s2);
  friend bool operator<(const char *s1, const JString& s2);
  friend bool operator>=(const char *s1, const JString& s2);
  friend bool operator<=(const char *s1, const JString& s2);
  //friend const JString operator+(const char *s1, const JString &s2);
  friend const JString operator+(const JString &s1,  const char *s2);
	friend const JString operator+(const JString &s1,  const JString &s2);

protected:
  char *data;           // Caracteres
  u32 length;  // Longitud

public:

  /** Creates an empty string.
   */
  inline JString();

  /** Creates a string that is a copy of the given string.
   * @param  s String to be copied.
   */
  inline JString(const char *s);

  /** Creates a string from a substring of the given string.
   * @param  s String.
   * @param  start Start of the substring.
   * @param  end Of the substring, -1 for the string length.
   */
  inline JString(const JString& s, u32 start, u32 end = (u32)-1);

  /** Creates a string from a substring of the given string.
   * @param  s String.
   * @param  start Start of the substring.
   * @param  end Of the substring, -1 for the string length.
   */
  inline JString(const char *s, u32 start, u32 end = (u32)-1);

  /** Creates a string that is a copy of the given string.
   * @param  s String to be copied.
   */
  inline JString(const JString& s);

  /** Destroys this string and frees the allocated resources.
   */
  inline virtual ~JString();

  /** Clears the string.
   */
  inline void Clear();

  /** Compares this string to another one.
   * @param  s Another string.
   * @return <b>true</b> if they are equal, <b>false</b> otherwise.
   */
  inline bool Equals(const JString& s) const;

  /** Compares this string to another one.
   * @param  s Another string.
   * @return <b>true</b> if they are equal, <b>false</b> otherwise.
   */
  inline bool operator==(const JString& s) const;

  /** Compares this string to another one.
   * @param  s Another string.
   * @return 0 i they are equal, >0 if this string is ASCII-greater than the given, <0 if its ASCII-lower.
   */
  inline s32 CompareTo(const JString& s) const;

  /** Compares this string to another one.
   * @param  s Another string.
   * @return 0 i they are equal, >0 if this string is ASCII-greater than the given, <0 if its ASCII-lower.
   */
  inline s32 CompareTo(const char *s) const;

  /** Test if this string is ASCII-greater than another.
   * @param  s Another string.
   * @return <b>true</b> if so, <b>false</b> otherwise.
   */
  inline bool operator>(const JString& s) const;

  /** Test if this string is ASCII-lower than another.
   * @param  s Another string.
   * @return <b>true</b> if so, <b>false</b> otherwise.
   */
  inline bool operator<(const JString& s) const;

  /** Test if this string is ASCII-greater or equal than another.
   * @param  s Another string.
   * @return <b>true</b> if so, <b>false</b> otherwise.
   */
  inline bool operator>=(const JString& s) const;

  /** Test if this string is ASCII-lower or equal than another.
   * @param  s Another string.
   * @return <b>true</b> if so, <b>false</b> otherwise.
   */
  inline bool operator<=(const JString& s) const;

  /** Compares this string to another one.
   * @param  s Another string.
   * @return <b>true</b> if they are different, <b>false</b> otherwise.
   */
  inline bool operator!=(const JString& s) const;

  /** Returns the char at the given pos.
   * @param  pos Index of the character to be retrieved.
   */
  inline char& operator[](s32 pos);

  /** Assigns a copy of the given string to this one.
   * @param  s Another string.
   */
  inline void operator=(const char *s);

  /** Assigns a copy of the given string to this one.
   * @param  s Another string.
   */
  inline void operator=(const JString &s);

  /** Returns the internal character array.
   * @return internal character array.
   */
  inline const char* Str() const;
  

  /** Appends the given string to this one.
   * @param  s Another string.
   */
  inline void Append(const JString& s);

  /** Appends the given string to this one.
   * @param  s Another string.
   */
  inline void Append(const char * s);

  /** Appends the given string to this one.
   * @param  s Another string.
   */
  inline void operator+=(const JString& s);

  /** Appends the given string to this one.
   * @param  s Another string.
   */
  inline void operator+=(const char *s);

  /** Appends the given char to this string.
   * @param  c Character to append.
   */
  inline void Append(char c);

  /** Appends the given char to this string.
   * @param  c Character to append.
   */
  inline void operator+=(char c);

  /** Creates a string from a format string (as printf uses).
   * @param  fmt Format string.
   * @param  ... Format parameters.
   * @return <b>true</b> if it could be formatted, <b>false</b> otherwise.
   */
  inline bool Format(const char *fmt, ...);

  /** Converts this string to uppercase.
   */
  inline void Uppercase();

  /** Converts this string to lowercase.
   */
  inline void Lowercase();

  /** Searchs for the given character from the given position within the string.
   * @param  c Character to be found.
   * @param  from Index within the string where to start the search (inclusive).
   * @return index of the character searched or -1 if it wasn't found.
   */
  inline s32 Find(char c, u32 from = 0);

  /** Searchs for the last index of the given character from the given position within the string.
   * @param  c Character to be found.
   * @param  from Index within the string where to start the search (inclusive).
   * @return index of the character searched or -1 if it wasn't found.
   */
  inline s32 FindLast(char c, u32 from = 0);

  /** Searchs for the index of the given string from the given position within this string.
   * @param  str String to be found.
   * @param  from Index within the string where to start the search (inclusive).
   * @return index of the character searched or -1 if it wasn't found.
   */
  inline s32 Find(const char *str, u32 from = 0);

  /** Searchs for the last index of the given string from the given position within this string.
   * @param  str String to be found.
   * @param  from Index within the string where to start the search (inclusive).
   * @return index of the character searched or -1 if it wasn't found.
   */
  inline s32 FindLast(const char *str, u32 from = 0);

  /** Replaces pat with rep from the given index, max number of times within this string.
   * @param  pat Character to look for.
   * @param  rep Character to replace pat with.
   * @param  from Start index within the string where to start the search (inclusive).
   * @param  max Maximum number of replacements to be made.
   * @return Number of replacements doen.
   */
  inline s32 Replace(const char pat, const char rep, u32 from = 0, u32 max = 0);

  /** Replaces pat with rep from the given index, max number of times within this string.
   * @param  pat String pattern to look for.
   * @param  rep String replacement to replace pat with.
   * @param  from Start index within the string where to start the search (inclusive).
   * @param  max Maximum number of replacements to be made.
   * @return Number of replacements doen.
   */
  inline s32 Replace(const char *pat, const char *rep, u32 from = 0, u32 max = 0);

  /** Returns a string that is a substring of this string.
   * @param  start Substring start position.
   * @param  end Substring end position.
   * @return Substring.
   */
  inline const JString Substring(u32 start, u32 end = (u32)-1);


  /** Cast to const char * type.
   * @return const char * string.
   */
  inline operator const char *() const {return data;}  // Cast a cadena


  /** Returns the length of this string.
   * @return Length of this string.
   */
  inline u32 Length() const {return length;} // Devuelve la longitud

  // Lee la cadena de un fichero (longitud + cadena)

  /** Reads a string from a JRW object (length in four bytes + characters)
   * @param  f JRW object.
   * @return 0 if succeeded, 1 if I/O error, 2 if data integrity error.
   */
  inline u32 Load(JRW &f);

  // Escribe la cadena a un fichero (longitud + cadena)

  /** Writes a string to a JRW object (length in four bytes + characters)
   * @param  f JRW object.
   * @return 0 if succeeded, 1 if I/O error, 2 if data integrity error.
   */
  inline u32 Save(JRW &f);
};

/** Test the equality of two strings.
 * @param  s1 First string.
 * @param  s1 Second string.
 * @return <b>true</b> if they are equal, <b>false</b> otherwise.
 */
inline bool operator==(const char *s1, const JString& s2)
{
  return (strcmp(s1, s2.data) == 0);
}

/** Test the equality of two strings.
 * @param  s1 First string.
 * @param  s1 Second string.
 * @return <b>true</b> if they are equal, <b>false</b> otherwise.
 */
inline bool operator==(const JString& s1, const char *s2)
{
  return (strcmp(s1.data, s2) == 0);
}

/** Test if the given strings are different.
 * @param  s1 First string.
 * @param  s1 Second string.
 * @return <b>true</b> if so, <b>false</b> otherwise.
 */
inline bool operator!=(const char *s1, const JString& s2)
{
  return (strcmp(s1, s2.data) != 0);
}

/** Test if the given strings are different.
 * @param  s1 First string.
 * @param  s1 Second string.
 * @return <b>true</b> if so, <b>false</b> otherwise.
 */
inline bool operator!=(const JString& s1, const char *s2)
{
  return (strcmp(s1.data, s2) != 0);
}

/** Test if the first string is ASCII-greater than the second.
 * @param  s1 First string.
 * @param  s1 Second string.
 * @return <b>true</b> if so, <b>false</b> otherwise.
 */
inline bool operator>(const char *s1, const JString& s2)
{
  return (strcmp(s1, s2.data) > 0);
}

/** Test if the first string is ASCII-lower than the second.
 * @param  s1 First string.
 * @param  s1 Second string.
 * @return <b>true</b> if so, <b>false</b> otherwise.
 */
inline bool operator<(const char *s1, const JString& s2) 
{
  return (strcmp(s1, s2.data) < 0);
}

/** Test if the first string is ASCII-greater or equal than the second.
 * @param  s1 First string.
 * @param  s1 Second string.
 * @return <b>true</b> if so, <b>false</b> otherwise.
 */
inline bool operator>=(const char *s1, const JString& s2)
{
  return (strcmp(s1, s2.data) >= 0);
}

/** Test if the first string is ASCII-lower or equal than the second.
 * @param  s1 First string.
 * @param  s1 Second string.
 * @return <b>true</b> if so, <b>false</b> otherwise.
 */
inline bool operator<=(const char *s1, const JString& s2)
{
  return (strcmp(s1, s2.data) <= 0);
}

// Constructor
JString::JString() : length(0)
{
  data = new char[1];
  data [0] = '\0';
}

// Constructor
JString::JString(const char *s)
{
  if (s)
  {
    length = (u32)strlen(s);
    data = new char[length + 1];
    strcpy(data, s);
  }
  else
  {
    data = new char[1];
    data [0] = '\0';
    length = 0;
  }
}

// Constructor de subcadena
inline JString::JString(const JString& s, u32 start, u32 end)
{
  if (end == (u32)-1)
    end = s.Length();

  if (start >= 0 && start < s.Length() && end > start && end <= s.Length())
  {
    length = end - start;
    data = new char[length + 1];
    strncpy(data, s.data + start, length);
    data[length] = '\0';
  }
  else
  {
    data = new char[1];
    data [0] = '\0';
    length = 0;
  }
}

// Constructor de subcadena
inline JString::JString(const char *s, u32 start, u32 end)
{
  if (s)
  {
    u32 len = (u32)strlen(s);
    if (end == (u32)-1)
      end = len;

    if (start >= 0 && start < len && end > start && end <= len)
    {
      length = end - start;
      data = new char[length + 1];
      strncpy(data, s + start, length);
      data[length] = '\0';
    }
    else
    {
      data = new char[1];
      data [0] = '\0';
      length = 0;
    }
  }
  else
  {
    data = new char[1];
    data [0] = '\0';
    length = 0;
  }
}

// Constructor copia
JString::JString(const JString &s)
{
  length = s.length;
  data = new char[length + 1];
  strcpy(data, s.data);
}

// Destructor
JString::~JString()
{
  delete[] data;
}

// Borra la cadena
inline void JString::Clear()
{
  delete[] data;
  data = new char[1];
  
  // La cadena no tiene estado 'nulo', como mucho estará vacía
  data [0] = '\0';
  length = 0;
}

// ¿Son iguales?
inline bool JString::Equals(const JString& s) const
{
  return (strcmp(data, s.data) == 0);
}

// ¿Son iguales?
inline bool JString::operator==(const JString& s) const
{
  return (strcmp(data, s.data) == 0);
}

// Comparación de cadenas
inline s32 JString::CompareTo(const JString &s) const
{
  return (strcmp(data, s.data));
}

// Comparación de cadenas
inline s32 JString::CompareTo(const char *s) const
{
  return (strcmp(data, s));
}

// ¿Es la primera mayor?
inline bool JString::operator>(const JString& s) const
{
  return (strcmp(data, s.data) > 0);
}

// ¿Es la primera menor?
inline bool JString::operator<(const JString& s) const
{
  return (strcmp(data, s.data) < 0);
}

// ¿Es la primera mayor o igual?
inline bool JString::operator>=(const JString& s) const
{
  return (strcmp(data, s.data) >= 0);
}

// ¿Es la primera menor o igual?
inline bool JString::operator<=(const JString& s) const
{
  return (strcmp(data, s.data) <= 0);
}

// ¿Son diferentes?
inline bool JString::operator!=(const JString& s) const
{
  return (strcmp(data, s.data) != 0);
}

// Devuelve el caracter en la posición dada
inline char& JString::operator[](s32 pos)
{
  return data[pos];
}

// Asignación
inline void JString::operator=(const char *s)
{
  if (s)
  {
    u32 len;
    
    len = (u32)strlen(s);
    if (len > length)
    {
      delete[] data;
      data = new char[len + 1];
    }

    length = len;
    strcpy(data, s);
  }
  else
  {
    delete[] data;
    data = new char[1];
    data[0] = '\0';
    length = 0;
  }
}

// Asignación
inline void JString::operator=(const JString &s)
{
  if (s.Length() > length)
  {
    delete[] data;
    data = new char[s.length + 1];
  }
  length = s.length;
  strcpy(data, s.data);
}

// Devuelve el array de caracteres
inline const char* JString::Str() const
{
  return ((const char*)data);
}

// Concatenación de cadenas
inline void JString::Append(const JString& s)
{
  if (s.length)
  {
    u32 len;
    len = s.length;

    char *strAux = new char[length + len + 1];
    strAux[0] = 0;
    
    strcpy(strAux, data);
    delete[] data;
    
    // Evita contar los caracteres en strcat(), ya que sabemos la longitud
    strcpy(strAux + length, s.data);
    
    data = strAux;
    length += len;
  }
}

// Concatenación de cadenas
inline void JString::Append(const char *s)
{
  u32 len = (u32)strlen(s);

  if (len)
  {
    char *strAux = new char[length + len + 1];
    strAux[0] = 0;
    
    strcpy(strAux, data);
    delete[] data;
    
    // Evita contar los caracteres en strcat(), ya que sabemos la longitud
    strcpy(strAux + length, s);
    
    data = strAux;
    length += len;
  }
}

// Concatenación de cadenas
inline void JString::operator+=(const JString& s)
{
  if (s.length)
  {
    u32 len;
    len = s.length;

    char *strAux = new char[length + len + 1];
    strAux[0] = 0;
    
    strcpy(strAux, data);
    
    // Evita contar los caracteres en strcat(), ya que sabemos la longitud
    strcpy(strAux + length, s.data);
    
    delete[] data;
    data = strAux;
    length += len;
  }
}

// Concatenación de cadenas
inline void JString::operator+=(const char *s)
{
  u32 len = (u32)strlen(s);

  if (len)
  {
    char *strAux = new char[length + len + 1];
    strAux[0] = 0;
    
    strcpy(strAux, data);
    delete[] data;
    
    // Evita contar los caracteres en strcat(), ya que sabemos la longitud
    strcpy(strAux + length, s);
    
    data = strAux;
    length += len;
  }
}

// Concatena la cadena
inline const JString operator+(const JString &s1,  const char *s2)
{
	JString s(s1);
	s.Append(s2);
	return s;
}

inline const JString operator+(const JString &s1,  const JString &s2)
{
	JString s(s1);
	s.Append(s2);
	return s;
}

inline const JString operator+(const char *s1, const JString& s2)
{
	JString s(s1);
	s.Append(s2);
	return s;
}

// Concatena un caracter
inline void JString::Append(char c)
{
  if (c)
  {
    char *strAux = new char[length + 2];
    strAux[0] = 0;
    
    if (data)
    {
      strcpy(strAux, data);
      delete[] data;
    }
    
    strAux[length] = c;
    data = strAux;
    length += 1;
    strAux[length] = 0;
  }
}

// Concatena un caracter
inline void JString::operator+=(char c)
{
  if (c)
  {
    char *strAux = new char[length + 2];
    strAux[0] = 0;
    
    if (data)
    {
      strcpy(strAux, data);
      delete[] data;
    }
    
    strAux[length] = c;
    data = strAux;
    length += 1;
    strAux[length] = 0;
  }
}

// Formatea la cadena tipo printf
inline bool JString::Format(const char *fmt, ...)
{
	int n, size = 0;
	char *p = 0;

	va_list ap;

	while (1) 
	{
		// Intenta escribir hasta que la escritura tenga éxito
		va_start(ap, fmt);
		n = vsnprintf (p, size, fmt, ap);
		va_end(ap);

		// Si tiene éxito, sale del bucle para asignar la nueva cadena
		if (n > -1 && n < size)
			break;

		// Si no, intenta determinar el espacio necesario
		if (n > -1)    // Esto ocurre con glibc >= 2.1
		{
			// Justo lo necesario
			size = n+1;
		}
		else           // Esto ocurre con glibc < 2.1
		{
			// Incrementa el tamaño
			size += (64 + size);
		}

		if (p)
		{
			delete[] p;
		}

		if ((p = new char[size]) == 0)
		{
			// No hay memoria
			return false;
		}
	}
	
	delete[] data;
	data = p;
	length = size - 1;
	
	return true;
}

// Convierte a mayúsculas
void JString::Uppercase()
{
	char *p = data;
	while (*p)
	{
		*p = toupper(*p);
		++p;
	}
}

// Convierte a minúsculas
void JString::Lowercase()
{
	char *p = data;
	while (*p)
	{
		*p = tolower(*p);
		++p;
	}
}

// Devuelve la posición del primer carácter c desde el comienzo de la cadena
s32 JString::Find(char c, u32 from)
{
	if (from >= length)
	{
		return -1;
	}

	char *p;
	if ((p = strchr(data + from, c)))
	{
		if (p > data)
			return p - data;
		else
			return data - p;
	}

	return -1;
}

// Devuelve la posición del último carácter c desde el comienzo de la cadena
s32 JString::FindLast(char c, u32 from)
{
	if (from >= length)
	{
		return -1;
	}

	char *p;
	if ((p = strrchr(data + from, c)))
	{
		if (p > data)
			return p - data;
		else
			return data - p;
	}

	return -1;
}

// Devuelve la posición de la cadena str desde el comienzo de la cadena
s32 JString::Find(const char *str, u32 from)
{
	if (from >= length)
	{
		return -1;
	}

	char *p;
	if ((p = strstr(data + from, str)))
	{
		if (p > data)
			return p - data;
		else
			return data - p;
	}

	return -1;
}

// Reemplaza las ocurrencias 'pat' con 'rep' desde 'from', hasta 'max' veces
s32 JString::Replace(const char pat, const char rep, u32 from, u32 max)
{
	s32 i, num = 0;
	
	while ((i = Find(pat, from)) > -1)
	{
		data[i] = rep;
		++num;
	}
	
	return num;
}

// Reemplaza las ocurrencias 'pat' con 'rep' desde 'from', hasta 'max' veces
s32 JString::Replace(const char *pat, const char *rep, u32 from, u32 max)
{
	s32 cur, patLen, repLen, num = 0;
	
	patLen = strlen(pat);
	repLen = strlen(rep);
	
	if (patLen == repLen)
	{
		// Caso especial, esto se hace más rápido
		while ((cur = Find(pat, from)) > -1)
		{
			strncpy(data + cur, rep, repLen);
			++num;
			from = cur + patLen;
		}
		
		return num;
	}

	JString str(*this, 0, from);

	while ((cur = Find(pat, from)) > -1)
	{
		str += Substring(from, cur);
		str += rep;
		++num;
		from = cur + patLen;
	}
	
	str += Substring(from);
	*this = str;
	
	return num;
}

// Devuelve una subcadena de esta cadena
inline const JString JString::Substring(u32 start, u32 end)
{
	return JString(*this, start, end);
}

// Lee la cadena de un fichero (longitud + cadena)
inline u32 JString::Load(JRW &f)
{
  f.ReadLE32(&length);
  delete[] data;
  data = new char[length + 1];
  f.Read(data, length);
  data[length] = 0;

  return 0;
}

// Escribe la cadena a un fichero (longitud + cadena)
inline u32 JString::Save(JRW &f)
{
  f.WriteLE32(&length);
  f.Write(data, length);

  return 0;
}

#endif // _JSTRING_INCLUDED
