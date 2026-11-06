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

///////////////////////////////////////////////////////////////////////////////
// @author: Juan Carlos Seijo Pérez
// @date: 15/06/2003
// @description: Clase base de ficheros
///////////////////////////////////////////////////////////////////////////////

#include <JLib/Util/JObject.h>
#include <JLib/Util/JFile.h>

u32 JFile::MAX_BLOCK_SIZE = JFILE_DEF_MAX_BLOCK_SIZE;

// Constructor
JFile::JFile(const s8 *filename) : file(0), buff(0), name(0), exists(false), size(0), buffSize(0)
{
	if (filename != 0)
	{
		name = new s8 [strlen(filename) + 1];
		strcpy(name, filename);

		Refresh();
	}
}

// Destructor
JFile::~JFile()
{
  Close();    // Quizá no sea necesario, ya que puede hacerlo el programador
	JDELETE_ARRAY(name);
}

// Vuelve a consultar la información del fichero
s32 JFile::Refresh()
{
	if (name)
	{
		s32 ret;
		if (0 != (ret = stat(name, &statInfo)))
		{
			if (errno == ENOENT)
				exists = false;
		}
		else
		{
			exists = true;
		}

		return ret;
	}

	return -2;
}

// Abre el fichero. mode como en open() de C. Si no existe actualiza 
// el flag de existencia.
bool JFile::Open(const s8 *filename, const s8 *mode)
{
  Close();

  if (filename)
	{
		JDELETE_ARRAY(name);

		name = new s8 [strlen(filename) + 1];
		strcpy(name, filename);

		// Refresca la información de fichero 'statInfo'
		Refresh();
	}

	if (name != 0)
	{
		if (0 != (file = fopen(name, mode)))
		{
			return true;
		}
	}

  return false;
}
  
// Lee el fichero. Si no se especifica readsize o es cero, lo lee entero
u32 JFile::Read(u32 readSize)
{
  if (!readSize)
    readSize = statInfo.st_size;
  
  FreeBuffer();

  buff = new s8[readSize];

  return (buffSize = (u32)fread(buff, sizeof(s8), readSize, file));
}

u32 JFile::ZRead()
{
  FreeBuffer();

	u32 len, lenUncomp;
	
	// Original & compressed size
	if (!ReadLE32(&lenUncomp) ||
			!ReadLE32(&len))
	{
		return 0;
	}

	unsigned char *buffComp = new unsigned char[len];

	if (!buffComp)
	{
		return 0;
	}

	buffSize = lenUncomp;
  buff = new s8[lenUncomp];

	if (!buff)
	{
		delete[] buffComp;
		return 0;
	}

	if (0 >= fread(buffComp, len, 1, file) && !feof(file))
	{
		delete[] buffComp;
		delete[] buff;
		return 0;
	}

  // For compatibility with zlib
	unsigned long lenUL, lenUncompUL;
  lenUncompUL = lenUncomp;
  lenUL = len;
  
	if (Z_OK != uncompress((Bytef*)buff, (uLongf*)&lenUncompUL, (Bytef*)buffComp, lenUL))
	{
		delete[] buffComp;
		delete[] buff;
		return 0;
	}

	delete[] buffComp;
  
  return (u32)(lenUncompUL + 8);
}

// Carga todo el fichero. Equivalente a Open() + Read(0).
bool JFile::Load(const s8 *filename, const s8 *mode)
{
  if (Open(filename, mode))
	{
    return 0 < Read();
	}

  return false;
}

// Lee del fichero
u32 JFile::Read(void *_buff, u32 _size)
{
  return (u32)fread(_buff, sizeof(s8), _size, file);
}

u32 JFile::ReadBool(bool *_buff) {u8 b; if (0 < Read(&b, 1)) {*_buff = (b ? true : false); return 1;} return 0;}

u32 JFile::Read8(u8 *_buff) {if (0 < Read(_buff, 1)) return 1; return 0;}

u32 JFile::Read8(s8 *_buff) {if (0 < Read(_buff, 1)) return 1; return 0;}

u32 JFile::ReadLE16(u16 *_buff) {if (0 < Read(_buff, 2)) {*_buff = SDL_SwapLE16(*_buff); return 2;} return 0;}

u32 JFile::ReadLE16(s16 *_buff) {if (0 < Read(_buff, 2)) {*_buff = SDL_SwapLE16(*_buff); return 2;} return 0;}

u32 JFile::ReadBE16(u16 *_buff) {if (0 < Read(_buff, 2)) {*_buff = SDL_SwapBE16(*_buff); return 2;} return 0;}

u32 JFile::ReadBE16(s16 *_buff) {if (0 < Read(_buff, 2)) {*_buff = SDL_SwapBE16(*_buff); return 2;} return 0;}

u32 JFile::ReadLE32(u32 *_buff) {if (0 < Read(_buff, 4)) {*_buff = SDL_SwapLE32(*_buff); return 4;} return 0;}

u32 JFile::ReadLE32(s32 *_buff) {if (0 < Read(_buff, 4)) {*_buff = SDL_SwapLE32(*_buff); return 4;} return 0;}

