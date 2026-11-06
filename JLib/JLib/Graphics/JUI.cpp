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

/** Clase de gestión de una interfaz de usuario.
 * @file    JUI.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    13/06/2004
 * @version 0.0.1 - 13/06/2004 - Primera versión.
 */

#include <JLib/Graphics/JUI.h>

bool JUI::Init()
{
	return true;
}

void JUI::Draw()
{
	for (int i = 0; i < numControls; ++i)
	{
		controls[i]->Draw();
	}
}

s32 JUI::Update()
{
	for (int i = 0; i < numControls; ++i)
	{
		controls[i]->Update();
	}

	return 0;
}

s32 JUI::TrackMouse(s32 x, s32 y, s32 bt)
{
	return 0;
}

s32 JUI::TrackKeyboard(s32 x, s32 y, s32 bt)
{
	return 0;
}

u32 JUI::Load(JRW &f)
{
	Destroy();

	// Carga el número de controles de la interfaz
	if (0 == f.ReadLE32(&numControls))
	{
		return 1;
	}
	
	// Crea el array de controles de la interfaz
	controls = new JControl *[numControls];

	// Carga los controles de la interfaz
	u32 t = 0;
	for (int i = 0; i < numControls; ++i)
	{
		if (0 == f.Read(&t, sizeof(t)))
		{
			Destroy();
			return 2;
		}

		switch (t)
		{
		case JCONTROLTYPE_IMAGE:
			controls[i] = new JControlImage;
			break;

		case JCONTROLTYPE_IMAGESPRITE:
			break;

		case JCONTROLTYPE_MESH:
			break;

		case JCONTROLTYPE_MESHSPRITE:
			break;

		default:
			fprintf(stderr, "Control de interfaz de usuario no reconocido.\n");
			Destroy();
			return 2;
		}

		if (0 != controls[i]->Load(f))
		{
			return 2;
		}
	}

	return 0;
}

u32 JUI::Save(JRW &f)
{
	// Salva el número de controles de la interfaz
	if (0 == f.WriteLE32(&numControls))
	{
		return 1;
	}

	// Salva los controles de la interfaz
	for (int i = 0; i < numControls; ++i)
	{
		if (0 != controls[i]->Save(f))
		{
			return 2;
		}
	}

	return 0;
}

void JUI::Destroy()
{
	if (controls != 0)
	{
		for (int i = 0; i < numControls; ++i)
		{
			JDELETE(controls[i]);
		}
	}

	JDELETE_ARRAY(controls);
}
