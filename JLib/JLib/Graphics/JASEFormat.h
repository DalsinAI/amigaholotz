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

/** Etiquetas de ficheros ASE
 * @file    JASEFormat.h
 * @author  Juan Carlos Seijo Pérez
 * @date    20/09/2003
 * @version 0.0.1 - 20/09/2003 - Primera versión.
 */

#ifndef _JASEFORMAT_INCLUDED
#define _JASEFORMAT_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JObject.h>

// Tags del formato ASE. Son las cadenas a utilizar por scanf más adelante.

/** Cabecera de fichero. */
#define JASETAG_FILE_HEADER               "*3DSMAX_ASCIIEXPORT"

/** La versión para la que se hace, no cuenta para validar el ase. */
#define JASETAG_FILE_VERSION              "200"
                                                                
/** Escena. */
#define JASETAG_SCENE                     "*SCENE"
  /** Ordinal del primer cuadro. */
  #define JASETAG_SCENE_FIRSTFRAME        "*SCENE_FIRSTFRAME"
  /** Ordinal del último cuadro. */
  #define JASETAG_SCENE_LASTFRAME         "*SCENE_LASTFRAME"
/** Lista de materiales. */
#define JASETAG_MATERIAL_LIST             "*MATERIAL_LIST"
  /** Número de materiales. */
  #define JASETAG_MATERIAL_COUNT          "*MATERIAL_COUNT"
  /** Material. */
  #define JASETAG_MATERIAL                "*MATERIAL"
  /** Núemro de submateriales. */
  #define JASETAG_SUBMATERIAL_COUNT       "*NUMSUBMTLS"
  /** Submaterial. */
  #define JASETAG_SUBMATERIAL             "*SUBMATERIAL"
  /** Tipo de material. */
  #define JASETAG_MATERIAL_CLASS          "*MATERIAL_CLASS"
    /** Material Multi/sub-objeto. */
    #define JASEVAL_MULTI_SUBOBJECT       "\"Multi/Sub-Object\""
    /** Material estándar. */
    #define JASEVAL_STANDARD              "\"Standard\""
  /** Color ambiente del material. */
  #define JASETAG_MATERIAL_AMBIENT        "*MATERIAL_AMBIENT"
  /** Color difuso del material. */
  #define JASETAG_MATERIAL_DIFFUSE        "*MATERIAL_DIFFUSE"
  /** Color especular del material. */
  #define JASETAG_MATERIAL_SPECULAR       "*MATERIAL_SPECULAR"
  /** Flag de brillo del material. */
  #define JASETAG_MATERIAL_SHINE          "*MATERIAL_SHINE"
  /** Intensidad de brillo del material. */
  #define JASETAG_MATERIAL_SHINESTRENGTH  "*MATERIAL_SHINESTRENGTH"
  /** Transparencia del material. */
  #define JASETAG_MATERIAL_TRANSPARENCY   "*MATERIAL_TRANSPARENCY"
  /** Iluminación propia. */
  #define JASETAG_MATERIAL_SELFILLUM      "*MATERIAL_SELFILLUM"
  /** Tipo XP. */
  #define JASETAG_MATERIAL_XP_TYPE        "*MATERIAL_XP_TYPE"
  /** Mapa difuso del material. */
  #define JASETAG_MATERIAL_MAP_DIFFUSE    "*MAP_DIFFUSE"
    /** Subnúmero del mapa del material. */
    #define JASETAG_MATERIAL_MAP_SUBNO    "*MAP_SUBNO"
    /** Textura del material. */
    #define JASETAG_MATERIAL_BITMAP       "*BITMAP"
/** Objeto. */
#define JASETAG_GEOMOBJECT                "*GEOMOBJECT"
/** Nombre. */
#define JASETAG_NODE_NAME                 "*NODE_NAME"
/** Geometría. */
#define JASETAG_MESH                      "*MESH"
  /** Tiempo dentro de la animación. */
  #define JASETAG_TIMEVALUE               "*TIMEVALUE"
  /** Númerod evértices. */
  #define JASETAG_MESH_NUMVERTEX          "*MESH_NUMVERTEX"
  /** Número de caras. */
  #define JASETAG_MESH_NUMFACES           "*MESH_NUMFACES"
  /** Lista de vértices. */
  #define JASETAG_MESH_VERTEX_LIST        "*MESH_VERTEX_LIST"
    /** Vértice. */
    #define JASETAG_MESH_VERTEX           "*MESH_VERTEX"
  /** Lista de caras. */
  #define JASETAG_MESH_FACE_LIST          "*MESH_FACE_LIST"
    /** Cara. */
    #define JASETAG_MESH_FACE             "*MESH_FACE"
      /** Primer vértice. */
      #define JASETAG_MESH_A              "A:"
      /** Segundo vértice. */
      #define JASETAG_MESH_B              "B:"
      /** Tercer vértice. */
      #define JASETAG_MESH_C              "C:"
      /** ID del material asociado. */
      #define JASETAG_MESH_MTLID          "*MESH_MTLID"
  /** Núemro de vértices con textura. */
  #define JASETAG_MESH_NUMTVERTEX         "*MESH_NUMTVERTEX"
  /** Lista de vértices con textura. */
  #define JASETAG_MESH_TVERTLIST          "*MESH_TVERTLIST"
    /** Vértice con textura. */
    #define JASETAG_MESH_TVERT            "*MESH_TVERT"
  /** Número de caras con textura. */
  #define JASETAG_MESH_NUMTVFACES         "*MESH_NUMTVFACES"
  /** Lista de caras con textura. */
  #define JASETAG_MESH_TFACELIST          "*MESH_TFACELIST"
    /** Cara con textura. */
    #define JASETAG_MESH_TFACE            "*MESH_TFACE"
  /** Lista de normales. */
  #define JASETAG_MESH_NORMALS            "*MESH_NORMALS"
    /** Normales de cara. */
    #define JASETAG_MESH_FACENORMAL       "*MESH_FACENORMAL"
      /** Normales de vértice. */
      #define JASETAG_MESH_VERTEXNORMAL   "*MESH_VERTEXNORMAL"
  /** Núemro de material. */
  #define JASETAG_MATERIAL_REF            "*MATERIAL_REF"

#endif  // _JASEFORMAT_INCLUDED