u32 JFile::ReadBE32(u32 *_buff) {if (0 < Read(_buff, 4)) {*_buff = SDL_SwapBE32(*_buff); return 4;} return 0;}

u32 JFile::ReadBE32(s32 *_buff) {if (0 < Read(_buff, 4)) {*_buff = SDL_SwapBE32(*_buff); return 4;} return 0;}

u32 JFile::ZRead(void * &_buff)
{
	u32 len, lenUncomp;
	
	// Original & compressed size
	if (!ReadLE32(&lenUncomp) ||
			!ReadLE32(&len))
	{
		return 0;
	}

	unsigned char *buffComp = new unsigned char[len];

	if (!buffComp)
	{
		return 0;
	}

	unsigned char *buffUncomp = new unsigned char[lenUncomp];

	if (!buffUncomp)
	{
		delete[] buffComp;
		return 0;
	}

	if (0 >= fread(buffComp, len, 1, file) && !feof(file))
	{
		delete[] buffComp;
		delete[] buffUncomp;
		return 0;
	}

  // For compatibility with zlib
	unsigned long lenUL, lenUncompUL;
  lenUncompUL = lenUncomp;
  lenUL = len;
  
	if (Z_OK != uncompress((Bytef*)buffUncomp, (uLongf*)&lenUncompUL, (Bytef*)buffComp, lenUL))
	{
		delete[] buffComp;
		delete[] buffUncomp;
		return 0;
	}

	delete[] buffComp;

  return (u32)(lenUncompUL);
}

// Lee del fichero al buffer dado y deja el puntero 
// en su posición original
u32 JFile::Peep(void *_buff, u32 _size)
{
  s32 p = Pos();
  u32 r = (u32)fread(_buff, _size, 1, file);
  Pos(p);

  return r;
}

u32 JFile::WriteBool(bool *_buff) {u8 b = ((*_buff) ? 1 : 0);return Write(&b, 1);}

u32 JFile::Write8(u8 *_buff) {return Write(_buff, 1);}

u32 JFile::Write8(s8 *_buff) {return Write(_buff, 1);}

u32 JFile::WriteLE16(u16 *_buff) {u16 v = SDL_SwapLE16(*_buff); return Write(&v, 2);}

u32 JFile::WriteLE16(s16 *_buff) {u16 v = SDL_SwapLE16(*_buff); return Write(&v, 2);}

u32 JFile::WriteBE16(u16 *_buff) {u16 v = SDL_SwapBE16(*_buff); return Write(&v, 2);}

u32 JFile::WriteBE16(s16 *_buff) {u16 v = SDL_SwapBE16(*_buff); return Write(&v, 2);}

u32 JFile::WriteLE32(u32 *_buff) {u32 v = SDL_SwapLE32(*_buff); return Write(&v, 4);}

u32 JFile::WriteLE32(s32 *_buff) {u32 v = SDL_SwapLE32(*_buff); return Write(&v, 4);}

u32 JFile::WriteBE32(u32 *_buff) {u32 v = SDL_SwapBE32(*_buff); return Write(&v, 4);}

u32 JFile::WriteBE32(s32 *_buff) {u32 v = SDL_SwapBE32(*_buff); return Write(&v, 4);}

// Escribe en el fichero
u32 JFile::Write(const void *_buff, u32 _size)
{
  return (u32) fwrite(_buff, sizeof(s8), _size, file);
}
  
u32 JFile::ZWrite(const void *_buff, u32 _size, s32 level)
{
	u32 sizeComp;
	sizeComp = compressBound(_size);

	unsigned char *buffComp = new unsigned char[sizeComp + 8];
	
	if (!buffComp)
	{
		return 0;
	}

  // For compatibility with zlib
  unsigned long sizeCompUL, sizeUL;
  sizeCompUL = sizeComp;
  sizeUL = _size;

	if (Z_OK != compress2((Bytef*)buffComp, (uLongf*)&sizeCompUL, (Bytef*)_buff, sizeUL, level))
	{
		delete[] buffComp;
		return 0;
	}

  sizeComp = (u32)sizeCompUL;
  
	// Tamaño original + Tamaño comprimido + datos comprimidos
	if (0 == WriteLE32(&_size) || 0 == WriteLE32(&sizeComp) ||
			fwrite(buffComp, sizeComp, 1, file) <= 0)
	{
		delete[] buffComp;
		return 0;
	}

	delete[] buffComp;

	return sizeComp + 8;
}

// Cierra el fichero. Devuelve true si liberó memoria, false si no lo hizo.
bool JFile::Close()
{
  if (!(file || buff))
    return false;

  if (file)
  {
    fclose(file);
    file = 0;
  }

  FreeBuffer();

  return true;
}

// Situa el puntero al inicio
void JFile::Rewind()
{
  rewind(file);
}

// Libera la memoria del buffer de forma segura
void JFile::FreeBuffer()
{
  if (buff)
  {
    delete[] buff;
    buff = 0;
  }
}

