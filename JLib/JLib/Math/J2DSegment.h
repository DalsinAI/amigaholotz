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

/** Segmento en el plano.
 * @file    J2DSegment.h
 * @author  Juan Carlos Seijo Pérez
 * @date    05/01/2004
 * @version 0.0.1 - 05/01/2004 - Primera versión.
 */

#ifndef _J2DSEGMENT_INCLUDED
#define _J2DSEGMENT_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JLoadSave.h>

/** Encapsula un segmento en el plano. Coordenadas medidas como 
 * en pantalla (eje y positivo hacia abajo).
 */
class J2DSegment : public JLoadSave
{
protected:
	bool vertical;                        /**< Flag de verticalidad */
	float lastX, lastY;                   /**< Último punto de intersección */
	float x1, y1, x2, y2;                 /**< Inicio y fin del segmento */
	float m;                              /**< Pendiente */
	float n;                              /**< Desplazamiento */

	/** Compara dos valores decimales con el margen 'epsilon' actual de la clase.
   * @param  a Primer valor.
   * @param  b Segundo valor.
   */
	bool NearlyEquals(float a, float b) const
	{
		return (a >= b - epsilon && a <= b + epsilon);
	}

  /** Recalcula la pendiente.
   */
	void RecalcM()
	{
		if (x1 == x2)
		{
			vertical = true;
			m = n = 0;
		}
		else
		{
			m = (y2 - y1)/(x2 - x1);
			n = y1 - (m * x1);
			vertical = false;
		}
	}

public:
	static const float epsilon;           /**< Margen de comparación entre dos puntos */

	/** Crea un nuevo segmento.
   */
	J2DSegment()
	{}

	/** Crea un nuevo segmento a partir de sus extremos.
   * @param  _x1 Coordenada x del primer extremo.
   * @param  _y1 Coordenada y del primer extremo.
   * @param  _x2 Coordenada x del segundo extremo.
   * @param  _y2 Coordenada y del segundo extremo.
   */
	J2DSegment(float _x1, float _y1, float _x2, float _y2) 
		: x1(_x1), y1(_y1), x2(_x2), y2(_y2)
	{
		RecalcM();
	}

  /** Determina si el segmento es vertical.
   * @return <b>true</b> en caso de que sea vertical, <b>false</b> si no.
   */
	bool Vertical() const {return vertical;}
	
  /** Devuelve la coordenada x del primer extremo.
   * @return Coordenada x del primer extremo.
   */
  float  X1() const {return x1;}

  /** Devuelve la coordenada y del primer extremo.
   * @return Coordenada y del primer extremo.
   */
	float  Y1() const {return y1;}

  /** Devuelve la coordenada x del segundo extremo.
   * @return Coordenada x del segundo extremo.
   */
  float  X2() const {return x2;}

  /** Devuelve la coordenada y del segundo extremo.
   * @return Coordenada y del segundo extremo.
   */
  float  Y2() const {return y2;}

  /** Devuelve la pendiente 'm' en la ecuación de la recta y = mx + n.
   * @return Pendiente.
   */
  float M() const {return m;}

  /** Devuelve el desplazamiento vertical 'n' en la ecuación de la recta y = mx + n.
   * @return Pendiente.
   */
  float   N() const {return n;}

	/** Establece la coordenada x del primer segmento.
   * @param  _x1 Coordenada x del primer segmento.
   */
  void X1(float _x1) {x1 = _x1; RecalcM();}

  /** Establece la coordenada y del primer segmento.
   * @param  _y1 Coordenada y del primer segmento.
   */
  void Y1(float _y1) {y1 = _y1; RecalcM();}

	/** Establece la coordenada x del segundo segmento.
   * @param  _x2 Coordenada x del segundo segmento.
   */
  void X2(float _x2) {x2 = _x2; RecalcM();}

  /** Establece la coordenada y del segundo segmento.
   * @param  _y2 Coordenada y del segundo segmento.
   */
  void Y2(float _y2) {y2 = _y2; RecalcM();}
	
  /** Establece las coordenadas de los extremos.
   * @param  _x1 Coordenada x del primer segmento.
   * @param  _y1 Coordenada y del primer segmento.
   * @param  _x2 Coordenada x del segundo segmento.
   * @param  _y2 Coordenada y del segundo segmento.
   */
  void Pos(float _x1, float _y1, float _x2, float _y2) 
	{X1(_x1); Y1(_y1); X2(_x2); Y2(_y2); RecalcM();}

  /** Devuelve la coordenada x del último punto de corte.
   * @return Coordenada x del último punto de corte.
   */
  float LastX() {return lastX;}

  /** Devuelve la coordenada y del último punto de corte.
   * @return Coordenada y del último punto de corte.
   */
  float LastY() {return lastY;}

	/** Determina si el valor x se encuentra entre min y max, inclusive.
   * @param  x Valor a probar.
   * @param  min Valor mínimo.
   * @param  max Valor máximo.
   * @return <b>true</b> si x está entre min y max, <b>false</b> si no.
   */
	bool Between(float x, float min, float max) const
	{
		return (min <= x && x <= max);
	}

