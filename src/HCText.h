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

/** Text messages for Holotz's Castle. Base class.
 * @file    HCText.h
 * @author  Juan Carlos Seijo Pérez
 * @date    25/06/2004
 * @version 0.0.1 - 25/06/2004 - Primera versión.
 */

#ifndef _HCTEXT_INCLUDED
#define _HCTEXT_INCLUDED

#include <JLib/Graphics/JDrawable.h>
#include <JLib/Graphics/JImage.h>
#include <JLib/Graphics/JFont.h>
#include <HCTheme.h>

typedef enum HCTextType
{
	HCTEXTTYPE_DIALOG,                    /**< Dialog balloon. */
	HCTEXTTYPE_NARRATIVE,                 /**< Narrative frame. */
	HCTEXTTYPE_COUNT,                     /**< Number of different types. */
};

/** Encapsulates a generic text message.
 */
class HCText : public JDrawable
{
 protected:
	JFont *font;                          /**< Font to render the text with. */
	JImage text;                          /**< Rendered text. */
	HCTheme *theme;                       /**< Theme of the text object. */
	HCTextType type;                      /**< Type of text object. */
	s32 subtype;                          /**< Subtype within the theme. */
	bool left;                            /**< Peak direction. */
	float *trackX;                        /**< Tracked x position. */
	float *trackY;                        /**< Tracked y position. */
	
	JTimer timer;                         /**< Timer for fades. */
	static s32 textSpeed;                 /**< Text duration factor [1-10]. */
	static s32 minDelay;                  /**< Minimum delay for text. */
	static s32 maxAlpha;                  /**< Maximum alpha value. */

 public:
	/** Creates an empty text object. Init must be called before using it.
	 */
	HCText() : font(0), theme(0), type(HCTEXTTYPE_DIALOG), 
	subtype(0), left(true), trackX(&pos.x), trackY(&pos.y)
	{}

	/** Initializes the object.
	 * @param  _type The type of text object.
	 * @param  _text Text to draw.
	 * @param  _theme Theme from where to load the images.
	 * @param  _font Font to use to render the text.
	 * @param  a The Alignment of the text.
	 * @param  _left Peak direction (only for dialog frames).
	 * @return <b>true</b> if successful, <b>false</b> if not.
	 */
	bool Init(HCTextType _type, const char *_text, HCTheme *_theme, 
						JFont *_font, JFontAlign a, bool _left = true, s32 _subtype = 0,
						u8 r = 255, u8 g = 255, u8 b = 255);

  /** Gets the type of this text.
   * @return Type of this text.
   */
  const HCTextType & Type() {return type;}

  /** Sets the type of this text.
   * @param newType New type of this text.
   */
  void Type(const HCTextType &newType) {type = newType;}

  /** Gets the subtype of this text.
   * @return Subtype of this text.
   */
  s32 Subtype() {return subtype;}

  /** Sets the subtype of this text.
   * @param newSubtype New type of this text.
   */
  void Subtype(s32 newSubtype) {subtype = newSubtype;}

	/** Positions this object.
	 * @param  x New x coordinate.
	 * @param  y New y coordinate.
	 */
	virtual void Pos(float x, float y) 
	{pos.x = x; pos.y = y; text.Pos(x, y); }

  /** Draws the object.
   */
  virtual void Draw();

  /** Updates the object.
	 * @return 2 if disappeared, 1 if following something, 0 otherwise.
   */
	virtual s32 Update();

	/** Follows the given coordinates such that every time the coordinates change,
	 * this text moves. If 0 is passed, the no tracking is made.
	 * @param  xTrack Pointer to the x position to track.
	 * @param  yTrack Pointer to the y position to track.
	 */
	void Track(float *x = 0, float *y = 0);

	/** Chechs whether this text is visible or not.
	 * @return <b>true</b> if it's visible, <b>false</b> otherwise.
	 */
	bool Visible() {return text.Alpha() > 0;}

	/** Gets this text's image.
	 * @return This text's image.
	 */
	JImage& Image() {return text;}

	/** Sets the text speed. The value varies from 1 (slow) to 10 (fast).
	 * @param  speed New value for speed.
	 */
	static void Speed(s32 speed) {textSpeed = speed;}

	/** Gets the text speed. The value varies from 1 (slow) to 10 (fast).
	 * @return speed New value for speed.
	 */
	static s32 Speed() {return textSpeed;}

	/** Prepares the text to be displayed.
	 */
	void Reset();

	/** Skips the text.
	 */
	void Skip() {timer.Start(1);}

	/** Frees resources.
	 */
	void Destroy();

	/** Destroys the object.
	 */
	virtual ~HCText()	{Destroy();}
};

#endif // _HCTEXT_INCLUDED
