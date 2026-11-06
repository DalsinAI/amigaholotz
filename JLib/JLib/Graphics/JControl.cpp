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

/** Clase base de un control de interfaz de usuario.
 * @file    JControl.cpp.
 * @author  Juan Carlos Seijo Pérez.
 * @date    27/10/2003.
 * @version 0.0.1 - 27/10/2003 - Primera versión.
 */

#include <JLib/Graphics/JControl.h>

u32 JControl::controlCount;

const u32 JControl::VISIBLE  = 0x00000001;
const u32 JControl::FOCUSED  = 0x00000002;
const u32 JControl::ENABLED  = 0x00000004;
const u32 JControl::SELECTED = 0x00000008;

// Constructor
JControl::JControl(JControl *_parent, 
									 u32 _state) : parent(_parent), focusIndex(++controlCount), state(_state), 
																 focus(0), unfocus(0), enable(0), disable(0), appear(0), disappear(0),
																 select(0), unselect(0), focusData(0), unfocusData(0), 
																 enableData(0), disableData(0), appearData(0), disappearData(0), 
																 selectData(0), unselectData(0)
																 
{
	id = controlCount;
}

// Destructor
JControl::~JControl()
{
  --controlCount;
}

// Actualiza el control. Devuelve el estado del control.
void JControl::Draw()
{
  if (Visible())
  {
    if (Enabled())
    {
      if (Selected())
      {
        DrawSelected();
      }
      else if (Focused())
      {
        DrawFocus();
      }
      else
      {
        DrawVisible();
      }
    }
    else
    {
      DrawDisabled();
    }
  }
}

// Actualiza el control. Devuelve el estado del control.
s32 JControl::Update()
{
  if (Visible())
  {
    if (Enabled())
    {
      if (Selected())
      {
        return UpdateSelected();
      }
      else if (Focused())
      {
        return UpdateFocus();
      }
      else
      {
        return UpdateVisible();
      }
    }
    else
    {
      return UpdateDisabled();
    }
  }
  
  return state;
}

// Muestra el control
void JControl:: Appear()
{
  if (!Visible())
  {
    state &= VISIBLE;

    OnAppear();
  }
}

// Oculta el control
void JControl:: Disappear()
{
  if (Visible())
  {
    state &= ~VISIBLE;

    OnDisappear();
  }
}

// Habilita el control
void JControl:: Enable()
{
  if (!Enabled())
  {
    state &= ENABLED;

    OnEnable();
  }
}

// Deshabilita el control
void JControl:: Disable()
{
  if (Enabled())
  {
    state &= ~ENABLED;

    OnDisable();
  }
}

// Enfoca el control
void JControl:: Focus()
{
  if (!Focused())
  {
    state &= FOCUSED;

    OnFocus();
  }
}

// Desenfoca el control
void JControl:: Unfocus()
{
  if (Focused())
  {
    state &= ~FOCUSED;

    OnUnfocus();
  }
}

// Selecciona el control
void JControl:: Select()
{
  if (!Selected())
  {
    state &= SELECTED;

    OnSelect();
  }
}

// Deselecciona el control
void JControl:: Unselect()
{
  if (Selected())
  {
    state &= ~SELECTED;

    OnUnselect();
  }
}

u32 JControl::Load(JRW &f)
{
	// Carga el índice de control y del control padre
	if (0 == f.ReadLE32(&id) ||
			0 == f.ReadLE32(&parentId))
	{
		return 1;
	}

	return 0;
}

u32 JControl::Save(JRW &f)
{
	// Carga el índice de control y del control padre
	if (0 == f.WriteLE32(&id) ||
			0 == f.WriteLE32(&parentId))
	{
		return 1;
	}

	return 0;
}

