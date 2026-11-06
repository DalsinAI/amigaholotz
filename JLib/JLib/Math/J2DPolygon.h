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

/** Polígono cerrado genérico
 * @file J2DPolygon.h
 * @author Juan Carlos Seijo Pérez
 * @date 01/02/2004 
 */

#ifndef _J2DPOLYGON_INCLUDED
#define _J2DPOLYGON_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JLoadSave.h>
#include <JLib/Math/J2DVector.h>
#include <string.h>
#include <math.h>

/** Polígono cerrado genérico.
 * Consta de varios vértices conectados correlativamente.
 */
class J2DPolygon : public JLoadSave
{
protected:
	s32 numVertices;                      /**< Número de vértices del polígono */
	J2DPoint* vertices;                   /**< Vértices del polígono */
	J2DVector* segments;                  /**< Segmentos del polígono */
	J2DVector* normals;                   /**< Normales a los segmentos del polígono */

public:
	/** Constructor.
	 * Crea un polígono vacío.
	 */
	J2DPolygon() : numVertices(0), vertices(0), segments(0), normals(0) {}
	
	
	/** Constructor.
	 * Crea un polígono cerrado a partir de los vértices dados.
	 * @param _numVertices Número de vértices.
	 * @param _vertices Vértices.
	 */
	J2DPolygon(s32 _numVertices, J2DPoint *_vertices) : numVertices(0), vertices(0), segments(0), normals(0)
	{Init(_numVertices, _vertices);}

	/** Inicializa el polígono con los vértices dados.
	 * @param _numVertices Número de vértices.
	 * @param _vertices Vértices.
	 */
	void Init(s32 _numVertices, J2DPoint *_vertices);
	
	/** Destructor.
	 * Libera memoria.
	 */
	virtual ~J2DPolygon() {Destroy();}
	
	/** Destruye el objeto y lo deja como si se acabará de crear con J2DPolygon.
	 */
	void Destroy();

	/** Determina si el punto dado está dentro del polígono.
	 * Por convenio, si un polígono está definido en 
	 * sentido antihorario (CCW) entonces un punto está dentro si cae dentro del 
	 * contorno cerrado del polígono. Si se define en sentido horario (CW), los puntos 
	 * de dentro del contorno cerrado del polígono están fuera.
	 * @param x coordenada x del punto.
	 * @param y coordenada y del punto.
	 * @return true si el punto dado está dentro, false en caso contrario.
	 */
	bool IsInside(J2DScalar x, J2DScalar y);

  /** Carga el polígono
	 * @param f Fichero posicionado en este recurso
   * @return 0 Si todo fue bien, 1 si hay error de E/S y 2 si hay error
   * en los datos.
	 */
	u32 Load(JFile &f);

  /** Salva el polígono
	 * @param f Fichero posicionado en este recurso
   * @return 0 Si todo fue bien, 1 si hay error de E/S y 2 si hay error
   * en los datos.
	 */
	u32 Save(JFile &f);
};

#endif // _J2DPOLYGON_INCLUDED
