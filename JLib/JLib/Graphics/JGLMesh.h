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

/** Definiciones para geometría 3D para OpenGL.
 * @file    JGLMesh.h
 * @author  Juan Carlos Seijo Pérez
 * @date    10/05/2004
 * @version 0.0.1 - 10/05/2004 - Primera versión.
 * @notes: Ficheros ASE - Curiosidad de MAX 5, pongo el ejemplo:
 * Supongamos una escena con tres bolas que comparten el mismo material multi/subobjeto
 * compuesto de tres submateriales de id 1, 2 y 3, respectivamente. Si las bolas
 * tienen id's 1, 2 y 3 respectivamente se asignarán de esa forma los submateriales
 * a cada una de ellas. Sin embargo si alguna de ellas tiene un valor diferente (mayor),
 * el material que aplica MAX es el resto de dividir el id de la bola menos uno entre el número
 * de submateriales del material y todo eso más uno. 
 * Así si las bolas tienen id's 1, 22 y 7, respectivamente,
 * - a la bola con MTLID 1 se le asigna el submaterial: ((1-1)%3) + 1 = (0%3) + 1 = 0 + 1 = 1
 * - a la bola con MTLID 23 se le asigna el submaterial: ((23-1)%3) + 1 = (21%3) + 1 = 0 + 1 = 1
 * - a la bola con MTLID 7 se le asigna el submaterial: ((8-1)%3) + 1 = (7%3) + 1 = 1 + 1 = 2
 * 
 * Notad que en el fichero ASE el MTLID es uno menos que en la escena ya.
 */

#ifndef _JGLMESH_INCLUDED
#define _JGLMESH_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JString.h>
#include <JLib/Util/JTextFile.h>
#include <JLib/Math/JVector.h>
#include <JLib/Graphics/JDrawable.h>
#include <JLib/Graphics/JASEFormat.h>
#include <JLib/Graphics/JGLTexture.h>

/** Color RGB. Todas las componentes van de 0.0f a 1.0f.
 */
struct JColor
{
  float r;                              /**< Componente roja. */
  float g;                              /**< Componente verde. */
  float b;                              /**< Componente azul. */
  float a;                              /**< Componente alfa. */
};

class JMaterial
{
 public:
	/** Crea un material vacío.
	 */
	JMaterial(): texture(0)
	{}

  JGLTexture *texture;                  /**< Textura asociada. */
  s32 parentId;                         /**< Id del padre. */
  s32 id;                               /**< Id. */
  bool isSubMaterial;                   /**< Indica si es submaterial. */
  bool isMulti;                         /**< Indica si es Multi/sub-objeto. */
  s32 numSubMaterials;                  /**< Númeor de submateriales. */
  s8 mtlClass[32];                      /**< Tipo de material. */
  JColor ambient;                       /**< Color ambiente. */
  JColor diffuse;                       /**< Color difuso. */
  JColor specular;                      /**< Color especular. */
  float shine;                          /**< Cantidad de brillo. */
  float shineStrenght;                  /**< Intensidad de brillo. */
  float transparency;                   /**< Transparencia. */
  float selfIllum;                      /**< Iluminación propia. */

	/** Devuelve la textura asociada a este material.
	 * @return La textura asociada a este material.
	 */
	JGLTexture * Texture()
	{
		return texture;
	}

	/** Libera los recursos asociados.
	 */
	void Destroy();

	/** Destruye el material y libera los recursos asociados.
	 */
	~JMaterial() {Destroy();}
};

typedef JVector JVertex;

/** Coordenada de textura.
 */
struct JTextureVertex
{
  float u;                              /**< Componente u. */
  float v;                              /**< Componente v. */
};

/** Cara.
 */
class JFace
{
 public:
  /** Crea el objeto. 
	 */
  JFace() : material(0) {}
  s32 v1;                               /**< Vértice 1. */
  s32 v2;                               /**< Vértice 2. */
  s32 v3;                               /**< Vértice 3. */
  JMaterial *material;                  /**< material asociado. */
  JVector normal;                       /**< Vector normal. */
};

/** Cara con textura.
 */
struct JTextureFace
{
  s32 v1;                               /**< Vértice 1. */
  s32 v2;                               /**< Vértice 2. */
  s32 v3;                               /**< Vértice 3. */
};

/** Clase de objeto 3D para OpenGL. Un objeto se compone de
 * vértices agrupados en caras, posiblemente texturizadas.
 * Nótese que los materiales se asignan a nivel de geometría.
 */
class JGLMeshObject
{
 public:
  JString name;                         /**< Nombre. */
  s32 numVertices;                      /**< Número de vértices. */
  s32 numTVertices;                     /**< Número de vértices de textura. */
  s32 numFaces;                         /**< Número de caras. */
  s32 numTFaces;                        /**< Número de caras de textura. */
  JVertex *vertices;                    /**< Vértices. */
  JTextureVertex *tVertices;            /**< Vértices de textura. */
  JVector *vertexNormals;               /**< Normales a los vértices. */
  JFace *faces;                         /**< Caras. */
  JTextureFace *tFaces;                 /**< Caras de textura. */

