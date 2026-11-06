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

/** Sistema de ficheros virtual.
 * @file    JFS.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    23/12/2003
 * @version 0.0.1 - 23/12/2003 - Primera versión.
 */

#include <JLib/Util/JFS.h>

// Carga la cabecera
u32 JResourceHeader::Load(JRW &f)
{
  if (0 != f.ReadLE32(&id) &&
			0 != f.ReadLE32(&pid) &&
      0 != f.ReadLE32(&type) &&
      0 != f.ReadLE32(&size) &&
      0 != f.ReadLE32(&flags))
  {
    return 0;
  }

  return 1;
}

// Salva la cebecera
u32 JResourceHeader::Save(JRW &f)
{
  if (0 != f.WriteLE32(&id) &&
			0 != f.WriteLE32(&pid) &&
      0 != f.WriteLE32(&type) &&
      0 != f.WriteLE32(&size) &&
      0 != f.WriteLE32(&flags))
  {
    return 0;
  }

  return 1;
}

u32 JResource::TypeOf(const char *filename)
{
	const int size = 13;
	const char *prefix[size] = {".txt",
															".c",
															".cpp",
															".h",
															".png",
															".tga",
															".jpg",
															".bmp",
															".xpm",
															".wav",
															".spr",
															".ttf",
															".jfs"};
	
	
	u32 types[size] = {JRES_TEXT,
										 JRES_TEXT,
										 JRES_TEXT,
										 JRES_TEXT,
										 JRES_IMAGE,
										 JRES_IMAGE,
										 JRES_IMAGE,
										 JRES_IMAGE,
										 JRES_IMAGE,
										 JRES_SOUND,
										 JRES_IMAGESPRITE,
										 JRES_FONT,
										 JRES_RESOURCEBLOCK};

	for (int i = 0; i < size; ++i)
	{
		if (strstr(filename, prefix[i]))
		{
			return types[i];
		}
	}

	return JRES_BINARY;
}

// Carga el recurso.
u32 JResource::Load(JRW &f, JLoadSave *where)
{
  u32 resp = 0;

  if (0 == (resp |= where->Load(f)))
  {
    loaded = true;
    data = where;
  }

  return resp;
}

bool JFS::ResizeAt(u32 offset, u32 numBytes)
{
	u32 orgPos = resFile.Tell();

	// TODO: Ver si hace falta
	resFile.Close();
	//resFile.Destroy();

	JFile f;
	
	if (!f.Open(resFilename, "r+b"))
	{
		perror("JFS::ResizeAt - No se pudo abrir el fichero");
		return false;
	}

	//	printf("Resizeando en byte %d, %d bytes\n", offset, numBytes);
	if (!f.ResizeAt(offset, numBytes))
	{
		perror("No se pudo cambiar el tamaño del fichero");
		f.Close();

		// Deja el jrw como estaba
		resFile.Create(resFilename, "r+b");
		resFile.Seek(orgPos, SEEK_SET);

		return false;
	}
	
	orgPos = f.Pos();
	f.Close();

	// Deja el jrw listo para escribir
	if (!resFile.Create(resFilename, "r+b"))
	{
		perror("JFS::ResizeAt - No se pudo volver a crear e jrw");
		return false;
	}

	resFile.Seek(orgPos, SEEK_SET);

	return true;
}

// Carga la cabecera del fichero
s32 JFS::LoadHeader()
{
	resFile.Seek(0, SEEK_SET);

	// Lee la cabecera
	char ident[9];
	if (!resFile.Read(ident, 8))
	{
		fprintf(stderr, "No se pudo leer la cabecera del fichero %s\n", resFilename.Str());
		return -1;
	}

	ident[8] = '\0';
	
	if (strcmp(ident, JRES_JFS_FILE) != 0)
	{
		fprintf(stderr, "%s no es un fichero JFS\n", resFilename.Str());
		return -2;
	}

	u8 major, minor;
	if (!resFile.Read(&major, 1) || !resFile.Read(&minor, 1))
	{
		fprintf(stderr, "No se pudo leer la versión de %s\n", resFilename.Str());
		return -3;
	}
	
	fprintf(stderr, "%s versión %d.%d\n", ident, major, minor);

	return 0;
}

