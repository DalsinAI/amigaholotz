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

/** Map definition file for Holotz's Castle.
 * @file    HCMap.h
 * @author  Juan Carlos Seijo Pérez
 * @date    29/04/2004
 * @version 0.0.1 - 29/04/2004 - First version.
 */

#ifndef _HCMAP_INCLUDED
#define _HCMAP_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <JLib/Util/JTypes.h>
#include <JLib/Graphics/JDrawable.h>
#include <JLib/Graphics/JImage.h>
#include <JLib/Util/JFile.h>
#include <HCTheme.h>
#include <HCCell.h>
#include <HCBreak.h>
#include <HCContFloor.h>

/** Linked list of cells to be drawn/positioned/updated.
 */
struct HCMapCell
{
	HCCell *cell;
	HCMapCell *next;
};

/** Holotz's Castle map. Consists of a matrix of cells.
 */
class HCMap : public JDrawable
{
 private:
  s32 width;                                      /**< Physical width of the map (in pixels). */
  s32 height;                                     /**< Physical height of the map (in pixels). */

 protected:
  s32 rows;                                       /**< Map rows (logical height). */
  s32 cols;                                       /**< Map columns (logical width). */
  s32 cellWidth;                                  /**< Cell width in pixels. */
  s32 cellHeight;                                 /**< Cell height in pixels. */
	HCCell ***cells;                                /**< Matrix of cells. */
	HCTheme *theme;                                 /**< Theme to be used. */
	s32 level;                                      /**< Level within the world. */

	s32 exitRow;                                    /**< Exit row. */
	s32	exitCol;                                    /**< Exit column. */
	
	float gravity;                                  /**< Gravity of the map. */
	
	HCMapCell *linkCells;                           /**< Linked list of cells to be drawn/updated/positioned. */

	/** Sets allowed actions for every cell in the map.
	 */
	void SetActions();

	/** Builds only the continuous cell at the given position based upon its neighbours.
	 * @param  row Row of the cell to build.
	 * @param  col Column of the cell to build.
	 */
	void BuildContFloorOnce(s32 row, s32 col);

 public:
  /** Creates an empty map. Init() must be called before starting to use it..
   */
  HCMap();

  /** Returns the number of rows.
   * @return number of rows.
   */
  s32 Rows()
  {
    return rows;
  }

  /** Returns the number of cols.
   * @return number of cols.
   */
  s32 Cols()
  {
    return cols;
  }

  /** Returns the width of a cell.
   * @return the width of a cell.
   */
  s32 CellWidth()
  {
    return cellWidth;
  }

  /** Returns the height of a cell.
   * @return the height of a cell.
   */
  s32 CellHeight()
  {
    return cellHeight;
  }

  /** Sets the width of a cell.
   * @param  w The width of a cell.
   */
  void CellWidth(s32 w)
  {
    cellWidth = w;
  }

  /** Sets the height of a cell.
   * @param  h The height of a cell.
   */
  void CellHeight(s32 h)
  {
    cellHeight = h;
  }

  /** Returns the width of the map.
   * @return the width of the map.
   */
  s32 Width()
  {
    return width;
  }

  /** Returns the height of the map.
   * @return the height of the map.
   */
  s32 Height()
  {
    return height;
  }

  /** Initializes this map.
   * @param  _theme Loaded theme to use.
   * @return <b>true</b> if it was loaded successfully, <b>false</b> if not.
   */
  bool Init(HCTheme &_theme);
  
  /** Converts from physical (screen) to logical (map) coords.
   * The result is put in the same variables passed.
   * @param  xPos X coordinate of the pixel and the column after the call.
   * @param  yPos Y coordinate of the pixel and the row after the call.
   */
  void ScreenToMap(s32 &xPos, s32 &yPos);

  /** Converts from logical (map) to physical (screen) coords.
   * The result is put in the same variables passed. 
   * @param  col Column of the cell and the x position of the cell's baseline mid-point.
   * @param  row Row of the cell and the y position of the cell's baseline mid-point.
   */
  void MapToScreen(s32 &col, s32 &row);