  /** Crea el objeto vacío. 
	 */
  JGLMeshObject() : vertices(0), tVertices(0), vertexNormals(0), faces(0), tFaces(0)
	{}

	/** Devuelve el nombre de esta geometría.
	 * @return nombre de esta geometría.
	 */
	const JString & Name()
	{
		return name;
	}

	/** Establece el nombre de esta geometría.
	 * @param  newName nombre de esta geometría.
	 */
	void Name(const JString & newName)
	{
		name = newName;
	}

	/** Devuelve el número de vértices de la geometría.
	 * @return Número de vértices de la geometría.
	 */
	s32 NumVertices()
	{
		return numVertices;
	}

	/** Devuelve el número de vértices de textura de la geometría.
	 * @return El número de vértices de textura de la geometría.
	 */
	s32 NumTVertices()
	{
		return numTVertices;
	}

	/** Devuelve el número de caras de la geometría.
	 * @return El número de caras de la geometría.
	 */
	s32 NumFaces()
	{
		return numFaces;
	}

	/** Devuelve el número de caras de textura de la geometría.
	 * @return El número de caras de textura de la geometría.
	 */
	s32 NumTFaces()
	{
		return numTFaces;
	}

	/** Devuelve los vértices de la geometría.
	 * @return Vértices de la geometría.
	 */
	JVertex * Vertices()
	{
		return vertices;
	}

	/** Devuelve los vértices de textura de la geometría.
	 * @return Vértices De Textura de la geometría.
	 */
	JTextureVertex * TVertices()
	{
		return tVertices;
	}

	/** Devuelve las normales a los vértices de la geometría.
	 * @return Las normales a los vértices de la geometría.
	 */
	JVector * VertexNormals()
	{
		return vertexNormals;
	}

	/** Devuelve las caras de la geometría.
	 * @return Las caras de la geometría.
	 */
	JFace * Faces()
	{
		return faces;
	}

	/** Devuelve las caras de textura de la geometría.
	 * @return Las caras de textura de la geometría.
	 */
	JTextureFace * TFaces()
	{
		return tFaces;
	}

	/** Libera los recursos asociados a este objeto.
	 */
	void Destroy();

	/** Destruye el objeto.
	 */
	virtual ~JGLMeshObject() {Destroy();}
};

/** Clase de geometría 3D para OpenGL. Consiste de una serie de objetos 3D
 * y sus correspondientes materiales.
 */
class JGLMesh : public JDrawable
{
 protected:
  s32 numMaterials;                     /**< Número de materiales. */
  s32 numObjects;                       /**< Número de objetos. */
  JMaterial *materials;                 /**< Materiales. */
  JGLMeshObject *objects;               /**< Objetos 3D de esta geometría. */
	
	JTextFile f;                          /**< Fichero de carga de objetos. */

	/** Carga los materiales de la geometría desde el fichero ASE abierto por LoadASE.
	 * @return <b>true</b> si se pudieron cargar, <b>false</b> si no.
	 */
	bool LoadASEMaterials();

	/** Carga los objetos de la geometría desde el fichero ASE abierto por LoadASE.
	 * @return <b>true</b> si se pudieron cargar, <b>false</b> si no.
	 */
	bool LoadASEObjects();

 public:
	/** Construye la geometría vacía, lista para cargarse con Load().
	 */
	JGLMesh() : materials(0), objects(0)
	{}

	/** Devuelve el número de materiales de la geometría.
	 * @return El número de materiales de la geometría.
	 */
	s32 NumMaterials()
	{
		return numMaterials;
	}

	/** Devuelve el número de objetos de esta geometría.
	 * @return El número de objetos de esta geometría.
	 */
	s32 NumObjects()
	{
		return numObjects;
	}

	/** Devuelve los materiales de la geometría.
	 * @return Los materiales de la geometría.
	 */
	JMaterial * Materials()
	{
		return materials;
	}

	/** Devuelve los objetos de esta geometría.
	 * @return los objetos de esta geometría.
	 */
	JGLMeshObject * Objects()
	{
		return objects;
	}

	/** Dibuja la geometría.
	 */
	void Draw();

	/** Carga la geometría desde el fichero dado.
	 * Por ahora el fichero puede ser formato ASE únicamente.
	 * @return <b>true</b> si se pudo cargar, <b>false</b> si no.
	 */
	bool LoadASE(const JString &filename);

	/** Carga la geometría desde el fichero J3D dado. Los ficheros J3D se generan a partir de Blender (www.blender.org)
   * con ayuda del script de exportación 'JExporter'.
	 * @return <b>true</b> si se pudo cargar, <b>false</b> si no.
	 */
	bool LoadJ3D(const JString &filename);

	/** Libera los recursos asociados a este objeto.
	 */
	void Destroy();

	/** Destruye el objeto.
	 */
	virtual ~JGLMesh() {Destroy();}
};

#endif // _JGLMESH_INCLUDED