// Salva la cabecera del fichero
s32 JFS::SaveHeader()
{
	resFile.Seek(0, SEEK_SET);

	// Escribe la cabecera
	if (!resFile.Write(JRES_JFS_FILE, 8))
	{
		fprintf(stderr, "No se pudo leer la cabecera del fichero %s\n", resFilename.Str());
		return -1;
	}

	u8 major = JRES_JFS_MAJOR, minor = JRES_JFS_MINOR;
	if (!resFile.Write(&major, 1) || !resFile.Write(&minor, 1))
	{
		fprintf(stderr, "No se pudo leer la versión de %s\n", resFilename.Str());
		return -3;
	}
	
	fprintf(stderr, "Salvado %s, %s versión %d.%d\n", resFilename.Str(), JRES_JFS_FILE, major, minor);

	return 0;
}

// Carga el índice de recursos
s32 JFS::LoadIndex()
{
	resFile.Seek(0, SEEK_END);
	fprintf(stderr, "Fin de fichero en %d\n", resFile.Tell());

	// Lee el índice al final del fichero
	resFile.Seek(-8, SEEK_END);
	fprintf(stderr, "Tamaño del índice en %d\n", resFile.Tell());

	u32 indexSize, indexCount;
	if (!resFile.ReadLE32(&indexSize) || !resFile.ReadLE32(&indexCount))
	{
		fprintf(stderr, "No se pudo cargar el índice\n");
		return -1;
	}

	resFile.Seek(-indexSize - 8, SEEK_END);

	index.resize(indexCount, 0);
	
	for (u32 i = 0; i < indexCount; ++i)
	{
		index[i] = new JFSIndexEntry;
		if (!index[i])
		{
			// No hay memoria
			return -2;
		} 
		
		index[i]->res = new JResource;

		if (!index[i]->res ||
				!resFile.ReadLE32(&index[i]->offset) ||
				!fprintf(stderr, "Leo en %d el offset %d\n", resFile.Tell() - 4, index[i]->offset) ||
				(0 != index[i]->name.Load(resFile)) ||
				(0 != index[i]->res->Header().Load(resFile)))
		{
			// Error al cargar la entrada
			return -3;
		}
	}

	return 0;
}

// Salva el índice de recursos
s32 JFS::SaveIndex()
{
	// Escribe el índice a partir del final del último recurso
	resFile.Seek(index[index.size() - 1]->offset + index[index.size() - 1]->res->Size(), SEEK_SET);
	
	fprintf(stderr, "index[index.size() - 1]->offset = %d, index[index.size() - 1]->res->Header().size = %d\n", index[index.size() - 1]->offset, index[index.size() - 1]->res->Header().size);

	u32 curPos = resFile.Tell();
	
	for (u32 i = 0; i < index.size(); ++i)
	{
		fprintf(stderr, "Escribo offset %d en %d, ", index[i]->offset, resFile.Tell());

		if (!index[i] || !resFile.WriteLE32(&index[i]->offset) ||
				(0 != index[i]->name.Save(resFile)) ||
				(0 != index[i]->res->Header().Save(resFile)))
		{
			return -1;
		}

		fprintf(stderr, "acabo en %d\n", resFile.Tell());
	}
	
	u32 indexSize = resFile.Tell() - curPos;
	u32 indexCount = index.size();
	
	curPos = resFile.Tell();
	resFile.Seek(0, SEEK_END);
	fprintf(stderr, "Fin de fichero está en %d\n", resFile.Tell());

	fprintf(stderr, "Escribo indexSize en %d como %d\n", resFile.Tell(), indexSize);

	if (0 == resFile.WriteLE32(&indexSize) || 0 == resFile.WriteLE32(&indexCount))
	{
		fprintf(stderr, "OOops, hubo un fallo gordo no se pudo salvar el tamaño o el count\n");
		return -2;
	}

	fprintf(stderr, "Fin de fichero en %d\n", resFile.Tell());

	return 0;
}

