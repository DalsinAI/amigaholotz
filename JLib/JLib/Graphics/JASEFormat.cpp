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

/** Formato de ficheros ASE
 * @file    JASEFormat.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    20/09/2003
 * @version 0.0.1 - 20/09/2003 - Primera versión.
 */

#include <JLib/Graphics/JASEFormat.h>

u32 JASEFormat::nextTextureId;
u32 JASEFormat::textureBaseId;

// Destructor
JASEFormat::~JASEFormat()
{
  if (materials)
    delete[] materials;

  if (objects)
  {
    for (s32 i = 0; i < numObjects; ++i)
    {
      if (objects[i].vertices)
        delete[] objects[i].vertices;

      if (objects[i].tVertices)
        delete[] objects[i].tVertices;

      if (objects[i].vertexNormals)
        delete[] objects[i].vertexNormals;

      if (objects[i].faces)
        delete[] objects[i].faces;

      if (objects[i].tFaces)
        delete[] objects[i].tFaces;
    }

    delete[] objects;
  }
};
