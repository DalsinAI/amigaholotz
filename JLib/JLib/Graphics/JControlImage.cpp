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

/** Control de interfaz de usuario compuesto por imágenes.
 * @file    JControlImage.cpp
 * @author  Juan Carlos Seijo Pérez.
 * @date    27/10/2003.
 * @version 0.0.1 - 27/10/2003 - Primera versión.
 */

#include <JLib/Graphics/JControlImage.h>

// Destructor
void JControlImage::Destroy()
{
	imgNormal.Destroy();
	imgDisabled.Destroy();
	imgFocused.Destroy();
	imgSelected.Destroy();
}

bool JControlImage::Init(JImage &normal, JImage &disabled, JImage &focused, JImage &selected)
{
	imgNormal.Ref(normal);
	imgDisabled.Ref(disabled);
	imgFocused.Ref(focused);
	imgSelected.Ref(selected);
  
  return true;
}

// Función de dibujo cuando está visible
void JControlImage::DrawVisible()
{
  imgNormal.Draw();
}

// Función de dibujo cuando está enfocado
void JControlImage::DrawFocus()
{
  imgFocused.Draw();
}

// Función de dibujo cuando está deshabilitado
void JControlImage::DrawDisabled()
{
  imgDisabled.Draw();
}

// Función de dibujo cuando está seleccionado
void JControlImage::DrawSelected()
{
  imgSelected.Draw();
}

// Función de actualización cuando está visible
s32 JControlImage::UpdateVisible()
{
  return state;
}

// Función de actualización cuando está enfocado
s32 JControlImage::UpdateFocus()
{
  return state;
}

// Función de actualización cuando está deshabilitado
s32 JControlImage::UpdateDisabled()
{
  return state;
}

// Función de actualización cuando está seleccionado
s32 JControlImage::UpdateSelected()
{
  return state;
}

u32 JControlImage::Load(JRW &f)
{
	// Carga la parte común del control (id e id del padre)
	if (0 != JControl::Load(f))
	{
		return 2;
	}

	// Carga las imágenes de control
	if (0 != imgNormal.Load(f) ||
			0 != imgFocused.Load(f) ||
			0 != imgSelected.Load(f) ||
			0 != imgDisabled.Load(f))
	{
		return 2;
	}

	return 0;
}

u32 JControlImage::Save(JRW &f)
{
	// Salva la parte común del control (id e id del padre)
	if (0 != JControl::Save(f))
	{
		return 2;
	}

	// Salva las imágenes de control
	if (0 != imgNormal.Save(f) ||
			0 != imgFocused.Save(f) ||
			0 != imgSelected.Save(f) ||
			0 != imgDisabled.Save(f))
	{
		return 2;
	}

	return 0;
}

