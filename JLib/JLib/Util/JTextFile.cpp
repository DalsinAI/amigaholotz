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

/** Clase para manejo de ficheros de texto.
 * @file    JTextFile.cpp.
 * @author  Juan Carlos Seijo Pérez
 * @date    15/06/2003
 * @version 0.0.1 - 15/06/2003 - Primera versión.
 */

#include <JLib/Util/JTextFile.h>

s8 JTextFile::line = '\n';

// Constructor
JTextFile::JTextFile(const s8 *_name) : JFile(_name), ptr(0)
{
}

// Destructor
JTextFile::~JTextFile()
{
  JFile::Close();
}

// Develve el puntero a la posición actual
s8 * JTextFile::GetPos()
{
  return ptr;
}

// Establece el puntero a la posición actual
bool JTextFile::SetPos(s8 *_ptr)
{
  if (_ptr >= buff && _ptr < buff + buffSize)
  {
    ptr = _ptr;

    return true;
  }

  return false;
}

// Lee el fichero como en la clase base pero asigna el puntero de desplazamiento.
u32 JTextFile::Read(u32 readSize)
{
  u32 sz;
  sz = JFile::Read(readSize);
  ptr = buff;

  return sz;
}

// Carga todo el fichero como en la clase base pero asigna el puntero de desplazamiento y 
// añade un cero al final
bool JTextFile::Load(const char *filename, const char *mode)
{
	if (!Open(filename, mode))
	{
		return false;
	}

	u32 ret;
	
	FreeBuffer();
	buff = new char[statInfo.st_size + 1];

	if (!buff)
	{
		return false;
	}

	ret = fread(buff, sizeof(char), statInfo.st_size, file);
	
	if (ret < 0)
	{
		return false;
	}
	
	ptr = buff;
	
	buffSize = ret + 1;
	buff[buffSize - 1] = '\0';
	
	return true;
}

// Lee el próximo número decimal. Salta todos los caracteres que no sean números.
bool JTextFile::ReadFloat(float *f)
{
  //while (isspace(*ptr)) ++ptr;

  s8 str[16];
  if (ReadWord(str))
  {
    *f = (float)atof(str);

    return true;
  }
  
  return false;
}

// Lee el próximo número entero. Salta todos los caracteres que no sean números.
bool JTextFile::ReadInteger(s32 *i)
{
  //while (isspace(*ptr)) ++ptr;

  s8 str[16];
  if (ReadWord(str))
  {
    *i = atoi(str);

    return true;
  }
  
  return false;
}

// Avanza hasta el comienzo de la cadena dada. Comienza desde el principio si jump 
// es true y se alcanza el final del fichero (hasta que se alcanza la posición de inicio).
bool JTextFile::FindNext(const s8 *str, bool jump)
{
  s8 *tmp;
  
  tmp = strstr(ptr, str);

  if (tmp > 0)
    ptr = tmp;
  else
  {
    if (jump)
    {
      tmp = strstr(buff, str);

      if (tmp > 0)
        ptr = tmp;
      else
        return false;
    }
    else
      return false;
  }

  return true;
}

u32 JTextFile::CountString(const s8 *str, s8* init, s8* end)
{
	u32 count = 0;
  s8 *pos = GetPos();

	if (init == 0)
	{
		init = pos;
	}

	if (end == 0)
	{
		end = buff + buffSize - 1;
	}

	if (!SetPos(init) || !SetPos(end))
	{
		// Posición no válida
		return 0;
	}
  
	SetPos(init);

	while (FindNext(str) && ptr < end)
	{
		++count;
		++ptr;
	}

	SetPos(pos);
	
  return count;
}

// Lee y avanza el puntero
bool JTextFile::ReadWord(s8 *str)
{
	s8 *ptrEnd = buff + buffSize + 1;
  s32 i = 0;
	
  while (isspace(*ptr) && ptr < ptrEnd) ++ptr;
  while (!isspace(*ptr) && ptr < ptrEnd)
  {
    str[i++] = *ptr;
    ++ptr;
  }
  str[i] = 0;
  
  return i > 0;
}

bool JTextFile::ReadQuotedWord(s8 *str)
{
	s8 *org = ptr, *end;
  s32 i = 0;

  while (isspace(*ptr)) ++ptr;

	if (*ptr != '"')
	{
		// No hay comillas de inicio
		ptr = org;
		return false;
	}

	++ptr;
	end = ptr;
	
	if (0 >= (end = strstr(ptr, "\"")))
	{
		// No hay comillas de cierre
		ptr = org;
		return false;
	}

  while (*ptr != '"')
  {
    str[i++] = *ptr;
    ++ptr;
  }
	++ptr;
  str[i] = 0;
  
  return true;
}

// Salta la siguiente palabra y avanza el puntero
bool JTextFile::SkipNextWord()
{
  while (isspace(*ptr)) ++ptr;
  while (!isspace(*ptr)) ++ptr;
  while (isspace(*ptr)) ++ptr;

  return true;
}

// Mueve el puntero al comienzo del documento
void JTextFile::StartOfDocument()
{
  ptr = buff;
}

// Mueve el puntero al comienzo de la siguiente línea con texto.
bool JTextFile::NextLine()
{
	s8 *ptrOrg = ptr, *ptrEnd = buff + buffSize - 1;
	
  while (*ptr != line && ptr < ptrEnd)
	{
    ++ptr;
	}
  
	if (ptr == ptrEnd)
		return false;

  while (isspace(*ptr))
	{
    ++ptr;
	}

	return ptrOrg != ptr;
}

// Lee una línea del fichero desde la posición actual (sin contar espacios).
bool JTextFile::ReadLine(s8 *str)
{
  s8 *orgPtr = ptr;
  
  // Se salta los blancos 
  while (isspace(*ptr)) ++ptr;
  s8 *tmp = ptr;
  
  if (NextLine())
  {
    --ptr;

    // Vuelve atrás el número de blancos saltados
    while (isspace(*ptr)) --ptr;
		++ptr;

    strncpy(str, tmp, ptr - tmp);
		str[ptr - tmp] = '\0';

    return true;
  }

  ptr = orgPtr;

  return false;
}

// Escribe en el fichero sin salto de línea
u32 JTextFile::Print(const s8 *str)
{
  if (!file)
    return 0;
  
  return(fputs(str, file));
}

// Escribe en el fichero con salto de línea
u32 JTextFile::PrintLine(const s8 *str)
{
  if (!file)
    return 0;
  
  fputs(str, file);
  
  return(fputc(line, file));
}

// Escribe en el fichero con formato y sin salto de línea
u32 JTextFile::Printf(const s8 *fmt, ... )
{
  if (!file)
    return 0;

  va_list vlist;
  s8 str[MAX_LINE_LENGTH];

  va_start(vlist, fmt);
  vsprintf(str, fmt, vlist);
  va_end(vlist);

  return(fputs(str, file));
}

// Lectura tipo scanf
s32 JTextFile::Scanf(const char *format, ...)
{
  va_list vlist;
  s32 ret;
	
	u32 strSize = buffSize - (ptr - buff);
	if (strSize == 0)
	{
		return 0;
	}

	char *str = new char[strSize + 1];
	memcpy(str, ptr, strSize);
	str[strSize] = '\0';

  va_start(vlist, format);
  ret = vsscanf(str, format, vlist);
  va_end(vlist);

	delete[] str;

	return ret;
}