// Carga el fichero de recursos
s32 JFS::Load()
{
	s32 ret = 0;

	if (0 != (ret = LoadHeader()))
	{
		fprintf(stderr, "Error cargando la cabecera\n");
		return ret;
	}

	if (0 != (ret = LoadIndex()))
	{
		fprintf(stderr, "Error cargando el índice\n");
		return ret;
	}

	return 0;
}

// Carga el recurso dado.
u32 JFS::Load(u32 id, JLoadSave *where)
{
	if (id < index.size() && where)
	{
		if (index[id] && index[id]->res)
		{
			if (index[id]->res->data)
			{
				// Ya cargado
				return 1;
			}

			// Se posiciona para cargar el recurso
			resFile.Seek(index[id]->offset, SEEK_SET);
			index[id]->res->data = where;

			return where->Load(resFile);
		}		
	}
	
	// Id o lugar de carga no válidos
	return 2;
}

// Devuelve el recurso pedido
JLoadSave * JFS::Get(const JString &_name)
{
	for (u32 i = 0;  i < index.size(); ++i)
	{
		if (index[i] && index[i]->name == _name)
		{
			if (index[i]->res)
			{
				return index[i]->res->data;
			}

			// Encontrado pero no cargado
			return 0;
		}
	}

  return 0;
}

// Devuelve el recurso pedido
JLoadSave * JFS::Get(u32 id)
{
	if (id < index.size() && index[id] && index[id]->res)
	{
		return index[id]->res->data;
	}

  return 0;
}

// Borra el recurso dado
bool JFS::Delete(JString &_name)
{
//   JResource *res;
//   bool found = false;

//   for (it->FirstInBranch(); !found && it->HasNext(); it->Next())
//   {
//     res = it->Data();
//     if (res->Type() != JRES_RESOURCEBLOCK && _name == res->Name())
//     {
//       found = true;
//       it->RemoveNode();
//     }
//   }

//   if (found)
//   {
//     return true;
//   }

  return false;
}

u32 JFS::AddToIndex(u32 offset, const JString &name, JResource *res)
{
	if (!res || res->header.id > index.size())
	{
		// Índice fuera de rango o no hay recurso
		return 1;
	}

	JFSIndexEntry *e = new JFSIndexEntry;
	e->offset = offset;
	e->name = name;
	e->res = res;
	
	if (res->header.id == index.size())
	{
		fprintf(stderr, "\n+++ AddToIndex: Al final\n");
		// caso especial, va al final y no hay que ajustar nada
		index.push_back(e);
		return 0;
	}

	fprintf(stderr, "\n+++ AddToIndex: Entre medias\n");

	std::vector<JFSIndexEntry *>::iterator itt = index.begin();
	itt += res->header.id;
	index.insert(itt, e);

	// Actualiza los índices posteriores
	for (u32 i = res->header.id + 1; i < index.size(); ++i)
	{
		fprintf(stderr, "\n+++ AddToIndex: Actualizo el id de %d\n", i);
		++index[i]->res->Header().id;
	}
		
	return 0;
}

s32 JFS::AddTreeResource(JResource *res)
{
	// Añade el recurso al árbol. Si el nodo actual es un bloque lo hace dentro de él, tras el último nodo;
	// si es un recurso de otro tipo, lo inserta tras él.
	if (it->Data())
	{
		fprintf(stderr, "\nAddTreeResource: El nodo ya tiene datos!\n");
		return -1;
	}
	
	it->Data(res);

	// Asigna el identificador
	if (it->node->prev)
	{
		if (it->node->prev->child)
		{
			// Si el anterior tiene hijos, el id es el próximo al último de la rama
			JTree<JResource *>::Iterator *itTmp = NewIterator(it->node->prev->child);

			do
			{
				itTmp->LastInBranch();
			} while (itTmp->Child());
			
			res->Header().id = itTmp->node->data->Header().id + 1;
			//			fprintf(stderr, "Asigno id de hijo del anterior %d %s\n", itTmp->node->data->Header().id + 1, index[itTmp->node->data->Header().id]->name.Str());

			delete itTmp;
		}
		else
		{
			//			fprintf(stderr, "Asigno id de hijo del anterior %d %s\n", it->node->prev->data->Header().id + 1, index[it->node->prev->data->Header().id]->name.Str());
			res->Header().id = it->node->prev->data->Header().id + 1;
		}
	}
	else
	{
		res->Header().id = it->node->parent->data->Header().id + 1;
	}

	res->Header().pid = it->node->parent->data->Header().id;

	return 0;
}