	/** Comprueba si el punto pertenece al segmento.
   * @param  x Coordenada x del punto a probar.
   * @param  y Coordenada y del punto a probar.
   * @return <b>true</b> si lo contiene, <b>false</b> si no.
   * @see    epsilon.
   */
	bool Contains(float x, float y) const
	{
		if (vertical)
		{
			return x1 == x && (Between(y, y1, y2) || Between(y, y2, y1));
		}

		return (NearlyEquals(y, ((m * x) + n)) && 
						(Between(x, x1, x2) ||
						 Between(x, x2, x1)));
	}

  /** Comprueba si el punto está en el rectángulo formado por el segmento.
   * @param  x Coordenada x del punto a probar.
   * @param  y Coordenada y del punto a probar.
   * @return <b>true</b> si lo está, <b>false</b> si no.
   */
	bool InRect(float x, float y) const
	{
		float xMin, xMax, yMin, yMax;
		
		if (x1 < x2) 
		{
			xMin = x1; 
			xMax = x2;
		} 
		else	
		{
			xMin = x2;
			xMax = x1;
		}

		if (y1 < y2) 
		{
			yMin = y1; 
			yMax = y2;
		} 
		else	
		{
			yMin = y2;
			yMax = y1;
		}

		return (Between(x, xMin, xMax) && Between(y, yMin, yMax));
	}
	
	/** Comprueba si se cortan los segmentos. En caso de cortarse, el punto
   * de corte se obtiene con LastX() y LastY().
	 * @param  other Segmento con el que comprobar el corte.
   * @return <b>true</b> si se cortan, <b>false</b> si no.
   */
	bool Cuts(const J2DSegment &other)
	{
		bool cut;

		if (vertical)
		{
			// Paralelos
			if (other.Vertical())
			{
				//printf("Paralelos verticales\n");
				cut = (Contains(lastX = other.X1(), lastY = other.Y1()) || 
							 Contains(lastX = other.X2(), lastY = other.Y2())); 
			}
			else
			{
				// No paralelos
				//printf("No Paralelos, primero vertical\n");
				lastX = x1;
				lastY = (other.M() * x1) + other.N();
				cut = (Contains(lastX, lastY) && 
					     other.Contains(lastX, lastY)); 
//				cut = InRect(lastX, lastY) && other.InRect(lastX, lastY);
			}
		}
		else if (other.Vertical())
		{
      // No paralelos
			//printf("No Paralelos, segundo vertical\n");
			lastX = other.X1();
			lastY = (M() * other.X1()) + N();
			cut = (Contains(lastX, lastY) && 
						 other.Contains(lastX, lastY)); 
//			cut = InRect(lastX, lastY) && other.InRect(lastX, lastY);
		}
		else
		// Paralelos
		if (m == other.M())      // ¡UFFFF!, aunque vienen de enteros, no creo que haya problemas
		{
			if ((s32)n != other.N())
			{
				//printf("Paralelos no coincidentes n1=%d, n2=%d\n", (s32)n, (s32)other.N());
				cut = false;
			}
			else
			{
				//printf("Coincidentes\n");
				cut = (Contains(lastX = other.X1(), lastY = other.Y1()) || 
							 Contains(lastX = other.X2(), lastY = other.Y2()) ||
							 other.Contains(lastX = X1(), lastY = Y1()) ||
					     other.Contains(lastX = X2(), lastY = Y2())); 
//				cut = InRect(lastX, lastY) && other.InRect(lastX, lastY);
			}
		}
		else
		{	
			// No Paralelos
			//printf("No Paralelos\n");
			lastX = (other.N() - n)/(m - other.M());
			lastY = n + (m * lastX);
		
			cut = (Contains(lastX, lastY) && 
				     other.Contains(lastX, lastY));
//			cut = InRect(lastX, lastY) && other.InRect(lastX, lastY);
		}

		//if (cut)
			////printf("(%d,%d)-(%d,%d) corta a (%d,%d)-(%d,%d) en (%d, %d)\n", (s32)x1, (s32)y1, 
			//			 (s32)x2, (s32)y2, (s32)other.X1(), (s32)other.Y1(), (s32)other.X2(), (s32)other.Y2(), (s32)lastX, (s32)lastY);
		return cut;
	}

  /** Carga el segmento.
   * @param  f Fichero abierto y posicionado para cargar el objeto.
   * @return 0 si todo fue bien, 1 en caso de error de E/S, 2 en caso
   * de error de integridad.
   */
	u32 Load(JFile &f)
	{
		if (0 != f.Read(&x1, sizeof(x1)) && 
				0 != f.Read(&y1, sizeof(y1)) && 
				0 != f.Read(&x2, sizeof(x2)) && 
				0 != f.Read(&y2, sizeof(y2)))
		{
			RecalcM();
			
			return 0;
		}
		
		return 1;
	}

	/** Salva el segmento.
   * @param  f Fichero abierto y posicionado para salvar el objeto.
   * @return 0 si todo fue bien, 1 en caso de error de E/S, 2 en caso
   * de error de integridad.
   */
	u32 Save(JFile &f)
	{
		if (0 != f.Write(&x1, sizeof(x1)) && 
				0 != f.Write(&y1, sizeof(y1)) && 
				0 != f.Write(&x2, sizeof(x2)) && 
				0 != f.Write(&y2, sizeof(y2)))
		{
			return 0;
		}
		
		return 1;
	}
};

const float J2DSegment::epsilon = 0.9f;

#endif  // _J2DSEGMENT_INCLUDED
