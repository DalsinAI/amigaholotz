/** Wrapper de SDL_RWops
 * @file    JRW.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    23/Mar/2005
 * @version 0.0.1 - 23/Mar/2005 - Primera versión.
 */

#include <JLib/Util/JRW.h>

u32 JRW::ZRead(void **buff)
{
	u32 len, lenUncomp;
	
	// Original & compressed size
	if (!ReadLE32(&lenUncomp) ||
			!ReadLE32(&len))
	{
		//		perror("ZRead - Error 1");
		return 0;
	}

	//	printf("ZRead: lenUncomp %d, len %d\n", lenUncomp, len);

	u8 *buffComp = new u8[len];

	if (!buffComp)
	{
		//		perror("ZRead - Error 2");
		return 0;
	}

  u8 *buff_out = new u8[lenUncomp];

	if (!buff_out)
	{
		//		perror("ZRead - Error 3");
		delete[] buffComp;
		return 0;
	}

	if (0 >= SDL_RWread(rwops, buffComp, len, 1))
	{
		//		perror("ZRead - Error 4");
		delete[] buffComp;
		delete[] buff_out;
		return 0;
	}

  // For compatibility with zlib
	unsigned long lenUL, lenUncompUL;
  lenUncompUL = lenUncomp;
  lenUL = len;
  
	if (Z_OK != uncompress((Bytef*)buff_out, (uLongf*)&lenUncompUL, (Bytef*)buffComp, lenUL))
	{
		//		perror("ZRead - Error 5");
		delete[] buffComp;
		delete[] buff_out;
		return 0;
	}

	delete[] buffComp;
	*buff = buff_out;

	//	perror("Todo ok");

  return (u32)lenUncompUL;
}

u32 JRW::Import(const char *filename)
{
	JRW f;
	
	if (!f.Create(filename, "rb"))
	{
		return 0;
	}

	u32 size = f.Seek(0, SEEK_END);
	f.Seek(0, SEEK_SET);
	
	u8 *buff = new u8[size];
	
	if (!buff)
	{
		return 0;
	}
	
	u32 ret;
	ret = f.Read(buff, size, 1);

	if (!ret)
	{
		delete[] buff;
		return 0;
	}
	
	Create(buff, size);
	
	return ret;
}

u32 JRW::ZWrite(const void *buff, u32 size, s32 level)
{
	u32 sizeComp;
	sizeComp = compressBound(size);

	unsigned char *buffComp = new unsigned char[sizeComp + 8];
	
	if (!buffComp)
	{
		return 0;
	}

  // For compatibility with zlib
  unsigned long sizeCompUL, sizeUL;
  sizeCompUL = sizeComp;
  sizeUL = size;

	if (Z_OK != compress2((Bytef*)buffComp, (uLongf*)&sizeComp, (Bytef*)buff, size, level))
	{
		delete[] buffComp;
		return 0;
	}

  sizeComp = (u32)sizeCompUL;

	// Tamaño original + Tamaño comprimido + datos comprimidos
	if (0 == WriteLE32(&size) || 0 == WriteLE32(&sizeComp) ||
			SDL_RWwrite(rwops, buffComp, sizeComp, 1) <= 0)
	{
		delete[] buffComp;
		return 0;
	}

	delete[] buffComp;

	return sizeComp;
}

u32 JRW::Export(const char *filename, u32 size)
{
	JRW f;
	
	if (!f.Create(filename, "wb"))
	{
		return 0;
	}
	
	u8 *buff = new u8[size];
	
	if (!buff)
	{
		return 0;
	}
	
	u32 ret;
	ret = Read(buff, size, 1);

	if (!ret)
	{
		delete[] buff;
		return 0;
	}
	
	ret = f.Write(buff, size, 1);
	delete[] buff;
	
	return ret;
}
