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

/** Menu en pantalla compuesto de cadenas de texto.
 * @file    JTextMenu.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    28/03/2004
 * @version 0.0.1 - 28/03/2004 - Primera versión.
 */

#include <JLib/Graphics/JTextMenu.h>

bool JTextMenuEntry::Init(JTextMenuConfig &cfg)
{
	// Borra posibles imágenes anteriores
	Destroy();

	// Renderiza según el tipo elegido
	switch (cfg.renderMode)
	{
	default:
	case JTEXTMENU_SOLID:
		image = cfg.font->RenderTextSolid(text, cfg.color);
		hiImage = cfg.font->RenderTextSolid(text, cfg.hiColor);
		break;

	case JTEXTMENU_SHADED:
		image = cfg.font->RenderTextShaded(text, cfg.color, cfg.backColor);
		hiImage = cfg.font->RenderTextShaded(text, cfg.hiColor, cfg.hiBackColor);
		break;

	case JTEXTMENU_BLENDED:
		image = cfg.font->RenderTextBlended(text, cfg.color);
		hiImage = cfg.font->RenderTextBlended(text, cfg.hiColor);
		break;
	}

	if (image != 0 && hiImage != 0)
		return true;

	return false;
}

bool JTextMenuEntry::Select()
{
	if (Action)
	{
		Action(data);
		return true;
	}
	
	return false;
}


JTextMenu::JTextMenu()
{
	curOption = options.NewIterator();
	maxW = maxH = 0;
}

void JTextMenu::ApplyLayout(JTree<JTextMenuEntry *>::Iterator *it)
{
	s32 xOff = 0, yOff = 0;
	s32 localW = 0, localH = 0;

	do
	{
		// Determina la anchura y altura máximas entre todas las opciones de esta rama
		if (config.layout != JTEXTMENU_SAMELINE)
		{
			xOff = it->Data()->Image()->Width();
		}
		else
		{
			xOff += it->Data()->Image()->Width();
		}
			
		if (config.layout != JTEXTMENU_SAMELINE)
		{
			if (yOff > 0)
			{
				// El número de espacios entre líneas es uno menos que el número de opciones
				yOff += config.lineDistance;
			}
			yOff += it->Data()->Image()->Height();
		}
		else
		{
			yOff = 0;
		}
		
		localW = xOff > localW ? xOff : localW;
		localH = yOff > localH ? yOff : localH;
		
		// Distribuye a los hijos
		if (it->Child())
		{
			ApplyLayout(it);
		}
	} while (it->Next());

	xOff = 0;
	it->FirstInBranch();

	switch (config.layoutV)
	{
		// La posición y del menú es la del borde inferior
	case JTEXTMENU_DOWN:
		yOff = -localH;
		break;
		
		// La posición y del menú es la del centro
	case JTEXTMENU_CENTER:
		yOff = -localH/2;
		break;
		
		// La posición y del menú es la del borde superior
	case JTEXTMENU_UP:
	default:
		yOff = 0;
		break;
	}

	// Aplica la distribución de menú
	do
	{
		switch (config.layout)
		{
			// La posición x del menú es la del borde izquierdo
		case JTEXTMENU_LEFT:
			it->Data()->Image()->Pos(0, yOff);
			it->Data()->HiImage()->Pos(0, yOff);
			yOff += it->Data()->Image()->Height() + config.lineDistance;
			break;
			
			// La posición x del menú es la del borde derecho
		case JTEXTMENU_RIGHT:
			xOff = -it->Data()->Image()->Width();
			it->Data()->Image()->Pos(xOff, yOff);
			it->Data()->HiImage()->Pos(xOff, yOff);
			yOff += it->Data()->Image()->Height() + config.lineDistance;
			break;
				
			// La posición x del menú es la del centro
		case JTEXTMENU_CENTER:
			xOff = -it->Data()->Image()->Width()/2;
			it->Data()->Image()->Pos(xOff, yOff);
			it->Data()->HiImage()->Pos(xOff, yOff);
			yOff += it->Data()->Image()->Height() + config.lineDistance;
			break;
				
			// La posición x del menú es la del borde izquierdo
		case JTEXTMENU_SAMELINE:
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

bool JTextMenu::RenderBranch(JTree<JTextMenuEntry *>::Iterator *it)
{
	bool ok = true;
	s32 xOff = 0, yOff = 0;

	do
	{
		// Renderiza esta opción
		ok = it->Data()->Init(config);

		// Determina la anchura y altura máximas entre todas las opciones de esta rama
		if (config.layout != JTEXTMENU_SAMELINE)
		{
			xOff = it->Data()->Image()->Width();
		}
		else
		{
			xOff += it->Data()->Image()->Width();
		}
			
		if (config.layout != JTEXTMENU_SAMELINE)
		{
			if (yOff > 0)
			{
				// El número de espacios entre líneas es uno menos que el número de opciones
				yOff += config.lineDistance;
			}
			yOff += it->Data()->Image()->Height();
		}
		else
		{
			yOff = it->Data()->Image()->Height();
		}

		maxW = xOff > maxW ? xOff : maxW;
		maxH = yOff > maxH ? yOff : maxH;

		// Renderiza los hijos
		if (ok && it->Child())
		{
			ok = RenderBranch(it);
		}
	} while (ok && it->Next());

	it->Parent();

	return ok;
}

bool JTextMenu::Init(JTextMenuConfig &cfg)
{
	if (curOption == 0 || cfg.font == 0)
	{
		// Parámetro inválido
		return false;
	}

	memcpy(&config, &cfg, sizeof(config));

	// Altura de línea
	if (config.lineDistance < 0)
	{
		config.lineDistance = config.font->LineDistance();
	}

	// Renderiza las opciones de menú
	curOption->Root();
	if (RenderBranch(curOption))
	{
		curOption->Root();
		ApplyLayout(curOption);
		curOption->Root();

		return true;
	}

	return false;
}

void JTextMenu::Draw()
{
	JTree<JTextMenuEntry *>::Iterator *it = new JTree<JTextMenuEntry *>::Iterator(*curOption);
	s32 xOff, yOff;

	it->FirstInBranch();

	do
	{
		// Si es la opción seleccionada muestra su imagen resaltada
		if (it->Data() == curOption->Data())
		{
			xOff = (s32)it->Data()->HiImage()->X();
			yOff = (s32)it->Data()->HiImage()->Y();
			it->Data()->HiImage()->Draw((s32)(X() + xOff), (s32)(Y() + yOff));
		}
		else
		{
			xOff = (s32)it->Data()->Image()->X();
			yOff = (s32)it->Data()->Image()->Y();
			it->Data()->Image()->Draw((s32)(X() + xOff), (s32)(Y() + yOff));
		}
	} while (it->Next());
	
	delete it;
}

void JTextMenu::TrackKeyboard(SDL_keysym key)
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
				if (!curOption->Prev())
					curOption->LastInBranch();
			}
			// TAB
			else
			{
				// Opción siguiente
				if (!curOption->Next())
					curOption->FirstInBranch();
			}
			break;

		case SDLK_UP:
		case SDLK_LEFT:
			// Opción anterior
			if (!curOption->Prev())
				curOption->LastInBranch();
			break;

		case SDLK_DOWN:
		case SDLK_RIGHT:
			// Opción siguiente
			if (!curOption->Next())
				curOption->FirstInBranch();
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

void JTextMenu::TrackMouse(s32 bt, s32 x, s32 y)
{
	// Actualiza el estado según el ratón
	if (config.trackMouse)
	{
		JTree<JTextMenuEntry *>::Iterator *it = new JTree<JTextMenuEntry *>::Iterator(*curOption);
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