s32 JFS::AddResource(const char *filename, u32 flags)
{
	JFile importFile;
	
	if (!importFile.Open(filename, "r+b"))
	{
		perror ("No se pudo abrir el fichero a importar");
		return -1;
	}

	// Añade un nuevo recurso al árbol, AddTreeResource calcula su id y su pid
	JResource *res = new JResource;

	if (0 != AddTreeResource(res))
	{
		fprintf(stderr, "No se pudo añadir el nodo al árbol.\n");
		delete res;

		return -2;
	}

	// Estos valores no los conoce AddTreeResource
	res->Header().type = JResource::TypeOf(filename);
	res->Header().flags = flags;

	// El offset es el del elemento con el id anterior
	u32 offset = index[res->Header().id-1]->offset + index[res->Header().id-1]->res->Size();

	fprintf(stderr, "AddResource: offset anterior %d + size anterior %d + hdr size %d\n", index[res->Header().id-1]->offset, index[res->Header().id-1]->res->Size(), index[res->Header().id-1]->res->header.Size());
	fprintf(stderr, "AddResource: Por tanto offset %d\n", offset);

	// Carga el buffer con los datos a salvar
	if (importFile.Size())
	{
		u8 *buff = new u8[importFile.Size()];

		if (!buff)
		{
			fprintf(stderr, "No hay memoria para importar el fichero\n");
			return -3;
		}

		if (0 == importFile.Read(buff, importFile.Size()))
		{
			fprintf(stderr, "Error al leer los datos del fichero a importar\n");
			delete[] buff;
			return -4;
		}

		if (JFS_COMPRESSED(res))
		{
			// Los datos deben comprimirse. Hay que hacerlo en memoria pues no sabemos a priori
			// cuánto ocuparán
			u32 sizeComp, size;
      unsigned long sizeUL, sizeCompUL;
			size = importFile.Size();
      sizeUL = size;

      // Zlib header says it can be computed with compressBounds but if so, it fails to compress2.
      // ZLib manual says that it must be 0.1% plus 12 bytes larger 8-?
			sizeComp = compressBound(size);
      sizeCompUL = sizeComp;
      
      printf("---> sizeComp es %lu, (uLongf*)&sizeComp es %lu\n", sizeUL, *((uLongf*)&sizeUL));

			unsigned char *buffComp = new unsigned char[sizeComp + 8];
	
			if (!buffComp)
			{
				delete[] buff;
				return -5;
			}
		
      s32 ret;
			if (Z_OK != (ret = compress2((Bytef*)buffComp, (uLongf*)&sizeCompUL, (Bytef*)buff, sizeUL, compressionLevel)))
			{
        printf("ret=%d, Z_MEM_ERROR=%d, Z_BUF_ERROR=%d, Z_STREAM_ERROR=%d\n", ret, Z_MEM_ERROR, Z_BUF_ERROR, Z_STREAM_ERROR);
				delete[] buff;
				delete[] buffComp;
				return -6;
			}

      sizeComp = (u32)sizeCompUL;

			res->Header().size = sizeof(size) + sizeof(sizeComp) + sizeComp;

			// Hace crecer el fichero de recursos desde la posición posterior al recurso actual.
			if (!ResizeAt(offset, res->Size()))
			{
				delete[] buff;
				delete[] buffComp;
				return -7;
			}
		
			// Tamaño original + Tamaño comprimido + datos comprimidos
			// Esto lo guarda en el formato que espera ZRead al cargar
			if (0 == resFile.WriteLE32(&size) || 0 == resFile.WriteLE32(&sizeComp) ||
					0 == resFile.Write(buffComp, sizeComp))
			{
				delete[] buff;
				delete[] buffComp;
				return -8;
			}
		
			delete[] buffComp;
		}
		else
		{
			res->Header().size = importFile.Size();

			if (!ResizeAt(offset, res->Size()))
			{
				delete[] buff;
				return -10;
			}

			if (0 == resFile.Write(buff, res->Header().size))
			{
				delete[] buff;
				return -11;
			}
		}

		delete[] buff;
	}
	else
	{
		// Fichero vacío
		res->Header().size = 0;
	}

	// Añade la entrada nueva al índice y ajusta las entradas por detrás
	AddToIndex(offset, filename, res);

	return 0;
}

