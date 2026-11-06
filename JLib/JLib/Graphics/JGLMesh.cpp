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

/** Clases de geometría 3D para OpenGL.
 * @file    JGLMesh.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    10/05/2004
 * @version 0.0.1 - 10/05/2004 - Primera versión.
 */

#include <JLib/Graphics/JGLMesh.h>

/** Dibuja la geometría.
 */
void JGLMesh::Draw()
{
  // Pinta el objeto cargado
  for (int i = 0; i < numObjects; ++i)
  {
    // Por cada cara (flat)
    for (int j = 0; j < objects[i].numFaces; ++j)
    {
      if (objects[i].faces[j].material->Texture())
      {
        glDisable(GL_COLOR_MATERIAL);
        glEnable(GL_TEXTURE_2D);
				objects[i].faces[j].material->Texture()->GLBind();

				glBegin(GL_TRIANGLES);
				glNormal3fv((GLfloat*)&objects[i].vertexNormals[objects[i].faces[j].v1]);
				glTexCoord2f(objects[i].tVertices[objects[i].tFaces[j].v1].u,
				             objects[i].tVertices[objects[i].tFaces[j].v1].v);
				glVertex3fv((GLfloat*)&objects[i].vertices[objects[i].faces[j].v1]);
      
				glNormal3fv((GLfloat*)&objects[i].vertexNormals[objects[i].faces[j].v2]);
				glTexCoord2f(objects[i].tVertices[objects[i].tFaces[j].v2].u,
				             objects[i].tVertices[objects[i].tFaces[j].v2].v);
				glVertex3fv((GLfloat*)&objects[i].vertices[objects[i].faces[j].v2]);
      
				glNormal3fv((GLfloat*)&objects[i].vertexNormals[objects[i].faces[j].v3]);
				glTexCoord2f(objects[i].tVertices[objects[i].tFaces[j].v3].u,
				             objects[i].tVertices[objects[i].tFaces[j].v3].v);
				glVertex3fv((GLfloat*)&objects[i].vertices[objects[i].faces[j].v3]);
				glEnd();
      }
      else
      {
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_COLOR_MATERIAL);
        glColor3fv((GLfloat*)&objects[i].faces[j].material->diffuse);

				glBegin(GL_TRIANGLES);
				glNormal3fv((GLfloat*)&objects[i].vertexNormals[objects[i].faces[j].v1]);
				glVertex3fv((GLfloat*)&objects[i].vertices[objects[i].faces[j].v1]);
      
				glNormal3fv((GLfloat*)&objects[i].vertexNormals[objects[i].faces[j].v2]);
				glVertex3fv((GLfloat*)&objects[i].vertices[objects[i].faces[j].v2]);
      
				glNormal3fv((GLfloat*)&objects[i].vertexNormals[objects[i].faces[j].v3]);
				glVertex3fv((GLfloat*)&objects[i].vertices[objects[i].faces[j].v3]);
				glEnd();
      }
    }
	}
}

void JMaterial::Destroy()
{
	JDELETE(texture);
}

void JGLMeshObject::Destroy()
{
	JDELETE_ARRAY(vertices);
	JDELETE_ARRAY(tVertices);
	JDELETE_ARRAY(vertexNormals);
	JDELETE_ARRAY(faces);
	JDELETE_ARRAY(tFaces);
}

void JGLMesh::Destroy()
{
	JDELETE_ARRAY(materials);
	JDELETE_ARRAY(objects);
}

