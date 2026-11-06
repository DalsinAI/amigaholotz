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
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 *  Juan Carlos Seijo Pérez
 *  jacob@mainreactor.net
 */

/** Utility funtions for FLTK v1.1.4 (www.fltk.org).
 * @file    JFLTK.h
 * @author  Juan Carlos Seijo Pérez
 * @date    07/01/2004
 * @version 0.0.1 - 07/01/2004 - First version
 */

#ifndef _JFLTK_INCLUDED
#define _JFLTK_INCLUDED

#include <JLib/Util/JTypes.h>

/** Shifts the selected elements in the browser a line up.
 * @param  brw Browser.
 */
void JFLTK_SelectedUp(Fl_Browser *brw)
{
  for (s32 i = 2; i <= brw->size(); ++i)
  {
    if (0 != brw->selected(i) &&
        0 == brw->selected(i - 1))
    {
			brw->move(i - 1, i);
    }
  }
}

/** Shifts the selected elements in the browser a line down.
 * @param  brw Browser.
 */
void JFLTK_SelectedDown(Fl_Browser *brw)
{
  for (s32 i = brw->size() - 1; i >= 1; --i)
  {
    if (0 != brw->selected(i))
    {
      if (0 == brw->selected(i + 1))
      {
        brw->move(i + 1, i);
      }
    }
  }
}

/** Removes the selected elements in the browser.
 * @param brw Browser.
 */
void JFLTK_RemoveSelected(Fl_Browser *brw)
{
  // Borra los seleccionados
  for (s32 i = 1; i <= brw->size(); ++i)
  {
    if (0 != brw->selected(i))
    {
      brw->remove(i);
      --i;
    }
  }
}

/** Checks if the browser has the string already.
 * @param  brw Browser.
 * @param  text Text to find.
 * @return The index in the list if it exists already, else 0 (zero).
 */
s32 JFLTK_Contains(Fl_Browser *brw, const s8 *text)
{
  bool exists = false;
	s32 i = 0;
  for (i = 1; i <= brw->size() && !exists; ++i)
  {
    if (0 == strcmp(brw->text(i), text))
      exists = true;
  }
	
	if (exists)
		return i - 1;

  return 0;
}

/** Request a name and checks that the name didn't exist in the browser already.
 * 'filename' is a posible name for the file or the default value.
 * @param  filename Name of the file from which to extract the default value or default value..
 * @param  brw Browser to check coincidences..
 * @return Choosen name of 0 (zero) if the user cancels the operation.
 */
const s8 * JFLTK_InputName(const s8 *filename, Fl_Browser *brw)
{
  const s8 *name = "";
  s8 *s;

  if (filename)
  {
    // Shows the name of the file without extension by default
    // Search the first dot from the end
    if (0 != (s = strrchr(filename, '.')))
      *s = 0;

    // Searchs the first slash from the end
    if (0 == (s = strrchr(filename, '/')))
      name = filename;
    else
      name = s + 1;
  }

  name = fl_input("Nombre", name);
  while (name != 0 && JFLTK_Contains(brw, name))
  {
    fl_ask("El nombre ya existe,\npor favor elija otro.");
    name = fl_input("Nombre", name);
  }

  return name;
}

#endif // _JFLTK_INCLUDED