s32 JFS::AddBlock(const char *name)
{
	// Añade el recurso al árbol. Si el nodo actual es un bloque lo hace dentro de él, tras el último nodo;
	// si es un recurso de otro tipo, lo inserta tras él.
	JResource *res = new JResource;

	if (0 != AddTreeResource(res))
	{
		fprintf(stderr, "Error al añadir un bloque al árbol.\n");
		delete res;

		return -1;
	}

	// Esta información no la conoce AddTreeBlock
	res->header.type = JRES_RESOURCEBLOCK;
	res->header.flags = JRES_FLAGS_BLOCK_OPENED;
	res->header.size = 0;

	// El offset es el del elemento con el id anterior
	u32 offset = index[res->header.id-1]->offset + index[res->header.id-1]->res->Size();

	// Hace crecer el fichero de recursos desde la posición posterior al recurso actual.
	if (!ResizeAt(offset, res->Size()))
	{
		fprintf(stderr, "Error al hacer resize!\n");
		return -10;
	}

  fprintf(stderr, "Añado al índice el bloque.\n");

	// Añade la entrada nueva al índice y ajusta las entradas por detrás
	AddToIndex(offset, name, res);

	return 0;
}

s32 JFS::BuildTree()
{
	if (!index.size())
	{
		// No hay índice
		fprintf(stderr, "BuildTree: No hay un índice cargado!\n");
		return -1;
	}

	// Borra todo el árbol
	it->Root();
	it->RemoveNode();
	
	if (it->Data())
	{
		delete it->Data();
		it->Data(0);
	}
	
	// Organiza los recursos en forma de árbol
	u32 i;
	u32 pid = 0;

	for (i = 0; i < index.size() - 1; ++i)
	{
		it->Data(index[i]->res);
		pid = index[i + 1]->res->Header().pid;

		if (index[i]->res->Header().type == JRES_RESOURCEBLOCK && pid == index[i]->res->Header().id)
		{
			// Recurso actual es bloque y tiene hijos
			it->AddBranchGo(0);
		}
		else
		if (pid == index[i]->res->Header().pid)
		{
			// Siguiente recurso al mismo nivel que el actual
			it->AddNodeGo(0);
		}
		else
		{
			while (it->Parent() && it->node->data->header.pid != pid)
			{}
			
			it->LastInBranch();
			it->AddNodeGo(0);
		}
	}
	
	it->Data(index[i]->res);
	
	return 0;
}

s32 JFS::Open(const char *_name, bool edit)
{
	if (!_name || ! JFile::Exists(_name))
	{
		// Nombre nulo o no exists el fichero
		return -1;
	}

	char str[4096];
	getcwd(str, sizeof(str));
	
	resFilename = str;
	resFilename += "/";
	resFilename += _name;
	
	if (!resFile.Create(resFilename, "r+b"))
	{
		// No se pudo abrir el fichero
		return -2;
	}
	
	if (0 != Load())
	{
		return -3;
	}

	if (it)
	{
		delete it;
	}
	
	it = NewIterator();
	it->Data(0);

	return BuildTree();
}