bool JGLMesh::LoadASE(const JString &filename)
{
	// Destruye posibles geometrías anteriores
	Destroy();

  if (!f.Load(filename))
    return false;

	// La cabecera debe coincidir, no exigimos tanto a la versión,
  // al menos lo intentamos
  if (!f.FindNext(JASETAG_FILE_HEADER) || !f.SkipNextWord())
    return false;

	/*
  // Versión del fichero
  f.ReadInteger(&fileVersion);
  
  // Primer frame
  if (!f.FindNext(JASETAG_SCENE_FIRSTFRAME) || !f.SkipNextWord())
    firstFrame = 0;
  else
    f.ReadInteger(&firstFrame);

  // Último frame
  if (!f.FindNext(JASETAG_SCENE_LASTFRAME) || !f.SkipNextWord())
    lastFrame = 0;
  else
    f.ReadInteger(&lastFrame);
	*/

  if (LoadASEMaterials())
    if (LoadASEObjects())
      return true;

  return false;
}

// Carga los materiales del fichero ASE.
// Devuelve true si todo fue bien, false en caso contrario.
bool JGLMesh::LoadASEMaterials()
{
  f.StartOfDocument();

  if (!f.FindNext(JASETAG_MATERIAL_COUNT))
    return false;

  f.SkipNextWord();
  f.ReadInteger(&numMaterials);
  
  // Contamos como materiales los submateriales que haya
  f.StartOfDocument();
  s32 c = 0;
  while (f.FindNext(JASETAG_SUBMATERIAL_COUNT))
  {
    f.SkipNextWord();
    f.ReadInteger(&c);
    numMaterials += c;
  }

  f.StartOfDocument();
  materials = new JMaterial[numMaterials];
  memset(materials, 0, sizeof(JMaterial) * numMaterials);
  
  JMaterial *mats = materials;
  s32 i = 0;
  while (i < numMaterials)
  {
    // En el nivel superior no hay submateriales
    mats[i].parentId = -1;
    mats[i].isSubMaterial = false;

    f.FindNext(JASETAG_MATERIAL_LIST);
    f.SkipNextWord();
    f.FindNext(JASETAG_MATERIAL_COUNT);
    f.SkipNextWord();
    f.FindNext(JASETAG_MATERIAL);
    f.SkipNextWord();
    f.ReadInteger(&mats[i].id);

    // Tipo de material
    f.FindNext(JASETAG_MATERIAL_CLASS);
    f.SkipNextWord();
    f.ReadWord(mats[i].mtlClass);

    // Color ambiente
    f.FindNext(JASETAG_MATERIAL_AMBIENT); f.SkipNextWord();
    f.ReadFloat(&mats[i].ambient.r); f.ReadFloat(&mats[i].ambient.g); f.ReadFloat(&mats[i].ambient.b);

    // Color difuso
    f.FindNext(JASETAG_MATERIAL_DIFFUSE); f.SkipNextWord();
    f.ReadFloat(&mats[i].diffuse.r); f.ReadFloat(&mats[i].diffuse.g); f.ReadFloat(&mats[i].diffuse.b);
    
    // Color especular
    f.FindNext(JASETAG_MATERIAL_SPECULAR); f.SkipNextWord();
    f.ReadFloat(&mats[i].specular.r); f.ReadFloat(&mats[i].specular.g); f.ReadFloat(&mats[i].specular.b);
    
    // Brillo
    f.FindNext(JASETAG_MATERIAL_SHINE); f.SkipNextWord();
    f.ReadFloat(&mats[i].shine);

    // Fuerza de brillo
    f.FindNext(JASETAG_MATERIAL_SHINESTRENGTH); f.SkipNextWord();
    f.ReadFloat(&mats[i].shineStrenght);

    // Transparencia
    f.FindNext(JASETAG_MATERIAL_TRANSPARENCY); f.SkipNextWord();
    f.ReadFloat(&mats[i].transparency);

    s8 str[32];   // Cadena auxiliar

    if (0 == strcmp(mats[i].mtlClass, JASEVAL_MULTI_SUBOBJECT))
    {
      // Material multi/subobjeto: procesamos los submateriales
      // No tiene el parámetro selfIllum
      mats[i].isMulti = true;
      
      f.FindNext(JASETAG_SUBMATERIAL_COUNT);
      f.SkipNextWord();
      f.ReadInteger(&mats[i].numSubMaterials);

      for (s32 j = i+1; j < i + 1 + mats[i].numSubMaterials; ++j)
      {
        mats[j].parentId = mats[i].id;
        f.FindNext(JASETAG_SUBMATERIAL); f.SkipNextWord();
        f.ReadInteger(&mats[j].id);

        // Sólo está permitido un nivel de jerarquía, así que no comprobamos
        // si es Multi/Sub-object
        f.FindNext(JASETAG_MATERIAL_CLASS);
        f.SkipNextWord();
        f.ReadWord(mats[j].mtlClass);
        mats[j].isSubMaterial = true;
      
        // Color ambiente
        f.FindNext(JASETAG_MATERIAL_AMBIENT); f.SkipNextWord();
        f.ReadFloat(&mats[j].ambient.r); f.ReadFloat(&mats[j].ambient.g); f.ReadFloat(&mats[j].ambient.b);

        // Color difuso
        f.FindNext(JASETAG_MATERIAL_DIFFUSE); f.SkipNextWord();
        f.ReadFloat(&mats[j].diffuse.r); f.ReadFloat(&mats[j].diffuse.g); f.ReadFloat(&mats[j].diffuse.b);
        
        // Color especular
        f.FindNext(JASETAG_MATERIAL_SPECULAR); f.SkipNextWord();
        f.ReadFloat(&mats[j].specular.r); f.ReadFloat(&mats[j].specular.g); f.ReadFloat(&mats[j].specular.b);
        
        // Brillo
        f.FindNext(JASETAG_MATERIAL_SHINE); f.SkipNextWord();
        f.ReadFloat(&mats[j].shine);

        // Fuerza de brillo
        f.FindNext(JASETAG_MATERIAL_SHINESTRENGTH); f.SkipNextWord();
        f.ReadFloat(&mats[j].shineStrenght);

        // Transparencia
        f.FindNext(JASETAG_MATERIAL_TRANSPARENCY); f.SkipNextWord();
        f.ReadFloat(&mats[j].transparency);

        // Iluminación propia
        f.FindNext(JASETAG_MATERIAL_SELFILLUM); f.SkipNextWord();
        f.ReadFloat(&mats[j].selfIllum);

        // Si tiene una textura asociada aparecerá en la línea siguiente a
        // MATERIAL_XP_TYPE
        f.FindNext(JASETAG_MATERIAL_XP_TYPE);
        f.NextLine();
        f.ReadWord(str);

        if (strcmp(str, JASETAG_MATERIAL_MAP_DIFFUSE) == 0)
        {
					char texFile[256];
          mats[j].texture = new JGLTexture;
          
          f.FindNext(JASETAG_MATERIAL_BITMAP); f.SkipNextWord();
          f.ReadWord(texFile);
          
					// Quita las comillas
					u32 len = (u32)strlen(texFile);
          memmove(&texFile[0], &texFile[1], len - 1);
          texFile[len - 2] = 0;
					
					/**< @todo Comprobar rendimiento con mipmaps y sin ellos. */
					// Carga la textura con mipmaps
					mats[j].texture->Load(texFile, true);
        }
      }
    }
    else
    {
      // Material standard u otro
      mats[i].isMulti = false;
      mats[i].numSubMaterials = 0;

      // Iluminación propia
      f.FindNext(JASETAG_MATERIAL_SELFILLUM); f.SkipNextWord();
      f.ReadFloat(&mats[i].selfIllum);

      // Si tiene una textura asociada aparecerá en la línea siguiente a
      // MATERIAL_XP_TYPE
      f.FindNext(JASETAG_MATERIAL_XP_TYPE);
      f.NextLine();
      f.ReadWord(str);

      if (strcmp(str, JASETAG_MATERIAL_MAP_DIFFUSE) == 0)
      {
				char texFile[256];

        mats[i].texture = new JGLTexture;
        
        f.FindNext(JASETAG_MATERIAL_BITMAP); f.SkipNextWord();
        f.ReadWord(texFile);
        u32 len = (u32)strlen(texFile);
        memmove(&texFile[0], &texFile[1], len - 1);
        texFile[len - 2] = 0;
				
				// Quita las comillas
				u32 leng = (u32)strlen(texFile);
				memmove(&texFile[0], &texFile[1], leng - 1);
				texFile[leng - 2] = 0;
					
				/**< @todo Comprobar rendimiento con mipmaps y sin ellos. */
				// Carga la textura con mipmaps
				mats[i].texture->Load(texFile, true);
      }
    }

    // Acumulamos los submateriales procesados y/o el actual
    i += mats[i].numSubMaterials + 1;
  }

  return true;
}

