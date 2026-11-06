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

/** Definiton of a rope.
 * @file    HCRope.h
 * @author  Juan Carlos Seijo Pérez
 * @date    05/06/2004
 * @version 0.0.1 -  05/06/2004 - First version.
 */

#ifndef _HCROPE_INCLUDED
#define _HCROPE_INCLUDED

#include <JLib/Graphics/JDrawable.h>
#include <JLib/Graphics/JImage.h>
#include <JLib/Util/JTimer.h>
#include <JLib/Util/JFile.h>
#include <HCTheme.h>

/** Encapsulates a rope for Holotz's Castle.
 * Consists of three base images (top, middle and edge of the rope).
 * The length of the rope refers to the number of middle parts.
 */
class HCRope : public JDrawable
{
 protected:
	float w;                    /**< Angular veloccity of the edge. */
	float period;               /**< Period of the edge. */
	s32 amplitude;              /**< Rope's amplitude. */
	s32 length;                 /**< Rope's length. */
	JImage top;                 /**< Top image. */
	JImage *middle;             /**< Middle images. */
	JImage edge;                /**< Edge image. */
	JTimer timer;               /**< Timer. */

	s32 subtype;                /**< Theme subtype. */
	s32 direction;              /**< Direction of movement (left/right). */

	/** Loads the rope. 
	 * @param file File opened and positioned already.
	 * @return 0 if it succeeds, 1 if there is an I/O error, 2 if integrity error.
	 */
	u32 Load(JRW &file);

 public:
	/** Creates an empty rope. Init must be called in order to use it.
	 */
	HCRope() : w(0), period(0), amplitude(0), length(5), middle(0), subtype(0)
	{}

	/** Initializes the rope with the given parameters.
	 * @param p Complete period of the rope in seconds (the time the edge needs to go
	 * from left to right and return).
	 * @param a Half the amplitude of movement in the horizontal direction.
	 * @param l Length of the rope, in middle image units.
	 * @param theme Theme to use.
	 */
	bool Init(float p, s32 a, u32 l, HCTheme &theme);
	
	/** Draws the rope.
	 */
	void Draw();

	/** Updates the rope.
	 * @return 0 if not changed, 1 if so. 
	 */
	s32 Update();

	/** Sets the position of the top of the rope.
	 * @param newX New x position.
	 * @param newY New y position.
	 */
	void Pos(float x, float y);

  /** Gets the period of this rope.
   * @return Period of this rope.
   */
  float Period() {return period;}

  /** Gets the amplitude of this rope.
   * @return Amplitude of this rope.
   */
  s32 Amplitude() {return amplitude;}

  /** Gets the length of this rope.
   * @return Length of this rope.
   */
  s32 Length() {return length;}

	/** Returns the edge image.
	 * @return Edge image.
	 */
	JImage & Edge() {return edge;}

	/** Loads the rope using the given theme.
	 * @param  file File opened and positioned already.
	 * @param  theme Theme to use for the graphics.
	 * @return 0 if it succeeds, 1 if there is an I/O error, 2 if integrity error.
	 */
	u32 Load(JRW &file, HCTheme &theme);

	/** Saves the rope. The file must be opened and positioned already.
	 * @param file File opened and positioned already.
	 * @return 0 if it succeeds, 1 if there is an I/O error, 2 if integrity error.
	 */
	u32 Save(JRW &file);

  /** Gets the subtype of this rope.
   * @return Subtype of this rope.
   */
  s32 Subtype() {return subtype;}

  /** Sets the subtype of this rope.
   * @param newSubtype New type of this rope.
   */
  void Subtype(s32 newSubtype) {subtype = newSubtype;}

	/** Returns the direction of movement.
	 * @return 0 if stopped, > 0 if moving right, < 0 if moving left
	 */
	s32 Direction() {return direction;}
	
	/** Destroys the object, frees resources and allows scalar destruction.
	 */
	virtual ~HCRope();
};

#endif // _HCROPE_INCLUDED