s32 JFS::Create(const char *_name)
{
	if (!_name)
	{
		// Nombre nulo
		return -1;
	}

	char str[4096];
	getcwd(str, sizeof(str));
	
	resFilename = str;
	resFilename += "/";
	resFilename += _name;
	
	if (!resFile.Create(resFilename, "w+b"))
	{
		// No se pudo abrir el fichero
		return -2;
	}
	
	if (0 != SaveHeader())
	{
		// No se pudo salvar la cabecera
		return -3;
	}

	// Índice vacío inicialmente
	u32 indexSize = 0, indexCount = 0;
	if (0 == resFile.WriteLE32(&indexSize) || 
			0 == resFile.WriteLE32(&indexCount))
	{
		return -4;
	}

	// Adición manual del primer recurso
	root->data = new JResource;
	root->data->header.id = 0;
	root->data->header.pid = 0;
	root->data->header.type = JRES_RESOURCEBLOCK;
	root->data->header.flags = JRES_FLAGS_BLOCK_OPENED;
	root->data->header.size = 0;
	
	// El offset es el tamaño de la cabecera
	u32 offset = JRES_JFS_HEADER_SIZE;

	// Hace crecer el fichero de recursos desde la posición posterior al recurso actual.
	if (!ResizeAt(offset, root->data->Size()))
	{
		return -10;
	}
		
	AddToIndex(JRES_JFS_HEADER_SIZE, _name, root->data);
	
	SaveIndex();

	if (it)
	{
		delete it;
	}
	
	it = NewIterator();
	
	return 0;
}

s32 JFS::Import(const char *filename, JTree<JResource *>::Node *where, bool after)
{
	static s32 depth = 0;
	fprintf(stderr, "Recursión %d, size es %d importo %s\n", depth, Size(), filename);
	
	char str[256];
	getcwd(str, 256);
	printf("CWD es %s\n", str);

	if (!filename || resFilename == "")
	{
		return -2;
	}

	if (depth == 0)
	{
		if (it)
		{
			delete it;
		}

		// Si nos dan un nodo (se asume que es de este árbol) mueve el iterador interno a esa posición,
		// si no, se posicionará el iterador al comienzo del árbol
		it = NewIterator(where);
	}

	printf("Depth es %d!\n", depth);

	if (JFile::IsDir(filename))
	{
		if (depth == 0)
		{
			if (JFS_BLOCKOPENED(it->Data()) || it->node == root)
			{
				printf("Añado rama inicial!\n");

				// Si es un bloque abierto o root
				if (it->Child())
				{
					// Tiene ya elementos, inserta antes del primero
					it->AddNodeGo(0, false);
				}
				else
				{
					// No tiene elementos, añade uno dentro
					it->AddBranchGo(0);
				}
			}
			else
			{
				printf("Añado nodo inicial!\n");

				// Bloque cerrado distinto de root o nodo, lo añade tras éste
				it->AddNodeGo(0, after);
			}
		}

		++depth;

		AddBlock(filename);
		fprintf(stderr, "Importando directorio: %*c%s\n", it->node->depth+1, '.', filename);

		s32 n, k;

		// Si es un directorio lo importa recursivamente
		struct dirent **namelist;
		n = scandir(filename, &namelist, 0, alphasort);

		if (n < 0)
		{
			perror("scandir");
			it->Parent();
			--depth;
			return -1;
		}
		
		s32 num = 0;

		if (n > 0)
		{
			chdir(filename);
			getcwd(str, 256);
			printf("CWD es %s\n", str);

			// Añade una entrada vacía para el primer elemento del directorio
			printf("Añado rama nueva al directorio!\n");
			it->AddBranchGo(0);

			// index[itt->node->data->Header().id]->name.Str()

			k = 0;
			while (k < n)
			{
				if (JFile::IsFile(namelist[k]->d_name) || (JFile::IsDir(namelist[k]->d_name) && (0 != strcmp(namelist[k]->d_name, ".") && 0 != strcmp(namelist[k]->d_name, ".."))))
				{
					++num;

					// Procesa recursivamente sus elementos
					printf("Voy a importar %s\n", namelist[k]->d_name);
					Import(namelist[k]->d_name);
					it->AddNodeGo(0);
				}
				free(namelist[k]);
				
				++k;
			}
			printf("No hay más elementos en %s\n", filename);

			// el último nodo (o la rama si el directorio estaba vacío) está vacío, lo borra
			if (it->Data())
			{
				printf("Horror, FALLO!!\n");
			}
			else
			{
				it->RemoveNode();
			}
			
			free(namelist);
			chdir("..");
			getcwd(str, 256);
			printf("CWD es %s\n", str);
		}

		//		printf("it->Parent()\n");
		if (num)
		{
			// Sólo pasa al padre si se importaron elementos del directorio pues si no
			// ya estamos en el padre tras hacer RemoveNode
			it->Parent();
		}
	}
	else if (JFile::IsFile(filename))
	{
		if (depth == 0)
		{
			if (JFS_BLOCKOPENED(it->Data()) || it->node == root)
			{
				printf("Añado rama!\n");

				// Si es un bloque abierto o root
				if (it->Child())
				{
					// Tiene ya elementos, inserta antes del primero
					it->AddNodeGo(0, false);
				}
				else
				{
					// No tiene elementos, añade uno dentro
					it->AddBranchGo(0);
				}
			}
			else
			{
				printf("Añado nodo!\n");

				// Bloque cerrado distinto de root o nodo, lo añade tras éste
				it->AddNodeGo(0, after);
			}
		}

		++depth;

		s32 ret = AddResource(filename, defaultFlags);

		fprintf(stderr, "Importando archivo: %s depth=%d, Addresource()=%d\n", filename, it->node->depth, ret);
	}

	if (!it->Data())
	{
		printf("-----> Nodo vacío, lo quito!! <-----\n");
		it->RemoveNode();
	}

	--depth;
	
	if (depth == 0)
	{
		// tras la última entrada actualiza el índice en disco
		return SaveIndex();
	}
	
	return 0;
}

