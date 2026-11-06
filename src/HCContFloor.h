/*
 * Holotz's Castle
 * Copyright (C) 2004 Juan Carlos Seijo Pérez
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the Free 
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 59 
 * Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * 
 * Juan Carlos Seijo Pérez
 * jacob@mainreactor.net
 */

/** Continuous floor cell.
 * @file    HCContFloor.h
 * @author  Juan Carlos Seijo Pérez
 * @date    02/06/2004
 * @version 0.0.1 - 02/06/2004 - First version.
 */

#ifndef _HCCONTFLOOR_INCLUDED
#define _HCCONTFLOOR_INCLUDED

#include <HCCell.h>
#include <HCTheme.h>

class HCContFloor : public HCDrawableCell
{
 public:
	
	HCContFloor() : HCDrawableCell(HCCELLTYPE_CONTFLOOR, 0, 0)
	{}
	
	/** Builds the image for this floor.
	 * @param  parts Array of parts of this floor (corners, sides, etc.)
	 * Tipically it is retrieved from HCTheme::ContFloor().
	 * @param  n7 Neighbour at position 7 (as in the keypad from your keyboard). false for none.
	 * @param  n8 Neighbour at position 8 (as in the keypad from your keyboard). false for none.
	 * @param  n9 Neighbour at position 9 (as in the keypad from your keyboard). false for none.
	 * @param  n4 Neighbour at position 4 (as in the keypad from your keyboard). false for none.
	 * @param  n6 Neighbour at position 6 (as in the keypad from your keyboard). false for none.
	 * @param  n1 Neighbour at position 1 (as in the keypad from your keyboard). false for none.
	 * @param  n2 Neighbour at position 2 (as in the keypad from your keyboard). false for none.
	 * @param  n3 Neighbour at position 3 (as in the keypad from your keyboard). false for none.
	 */
	void Build(JImage *parts, 
						 bool n7,
						 bool n8,
						 bool n9,
						 bool n4,
						 bool n6,
						 bool n1,
						 bool n2,
						 bool n3);

	/** Allows scalar destruction.
	 */
	virtual ~HCContFloor()
	{}
};

#endif // _HCCONTFLOOR_INCLUDED