  /** Converts from physical (screen) to logical (map) coords.
   * @param  xx X coordinate of the pixel.
   * @return the column corresponding to that x.
   */
  s32 ToCol(s32 xx);

  /** Converts from physical (screen) to logical (map) coords.
   * @param  yy Y coordinate of the pixel.
   * @return the row corresponding to that y.
   */
  s32 ToRow(s32 yy);

  /** Converts from logical (map) to physical (screen) coords.
   * @param  col Column of the cell.
   * @return the x coordinate of the mid-point of the baseline of the cell.
   */
  s32 ToX(s32 col);

  /** Converts from logical (map) to physical (screen) coords.
   * @param  row Row of the cell.
   * @return the y coordinate of the mid-point of the baseline of the cell.
   */
  s32 ToY(s32 row);

	/** Updates the map.
	 * @return 0 if successful, != 0 if not.
	 */
	s32 Update();

  /** Draws the map.
   */
  virtual void Draw();

	/** Positions this map.
	 * @param  xPos New x coordinate.
	 * @param  yPos New y coordinate.
	 */
	virtual void Pos(float xPos, float yPos);

	/** Returns the cells at the row and column specified.
	 * It doesn't check if row and col are valid.
	 * @param  row Cell's row.
	 * @param  col Cell's column.
	 * @return The requested cell.
	 */
	HCCell *Cell(s32 row, s32 col) 
	{return cells[row >= rows ? rows - 1 : row][col >= cols ? cols - 1 : col];}

	/** Returns all the cells.
	 * @return The map's cells.
	 */
	HCCell ***Cells() 
	{return cells;}

	/** Returns the exit column for the main character.
	 * @return the exit column for the main character.
	 */
	s32 ExitCol() {return exitCol;}

	/** Returns the exit row for the main character.
	 * @return the exit row for the main character.
	 */
	s32 ExitRow() {return exitRow;}

	/** Returns the exit column for the main character.
	 * @param  col The exit column for the main character.
	 */
	void ExitCol(s32 col) {exitCol = col;}

	/** Returns the exit row for the main character.
	 * @param  row The exit row for the main character.
	 */
	void ExitRow(s32 row) {exitRow = row;}

	/** Loads the map.
	 * @param  f The file from where to load, already positioned.
	 * @return 0 if succeeded, 1 if I/O error or 2 if integrity error.
	 */
	u32 Load(JRW &f);

	/** Saves the map.
	 * @param  f The file where to save, already positioned.
	 * @return 0 if succeeded, 1 if I/O error or 2 if integrity error.
	 */
	u32 Save(JRW &f);

  /** Destroys the object and free its associated resources.
   */
  void Destroy();

	/** Resizes the map. If enlarged, the old cells are pasted from the
	 * bottom-left corner of the original map to the same corner of the new map.
	 * @param  newRows New number of rows.
	 * @param  newCols New number of cols.
	 * @param  growRight Indicates whether it must grow to the right (<b>true</b>, by default) or to the left.
	 * @param  growDown Indicates whether it must grow down (<b>true</b>, by default) or upwards.
	 */
	void Resize(s32 newRows, s32 newCols, bool growRight = true, bool growDown = true);

	/** Builds the continuous cell at the given position and its neighbours.
	 * @param  row Row of the cell to build.
	 * @param  col Column of the cell to build.
	 */
	void BuildContFloor(s32 row, s32 col);
	
	/** (Re)Builds the linked list of cells.
	 */
	void BuildCellLinkList();

	/** Destroys the linked list of cells.
	 */
	void DestroyCellLinkList();
	
	/** Sets the gravity for this map.
	 * @param  g New gravity for this map.
	 */
	void Gravity(float g) {gravity = g;}
 
	/** Gets the gravity for this map.
	 * @return  Gravity for this map.
	 */
	float Gravity() {return gravity;}
 
  /** Destroys this object.
   */
  virtual ~HCMap() {Destroy();}
};

#endif // _HCMAP_INCLUDED
