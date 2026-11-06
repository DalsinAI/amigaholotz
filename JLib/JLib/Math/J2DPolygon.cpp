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
 * @file    J2DPolygon.cpp.
 * @author  Juan Carlos Seijo Pérez.
 * @date    01/02/2004 
 * @version 0.0.1 - 01/02/2004 - Primera versión.
 */

#include <JLib/Math/J2DPolygon.h>
#include <stdio.h>

void J2DPolygon::Init(s32 _numVertices, J2DPoint *_vertices)
{
	Destroy();

	numVertices = _numVertices;
	vertices = new J2DPoint[numVertices];
	memcpy(vertices, _vertices, numVertices * sizeof(J2DPoint));

	segments = new J2DVector[numVertices];
	normals = new J2DVector[numVertices];
	
	// El vector normal en sentido CCW de (x, y) es (y, -x)
	for (s32 i = 0; i < numVertices - 1; ++i)
	{
		segments[i] = _vertices[i] - _vertices[i + 1];
		normals[i].Pos(segments[i].y, -segments[i].x);
	}

	segments[numVertices - 1] = _vertices[numVertices - 1] - _vertices[0];
	normals[numVertices - 1].Pos(segments[numVertices - 1].y, -segments[numVertices - 1].x);
}

void J2DPolygon::Destroy()
{
	JDELETE_ARRAY(vertices);
	JDELETE_ARRAY(normals);
	JDELETE_ARRAY(segments);
}
	
bool J2DPolygon::IsInside(J2DScalar x, J2DScalar y)
{
	J2DVector v(x, y);
	J2DVector vNorm, vTemp;
	J2DScalar s = 1;

	// Dejamos como caso más probable que esté fuera (en sentido CCW)
	for (s32 i = 0; s >= 0 && i < numVertices; ++i)
	{
		vNorm = normals[i];
		vTemp = vertices[i] - v;
		//printf("vNorm (%f, %f). ", vNorm.X(), vNorm.Y());
		//printf("vTemp (%f, %f). ", vTemp.X(), vTemp.Y());
		s = (vNorm).Dot(vTemp);
		//printf("Dot (%f).\n", s);
	}

	return s >= 0;
}

u32 J2DPolygon::Load(JFile &f)
{
	// Salva el número de vértices
	if (0 == f.Read(&numVertices, sizeof(numVertices)))
	{
		return 1;
	}

	// Salva los vértices
	u32 ret = 0;
	J2DPoint *v = new J2DPoint[numVertices];

	for (s32 i = 0; 0 == ret && i < numVertices; ++i)
	{
		ret = v[i].Load(f);
	}

	if (ret == 0)
	{
		// Inicializa el polígono
		Init(numVertices, v);
	}

	JDELETE_ARRAY(v);
	
	return ret;
}

u32 J2DPolygon::Save(JFile &f)
{
	// Salva el número de vértices
	if (0 == f.Write(&numVertices, sizeof(numVertices)))
	{
		return 1;
	}

	// Salva los vértices
	u32 ret = 0;

	for (s32 i = 0; 0 == ret && i < numVertices; ++i)
	{
		ret = vertices[i].Save(f);
	}
	
	return ret;
}