s32 JFS::Export()
{
	JString dirName;
	int i = 1;
	
	// Busca un subdirectorio que no exista para exportar
	do
	{
		dirName.Format("export%d", i++);
	}	while(JFile::Exists(dirName));
	
// 	char str[256];
// 	getcwd(str, 256);
// 	printf("Estoy en %s\n", str);
// 	printf("Creo %s\n", dirName.Str());

#ifndef _WIN32
	if (0 != mkdir(dirName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
#else
	if (0 != mkdir(dirName))
#endif //_WIN32
	{
		perror("Error creating export base directory");
		return -1;
	}
	
	chdir(dirName);
	
	// No exporta el nodo raíz
	u32 id, lastPid;
	for (id = 1, lastPid = 0; id < index.size(); ++id)
	{
		//		printf("Lastpid es %s, cur.pid es %s\n", index[lastPid]->name.Str(), index[index[id]->res->Header().pid]->name.Str());

		while (lastPid && lastPid >= index[id]->res->Header().pid)
		{
			// Sube un directorio hasta que el padre sea el mismo que el del recurso actual
			//			getcwd(str, 256);
			//			printf("Estoy en %s\n", str);
			chdir("..");
			lastPid = index[lastPid]->res->Header().pid;
			//getcwd(str, 256);
			//printf("Subo a %s\n", str);
		}
		//printf("Ahora lastpid es %s, cur.pid es %s\n", index[lastPid]->name.Str(), index[index[id]->res->Header().pid]->name.Str());

		if (JFS_IS_BLOCK(index[id]->res))
		{
			// Bloque, sólo crea un directorio
			dirName = index[id]->name;

			//printf("Creo %s\n", dirName.Str());

#ifndef _WIN32
	    if (0 != mkdir(dirName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
#else
	    if (0 != mkdir(dirName))
#endif //_WIN32
			{
				perror("Error creating directory");
				printf("%s\n", dirName.Str());
				return -1;
			}

			int ret = chdir(dirName);

			//getcwd(str, 256);
			//printf("Cambio a %s\n", str);

			if (ret != 0)
			{
				perror("chdir falló");
			}
			else
			{
				// Cambio de padre
				lastPid = index[id]->res->Header().pid;
			}
		}
		else
		{
			//char str[256];
			//getcwd(str, 256);
			//printf("Estoy en %s\n", str);
		
			//printf("Creo archivo %s size es %d\n", index[id]->name.Str(), index[id]->res->Header().size);

			// Abre el fichero de destino
			JFile f;
			if (!f.Open(index[id]->name, "w+b"))
			{
				return -3;
			}
			
			// Fichero regular (pronto también links... :)
			// Este es el mismo código que para cargar un recurso
			resFile.Seek(index[id]->offset, SEEK_SET);
		
			if (index[id]->res->Header().size)
			{
				u8 *buff;
				u32 size;

				// Lee los datos desde el formato adecuado
				if (JFS_COMPRESSED(index[id]->res))
				{
					if (0 == (size = resFile.ZRead(&buff)))
					{
						fprintf(stderr, "JFS::Export - Error reading compressed resource\n");
						delete[] buff;
						
						return -4;
					}
				}
				else
				{
					size = index[id]->res->Header().size;
					buff = new u8[size];
					
					if (0 == resFile.Read(buff, size))
					{
						fprintf(stderr, "JFS::Export - Error reading resource\n");
						delete[] buff;
						
						return -5;
					}
				}


				// Escribe el fichero de destino
				if (!f.Write(buff, size))
				{
					delete[] buff;
					return -6;
				}

				delete[] buff;
			}
		}
	}

	while (lastPid && lastPid > 0)
	{
		// Sube un directorio hasta que el padre sea el mismo que el del recurso actual
		//getcwd(str, 256);
		//printf("FIN: Estoy en %s\n", str);
		chdir("..");
		lastPid = index[lastPid]->res->Header().pid;
		//getcwd(str, 256);
		//printf("FIN: Cambio a %s\n", str);
	}
	
	chdir("..");
	//getcwd(str, 256);
	//printf(".. a %s\n", str);
	chdir("..");
	//getcwd(str, 256);
	//printf("2 .. a %s\n", str);
	
	return 0;
}

const JString JFS::FilenameFromId(const char *name)
{
	JString ret = name;
	ret.Lowercase();
	s32 dotPos = ret.FindLast('_');

	// Pone el punto de la extensión (esto no funciona en id's que provengan de archivos con espacios sin extensión)
	if (dotPos >= 0)
	{
		ret[dotPos] = '.';
	}

	return ret;
}

const JString JFS::IdFromFilename(const char *name)
{
	JString ret;
	if (prefix.Length())
	{
		ret = prefix + "_" + name;
	}
	else
	{
		ret = name;
	}

	ret.Uppercase();
	ret.Replace(' ', '_');
	ret.Replace('.', '_');
	ret.Replace('-', '_');
	
	return ret;
}

bool JFS::ExportIndex(const char *filename)
{
	JTextFile f;
	if (!f.Open(filename, "w+b"))
	{
		perror("JFS::ExportIndex");
		return false;
	}
	
	if (!f.PrintLine(JRES_JFS_EXPORT_VERSION) ||
			!f.PrintLine(JRES_JFS_EXPORT_WARNING) ||
			!f.PrintLine(JRES_JFS_EXPORT_BEGIN"\n"))
	{
		perror("JFS::ExportIndex");
		return false;
	}
	
	char str[4096];
	JString exportName;
	s32 j = 0;

	for (u32 i = 0; i < index.size(); ++i)
	{
		exportName = IdFromFilename(index[i]->name.Str());

		j = index[i]->res->Header().pid;

		while (j > 0)
		{
			exportName = IdFromFilename(index[j]->name.Str()) + ("_" + exportName);
			j = index[j]->res->Header().pid;
		} 

		snprintf(str, sizeof(str), "#define %-40s %d", exportName.Str(), i);
		if (!f.PrintLine(str))
		{
			perror("JFS::ExportIndex");
			return false;
		}
	}

	if (!f.PrintLine("\n"JRES_JFS_EXPORT_END))
	{
		perror("JFS::ExportIndex");
		return false;
	}
	
	return true;
}