// Carga los objetos (geometría) del fichero ASE.
// Devuelve true si todo fue bien, false en otro caso.
bool JGLMesh::LoadASEObjects()
{
  f.StartOfDocument();

  // Cuenta los objetos
  numObjects = 0;
  while (f.FindNext(JASETAG_GEOMOBJECT)) 
  {
    f.SkipNextWord();
    ++numObjects;
  }

  // Crea el array de objetos
  objects = new JGLMeshObject[numObjects];
  JGLMeshObject *objs = objects;

  f.StartOfDocument();
	char str[256];

  for (s32 i = 0; i < numObjects; ++i)
  {
    // Nombre
    f.FindNext(JASETAG_NODE_NAME); f.SkipNextWord();
    f.ReadWord(str);
    
    // Quita las comillas
    u32 len = (u32)strlen(str);
    memmove(&str[0], &str[1], len - 1);
    str[len - 2] = 0;
		objs[i].name = str;

    // Procesa la geometría...
    f.FindNext(JASETAG_MESH); f.SkipNextWord();
    s8 *oldPos = f.GetPos();
    
    // Número de vértices: crea el array de vértices y de normales
    if (i == 64)
      i = i;
    f.FindNext(JASETAG_MESH_NUMVERTEX); f.SkipNextWord();
    f.ReadInteger(&objs[i].numVertices);
    objs[i].vertices = new JVertex[objs[i].numVertices];

    JVertex *verts = objs[i].vertices;

    // Número de caras: crea el array de caras
    f.FindNext(JASETAG_MESH_NUMFACES); f.SkipNextWord();
    f.ReadInteger(&objs[i].numFaces);
    objs[i].faces = new JFace[objs[i].numFaces];

    JFace *faces = objs[i].faces;

    // Lista de vértices
    f.FindNext(JASETAG_MESH_VERTEX_LIST); f.SkipNextWord();
    for (s32 j = 0; j < objs[i].numVertices; ++j)
    {
      f.FindNext(JASETAG_MESH_VERTEX); f.SkipNextWord(); f.SkipNextWord(); // Saltamos el índice de vértice
      
      // OJO! MAX da la vuelta... hay que cambiar Y por -Z
      f.ReadFloat(&verts[j].x); f.ReadFloat(&verts[j].z); f.ReadFloat(&verts[j].y); 
      verts[j].z = -verts[j].z;
    }

    // Buscamos primero la referencia del material...
    s32 mtlRef;
    
    f.FindNext(JASETAG_MATERIAL_REF);f.SkipNextWord();
    f.ReadInteger(&mtlRef);
    
    // ...Buscamos el material correspondiente a esa referencia...
    s32 mtlIndex = 0;
    while (mtlIndex < numMaterials && materials[mtlIndex].id != mtlRef)
    {
      // Avanzamos el nº de submateriales que indique este material y vamos al siguiente
      if (materials[mtlIndex].isMulti)
        mtlIndex += materials[mtlIndex].numSubMaterials;
      else
        ++mtlIndex;
    }
    
    // Si no lo encuentra, salimos
    if (materials[mtlIndex].id != mtlRef)
      return false;

    // Si lo encontró, volvemos al punto de inicio
    f.SetPos(oldPos);

    // Asignamos el material a la cara (queda pendiente ponerle textura) ...
    s32 vInd;
    
    if (materials[mtlIndex].isMulti)
    {
      // Lista de caras
      s32 m;
      
      f.FindNext(JASETAG_MESH_FACE_LIST); f.SkipNextWord();
      for (s32 k = 0; k < objs[i].numFaces; ++k)
      {
        // A:
        f.FindNext(JASETAG_MESH_A); f.SkipNextWord();
        f.ReadInteger(&faces[k].v1);
        // B:
        f.FindNext(JASETAG_MESH_B); f.SkipNextWord();
        f.ReadInteger(&faces[k].v2);
        // C:
        f.FindNext(JASETAG_MESH_C); f.SkipNextWord();
        f.ReadInteger(&faces[k].v3);

        // MTLID
        f.FindNext(JASETAG_MESH_MTLID); f.SkipNextWord();
        f.ReadInteger(&m);

        // Si es Multi/subobjeto debemos atender al MTLID de las caras...
        // v. nota al comienzo del .h
        faces[k].material = &materials[mtlIndex + 1 + (m%materials[mtlIndex].numSubMaterials)];
      }
    }
    else
    {
      // MATERIAL_REF

      // Lista de caras
      f.FindNext(JASETAG_MESH_FACE_LIST); f.SkipNextWord();
      for (s32 k = 0; k < objs[i].numFaces; ++k)
      {
        f.FindNext(JASETAG_MESH_FACE);
        // A:
        f.FindNext(JASETAG_MESH_A); f.SkipNextWord();
        f.ReadInteger(&faces[k].v1);
        // B:
        f.FindNext(JASETAG_MESH_B); f.SkipNextWord();
        f.ReadInteger(&faces[k].v2);
        // C:
        f.FindNext(JASETAG_MESH_C); f.SkipNextWord();
        f.ReadInteger(&faces[k].v3);

        // Al no tener un material Multi asignado, todas las caras tienen el 
        // material indicado en MATERIAL_REF
        faces[k].material = &materials[mtlIndex];
      }
    }

    // Texturas.

    // El número de vértices de textura puede ser (y, en general, será)
    // diferente al número de vértices totales
    f.FindNext(JASETAG_MESH_NUMTVERTEX); f.SkipNextWord();
    f.ReadInteger(&objs[i].numTVertices);

    // Creamos el array de vértices de textura y lo rellenamos
    objs[i].tVertices = new JTextureVertex[objs[i].numTVertices];
    
    if (f.FindNext(JASETAG_MESH_TVERTLIST))
    {
      f.SkipNextWord();
      for (s32 b = 0; b < objs[i].numTVertices; ++b)
      {
        f.FindNext(JASETAG_MESH_TVERT); f.SkipNextWord(); f.SkipNextWord();
        
        // OJO: Comprobar que x e y corresponden a las mismas U, V en OpenGL
        f.ReadFloat(&objs[i].tVertices[b].u);
        f.ReadFloat(&objs[i].tVertices[b].v);
      }
    }

    // El número de caras de textura puede es
    // igual al número de caras totales
    if (f.FindNext(JASETAG_MESH_NUMTVFACES))
    {
      f.SkipNextWord();
      f.ReadInteger(&objs[i].numTFaces);

      // Creamos el array de caras de textura y lo rellenamos
      if (objs[i].numTFaces > 0)
      {
        objs[i].tFaces = new JTextureFace[objs[i].numTFaces];
        
        f.FindNext(JASETAG_MESH_TFACELIST); f.SkipNextWord();
        for (s32 c = 0; c < objs[i].numTFaces; ++c)
        {
          f.FindNext(JASETAG_MESH_TFACE); f.SkipNextWord(); f.SkipNextWord();
          f.ReadInteger(&objs[i].tFaces[c].v1);
          f.ReadInteger(&objs[i].tFaces[c].v2);
          f.ReadInteger(&objs[i].tFaces[c].v3);
        }
      }
    }
    
    // Calcula las normales a las caras
    objs[i].vertexNormals = new JVertex[objs[i].numVertices];
    memset(objs[i].vertexNormals, 0, sizeof(JVertex) * objs[i].numVertices);
    
    JVertex *vNorm = objs[i].vertexNormals;
    JVector v1, v2, v3, vt1, vt2, vn;
    
    for (s32 d = 0; d < objs[i].numFaces; ++d)
    {
      if (d == 35)
        d = d;

      v1 = JVector(objs[i].vertices[faces[d].v1].x,
                   objs[i].vertices[faces[d].v1].y,
                   objs[i].vertices[faces[d].v1].z);
      v2 = JVector(objs[i].vertices[faces[d].v2].x,
                   objs[i].vertices[faces[d].v2].y,
                   objs[i].vertices[faces[d].v2].z);
      v3 = JVector(objs[i].vertices[faces[d].v3].x,
                   objs[i].vertices[faces[d].v3].y,
                   objs[i].vertices[faces[d].v3].z);
      
      vt1 = (v3 - v1);
      vt2 = (v3 - v2);
      vn = vt1.Cross(vt2);
      vn.Normalize();
      faces[d].normal.x = vn.x;
      faces[d].normal.y = vn.y;
      faces[d].normal.z = vn.z;
    }

    // Asigna las normales a cada vértice
    s32 *shared = new s32[objs[i].numVertices];
    memset(shared, 0, sizeof(s32) * objs[i].numVertices);
    
    for (s32 k = 0; k < objs[i].numFaces; ++k)
    {
      if (i == 219 && k == 68)
        i = i;
      
      // Si algún día quieres quedarte con las normales del ASE...
      /*
      f.FindNext(JASETAG_MESH_FACENORMAL); f.SkipNextWord(); f.SkipNextWord();
      f.ReadFloat(&faces[k].normal.x);
      f.ReadFloat(&faces[k].normal.z);        // Por el cambio de MAX y -> -z
      f.ReadFloat(&faces[k].normal.y);
      faces[k].normal.z = -faces[k].normal.z;
      */

      // Suma las normales dadas a cada vértice de la cara. Resultado:
      // al final todas las normales estarán promediadas en cada vértice.
      f.FindNext(JASETAG_MESH_VERTEXNORMAL); f.SkipNextWord();
      f.ReadInteger(&vInd);
      vNorm[vInd].x += faces[k].normal.x;
      vNorm[vInd].y += faces[k].normal.y;
      vNorm[vInd].z += faces[k].normal.z;
      ++shared[vInd];

      f.FindNext(JASETAG_MESH_VERTEXNORMAL); f.SkipNextWord();
      f.ReadInteger(&vInd);
      vNorm[vInd].x += faces[k].normal.x;
      vNorm[vInd].y += faces[k].normal.y;
      vNorm[vInd].z += faces[k].normal.z;
      ++shared[vInd];

      f.FindNext(JASETAG_MESH_VERTEXNORMAL); f.SkipNextWord();
      f.ReadInteger(&vInd);
      vNorm[vInd].x += faces[k].normal.x;
      vNorm[vInd].y += faces[k].normal.y;
      vNorm[vInd].z += faces[k].normal.z;
      ++shared[vInd];
    }

    // Promedia y normaliza las normales sumadas para cada vértice
    for (s32 h = 0; h < objs[i].numVertices; ++h)
    {
      vn = JVector(objs[i].vertexNormals[h].x,
                   objs[i].vertexNormals[h].y,
                   objs[i].vertexNormals[h].z);
      vn /= JScalar(shared[h]);
      vn.Normalize();
      objs[i].vertexNormals[h].x = vn.x;
      objs[i].vertexNormals[h].y = vn.y;
      objs[i].vertexNormals[h].z = vn.z;
    }

    delete[] shared;
  }

  return true;
}

