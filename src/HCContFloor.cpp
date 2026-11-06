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
 * @file    HCContFloor.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    02/06/2004
 * @version 0.0.1 - 02/06/2004 - First version.
 */

#include <HCContFloor.h>

void HCContFloor::Build(JImage *parts, 
												bool n7, 
												bool n8, 
												bool n9,
												bool n4, 
												bool n6, 
												bool n1, 
												bool n2, 
												bool n3)
{
	s32 cw = parts[HCFDT_C1].Width();
	s32 ch = parts[HCFDT_C1].Height();
	s32 iw = parts[HCFDT_I].Width();
	s32 ih = parts[HCFDT_I].Height();

	JImage *img;
	drawable.Create((cw * 2) + iw, (ch * 2) + ih,	32);
	img = &drawable;

	// UL Corner
	if (n7)
	{
		if (n4)
		{
			if (n8)
			{
				// The three exist (*) around us (X):
				//
				//  **
				//  *X
				img->Paste(&parts[HCFDT_C], 0, 0, cw, ch, 0, 0);
			}
			else
			{
				//  * 
				//  *X
				img->Paste(&parts[HCFDT_CU], 0, 0, cw, ch, 0, 0);
			}
		}
		else
		{
			if (n8)
			{
				//  **
				//   X
				img->Paste(&parts[HCFDT_CL], 0, 0, cw, ch, 0, 0);
			}
			else
			{
				//  * 
				//   X
				img->Paste(&parts[HCFDT_C7], 0, 0, cw, ch, 0, 0);
			}
		}
	}
	else
	{
		if (n4)
		{
			if (n8)
			{
				//   *
				//  *X
				img->Paste(&parts[HCFDT_C7UL], 0, 0, cw, ch, 0, 0);
			}
			else
			{
				//    
				//  *X
				img->Paste(&parts[HCFDT_CU], 0, 0, cw, ch, 0, 0);
			}
		}
		else
		{
			if (n8)
			{
				//   *
				//   X
				img->Paste(&parts[HCFDT_CL], 0, 0, cw, ch, 0, 0);
			}
			else
			{
				//    
				//   X
				img->Paste(&parts[HCFDT_C7], 0, 0, cw, ch, 0, 0);
			}
		}
	}

	// UR Corner
	if (n9)
	{
		if (n6)
		{
			if (n8)
			{
				// **
				// X*
				img->Paste(&parts[HCFDT_C], 0, 0, cw, ch, cw + iw, 0);
			}
			else
			{
				//  * 
				// X*
				img->Paste(&parts[HCFDT_CU], 0, 0, cw, ch, cw + iw, 0);
			}
		}
		else
		{
			if (n8)
			{
				// **
				// X
				img->Paste(&parts[HCFDT_CR], 0, 0, cw, ch, cw + iw, 0);
			}
			else
			{
				//  * 
				// X
				img->Paste(&parts[HCFDT_C9], 0, 0, cw, ch, cw + iw, 0);
			}
		}
	}
	else
	{
		if (n6)
		{
			if (n8)
			{
				// *
				// X*
				img->Paste(&parts[HCFDT_C9UR], 0, 0, cw, ch, cw + iw, 0);
			}
			else
			{
				//    
				// X*
				img->Paste(&parts[HCFDT_CU], 0, 0, cw, ch, cw + iw, 0);
			}
		}
		else
		{
			if (n8)
			{
				// *
				// X
				img->Paste(&parts[HCFDT_CR], 0, 0, cw, ch, cw + iw, 0);
			}
			else
			{
				//    
				// X
				img->Paste(&parts[HCFDT_C9], 0, 0, cw, ch, cw + iw, 0);
			}
		}
	}

	// DL Corner
	if (n1)
	{
		if (n4)
		{
			if (n2)
			{
				// *X
				// **
				img->Paste(&parts[HCFDT_C], 0, 0, cw, ch, 0, ch + ih);
			}
			else
			{
				// *X
				//  * 
				img->Paste(&parts[HCFDT_C1DL], 0, 0, cw, ch, 0, ch + ih);
			}
		}
		else
		{
			if (n2)
			{
				//  X
				// **
				img->Paste(&parts[HCFDT_CL], 0, 0, cw, ch, 0, ch + ih);
			}
			else
			{
				//  X
				// * 
				img->Paste(&parts[HCFDT_C1], 0, 0, cw, ch, 0, ch + ih);
			}
		}
	}
	else
	{
		if (n4)
		{
			if (n2)
			{
				// *X
				//  *
				img->Paste(&parts[HCFDT_C1DL], 0, 0, cw, ch, 0, ch + ih);
			}
			else
			{
				// *X
				//    
				img->Paste(&parts[HCFDT_CD], 0, 0, cw, ch, 0, ch + ih);
			}
		}
		else
		{
			if (n2)
			{
				// X
				// *
				img->Paste(&parts[HCFDT_CL], 0, 0, cw, ch, 0, ch + ih);
			}
			else
			{
				// X
				//    
				img->Paste(&parts[HCFDT_C1], 0, 0, cw, ch, 0, ch + ih);
			}
		}
	}

	// DR Corner
	if (n3)
	{
		if (n6)
		{
			if (n2)
			{
				// X*
				// **
				img->Paste(&parts[HCFDT_C], 0, 0, cw, ch, cw + iw, ch + ih);
			}
			else
			{
				// X*
				//  * 
				img->Paste(&parts[HCFDT_CD], 0, 0, cw, ch, cw + iw, ch + ih);
			}
		}
		else
		{
			if (n2)
			{
				// X
				// **
				img->Paste(&parts[HCFDT_CR], 0, 0, cw, ch, cw + iw, ch + ih);
			}
			else
			{
				// X
				//  * 
				img->Paste(&parts[HCFDT_C3], 0, 0, cw, ch, cw + iw, ch + ih);
			}
		}
	}
	else
	{
		if (n6)
		{
			if (n2)
			{
				// X*
				// *
				img->Paste(&parts[HCFDT_C3DR], 0, 0, cw, ch, cw + iw, ch + ih);
			}
			else
			{
				// X*
				//    
				img->Paste(&parts[HCFDT_CD], 0, 0, cw, ch, cw + iw, ch + ih);
			}
		}
		else
		{
			if (n2)
			{
				// X
				// *
				img->Paste(&parts[HCFDT_CR], 0, 0, cw, ch, cw + iw, ch + ih);
			}
			else
			{
				// X
				//    
				img->Paste(&parts[HCFDT_C3], 0, 0, cw, ch, cw + iw, ch + ih);
			}
		}
	}

	// U Side
	if (n8)
	{
		//  *
		//  X
		img->Paste(&parts[HCFDT_SV], 0, 0, iw, ch, cw, 0);
	}
	else
	{
		//   
		//  X
		img->Paste(&parts[HCFDT_S8], 0, 0, iw, ch, cw, 0);
	}

	// D Side
	if (n2)
	{
		//  X
		//  *
		img->Paste(&parts[HCFDT_SV], 0, 0, iw, ch, cw, ch + ih);
	}
	else
	{
		//  X
		//   
		img->Paste(&parts[HCFDT_S2], 0, 0, iw, ch, cw, ch + ih);
	}

	// L Side
	if (n4)
	{
		//  
		// *X
		img->Paste(&parts[HCFDT_SH], 0, 0, cw, ih, 0, ch);
	}
	else
	{
		//   
		//  X
		img->Paste(&parts[HCFDT_S4], 0, 0, cw, ih, 0, ch);
	}

	// R Side
	if (n6)
	{
		//  
		// X*
		img->Paste(&parts[HCFDT_SH], 0, 0, cw, ih, cw + iw, ch);
	}
	else
	{
		//   
		// X
		img->Paste(&parts[HCFDT_S6], 0, 0, cw, ih, cw + iw, ch);
	}

	// The center cannot change
	img->Paste(&parts[HCFDT_I], 0, 0, iw, ih, cw, ch);
}