// Determina si existe el fichero
bool JFile::Exists(const s8 *name)
{
	if (name != 0)
  {
	  struct stat s;
	  if (0 != stat(name, &s))
	  {
		  if (errno == ENOENT)
		  {
			  return false;
		  }
	  }
		
		return true;
  }

  return false;
}

// Determina si existe el fichero
u32 JFile::Size(const s8 *name)
{
	if (name == 0)
		return 0;

	struct stat s;
	if (0 == stat(name, &s))
	{
		return s.st_size;
	}
	
	return 0;
}

// Determina si el fichero dado es del tipo dado
bool JFile::IsOfType(const s8 *name, mode_t mode)
{
	if (name == 0)
		return false;

	struct stat s;
	if (0 == stat(name, &s))
	{
		return 0 != (s.st_mode & mode);
	}
	
	return false;
}

// Determina si existe el fichero
bool JFile::IsFile(const s8 *name)
{
	return IsOfType(name, S_IFREG);
}

// Determina si existe el fichero
bool JFile::IsDir(const s8 *name)
{
	return IsOfType(name, S_IFDIR);
}

// Determina si existe el fichero
bool JFile::IsLink(const s8 *name)
{
	return IsOfType(name, S_IFLNK);
}

// Determina si existe el fichero
bool JFile::IsCharDev(const s8 *name)
{
	return IsOfType(name, S_IFCHR);
}

// Determina si existe el fichero
bool JFile::IsBlockDev(const s8 *name)
{
	return IsOfType(name, S_IFBLK);
}

bool JFile::ResizeBack(u32 numBytes)
{
	if (numBytes == 0)
	{
		return false;
	}
	
	if (0 != fseek(file, 0, SEEK_END))
	{
		return false;
	}

	u32 pos = ftell(file);

	if (0 != fseek(file, numBytes, SEEK_END) ||
			0 != fseek(file, pos, SEEK_SET))
	{
		return false;
	}

	return true;
}

bool JFile::ResizeAt(u32 byte, s32 numBytes)
{
	if (numBytes < 0)
	{
		return false;
	}

	if (numBytes == 0)
	{
		// Tan solo lo deja preparado para copiar en la zona dada
		fseek(file, byte, SEEK_SET);
		return true;
	}

	if (0 != fseek(file, 0, SEEK_END))
	{
		return false;
	}
	
	u32 oldEnd = ftell(file);
	
	if (byte > oldEnd)
	{
		byte = oldEnd;
	}

	u32 remSize = oldEnd - byte;        // Espacio restante por desplazar
	u32 copiedSize = 0;                 // Espacio ya copiado

	// Establece un tamaño de bloque de copia adecuado
	u32 blockSize = MAX_BLOCK_SIZE < remSize ? MAX_BLOCK_SIZE : remSize;
	
	//	printf("BlockSize es %d\n", blockSize);

	// Cambia el tamaño del fichero
	fseek(file, numBytes, SEEK_END);
	int ret;

	if (!remSize)
	{
		fseek(file, 0, SEEK_SET);

		return true;
	}

	// Guarda la nueva posición de fin
	u32 newEnd = ftell(file);

	u8 *copyBuff = new u8[blockSize];

	// Desplaza el trozo final
	while (remSize >= blockSize)
	{
		if (0 != fseek(file, oldEnd - copiedSize - blockSize, SEEK_SET))
			perror("1");

		if (0 > (ret = fread(copyBuff, blockSize, 1, file)) || feof(file))
			perror("2");

		if (0 != fseek(file, newEnd - copiedSize - blockSize, SEEK_SET))
			perror("3");

		if (0 >= (ret = fwrite(copyBuff, blockSize, 1, file)))
			perror("4");

		copiedSize += blockSize;
		remSize -= blockSize;
	}

	if (remSize > 0)
	{
		// Copia el trozo final, si queda algo
		blockSize = remSize;
		if (0 != fseek(file, oldEnd - copiedSize - blockSize, SEEK_SET))
			perror("1");

		if (0 > (ret = fread(copyBuff, blockSize, 1, file)) || feof(file))
			perror("2");

		if (0 != fseek(file, newEnd - copiedSize - blockSize, SEEK_SET))
			perror("3");

		if (0 >= (ret = fwrite(copyBuff, blockSize, 1, file)))
			perror("4");

		copiedSize += blockSize;
		remSize -= blockSize;
	}

	//	fprintf(stderr, "Total por copiar %d bytes, copiados %d bytes newEnd es %d\n", remSize, copiedSize, newEnd);

	if (copiedSize != oldEnd - byte)
	{
		fprintf(stderr, "No coincide el tamaño copiado (orig era %d, copiados %d)\n", oldEnd - byte, copiedSize);
	}

	if (remSize != 0)
	{
		fprintf(stderr, "Faltan datos por copiar (remSize != 0)\n");
	}

	// Deja el puntero al comienzo de la zona vacía
	fseek(file, byte, SEEK_SET);
	
	fflush(file);
	
	delete[] copyBuff;
	
	return true;
}
