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

/** Menu en pantalla compuesto de imagénes.
 * @file    JImageMenu.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    28/04/2004
 * @version 0.0.1 - 28/04/2004 - Primera versión.
 * @version 0.0.2 - 25/09/2004 - Modificación del método 2D de Pos() para aceptar floats (quita libertad sino).
 */

#include <JLib/Graphics/JImageMenu.h>

bool JImageMenuEntry::Select()
{
	if (Action)
	{
		Action(data);
		return true;
	}
	
	return false;
}

JImageMenu::JImageMenu()
{
	curOption = options.NewIterator();
}

void JImageMenu::ApplyLayout(JTree<JImageMenuEntry *>::Iterator *it)
{
	// Si es distribución libre, lo especificarán desde fuera
	if (config.layout == JIMAGEMENU_FREE)
	{
		return;
	}

	s32 maxX = 0;
	s32 xOff = 0, yOff = 0;

	// Determina la anchura máxima entre todas las opciones de esta rama
	do
	{
		maxX = maxX < it->Data()->Image()->Width() ? it->Data()->Image()->Width() : maxX;

		// Renderiza los hijos
		if (it->Child())
		{
			ApplyLayout(it);
		}
	} while (it->Next());

	it->FirstInBranch();

	// Aplica la distribución de menú
	do
	{
		switch (config.layout)
		{
		case JIMAGEMENU_LEFT:
			it->Data()->Image()->Pos(0, yOff);
			it->Data()->HiImage()->Pos(0, yOff);
			yOff += it->Data()->Image()->Height();
			break;
				
		case JIMAGEMENU_RIGHT:
			xOff = -it->Data()->Image()->Width();
			it->Data()->Image()->Pos(xOff, yOff);
			it->Data()->HiImage()->Pos(xOff, yOff);
			yOff += it->Data()->Image()->Height();
			break;
				
		case JIMAGEMENU_CENTER:
			xOff = -(it->Data()->Image()->Width()/2);
			it->Data()->Image()->Pos(xOff, yOff);
			it->Data()->HiImage()->Pos(xOff, yOff);
			yOff += it->Data()->Image()->Height();
			break;
				
		case JIMAGEMENU_SAMELINE:
			it->Data()->Image()->Pos(xOff, 0);
			it->Data()->HiImage()->Pos(xOff, 0);
			xOff += it->Data()->Image()->Width();
			break;
				
		default:
			break;
		}
	} while (it->Next());

	it->Parent();
}

bool JImageMenu::Init(JImageMenuConfig &cfg)
{
	if (curOption == 0)
	{
		// No es una aplicación válida o no se añadieron opciones
		return false;
	}

	memcpy(&config, &cfg, sizeof(config));

	// Renderiza las opciones de menú
	curOption->Root();
	ApplyLayout(curOption);

	return true;
}

void JImageMenu::Pos(float x, float y)
{
	pos.x = x;
	pos.y = y;
	if (curOption)
	{
		// Renderiza las opciones de menú
		curOption->Root();
		ApplyLayout(curOption);
	}
}

void JImageMenu::Draw()
{
	JTree<JImageMenuEntry *>::Iterator *it = new JTree<JImageMenuEntry *>::Iterator(*curOption);
	JImage * img;

	it->FirstInBranch();

	do
	{
		// Si es la opción seleccionada muestra su imagen resaltada
		if (it->Data() == curOption->Data())
		{
			img = it->Data()->HiImage();
		}
		else
		{
			img = it->Data()->Image();
		}

		img->Draw((s32)(X() + img->X()), (s32)(Y() + img->Y()));

	} while (it->Next());
	
	delete it;
}

void JImageMenu::TrackKeyboard(SDL_keysym key)
{
	// Actualiza el estado según el teclado
	if (config.trackKeyboard)
	{
		switch (key.sym)
		{
		case SDLK_TAB:
			// SHIFT + TAB
			if (JApp::App()->KeyMods() & KMOD_SHIFT)
			{
				// Opción anterior
				curOption->Prev();
			}
			// TAB
			else
			{
				// Opción siguiente
				curOption->Next();
			}
			break;

		case SDLK_UP:
		case SDLK_LEFT:
			// Opción anterior
			curOption->Prev();
			break;

		case SDLK_DOWN:
		case SDLK_RIGHT:
			// Opción siguiente
			curOption->Next();
			break;

		case SDLK_KP_ENTER:
		case SDLK_RETURN:
			// Ejecuta la acción asociada
			if (!curOption->Data()->Select() && config.autoEnter)
			{
				// Si no hay acción asociada y se especificó autoEnter, intenta descender
				// a la opción hija.
				curOption->Child();
			}
			break;
		
		case SDLK_ESCAPE:
			// Va a la opción padre.
			curOption->Parent();
			break;

		default:
			break;
		} // switch (key)
	} // Track keyboard
}

void JImageMenu::TrackMouse(s32 bt, s32 x, s32 y)
{
	// Actualiza el estado según el ratón
	if (config.trackMouse)
	{
		JTree<JImageMenuEntry *>::Iterator *it = new JTree<JImageMenuEntry *>::Iterator(*curOption);
		s32 mx, my;
		bool found = false;
		mx = JApp::App()->MouseX();
		my = JApp::App()->MouseY();

		it->FirstInBranch();

		// Comprueba si está sobre alguna opción
		do
		{
			if (mx > it->Data()->Image()->X() + X() && 
					mx < it->Data()->Image()->X() + X() + it->Data()->Image()->Width() &&
					my > it->Data()->Image()->Y() + Y() && 
					my < it->Data()->Image()->Y() + Y() + it->Data()->Image()->Height())
			{
				// Está dentro, hace que sea la opción resaltada
				found = true;

				// Borra el iterador actual
				delete(curOption);

				curOption = it;
			}
		} while (!found && it->Next());


		if (found)
		{
			// Si ahora está pulsado, activa el flag de pulsación
			if (bt & SDL_BUTTON_LEFT)
			{
				// Ejecuta la acción asociada
				if (!curOption->Data()->Select() && config.autoEnter)
				{
					// Si no hay acción asociada y se especificó autoEnter, intenta descender
					// a la opción hija.
					curOption->Child();
				}
			}
		}

		// Si encontró una opción bajo el cursor el iterador sobre esa opción
		// pasa a ser el iterador de opción actual, no lo borra
		if (!found)
			delete it;
	} // Track mouse
}

