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
 * @file    HCMap.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    29/04/2004
 * @version 0.0.1 - 29/04/2004 - First version.
 */

#include <HCMap.h>

HCMap::HCMap()
{
	cells = 0;
	rows = cols = cellWidth = cellHeight = 0;
	width = height = 0;
	exitRow = exitCol = 0;
	linkCells = 0;
	
	//gravity = 0.7f;
	gravity = 0.9f;
}

bool HCMap::Init(HCTheme &_theme)
{
	cells = 0;
	rows = cols = 0;
	width = height = 0;
	exitRow = exitCol = 0;

	theme = &_theme;

	// Inits common parameters
	cellWidth = theme->Floor(0).Width();
	cellHeight = theme->Floor(0).Height();

	Pos(0, 0);

	return true;
}

s32 HCMap::Update()
{
	HCMapCell *mc = linkCells;

	while (mc)
	{
		mc->cell->Update();
		mc = mc->next;
	}

  return 1;
}

void HCMap::Draw()
{
	HCMapCell *mc = linkCells;

	while (mc)
	{
		mc->cell->Draw();
		mc = mc->next;
	}
}

void HCMap::Pos(float xPos, float yPos)
{
	pos.x = xPos;
	pos.y = yPos;

	for (s32 j = 0; j < rows; ++j)
	{
		for (s32 i = 0; i < cols; ++i)
		{
			cells[j][i]->Pos(xPos + (i * cellWidth), yPos + (j * cellHeight));
		}
	}
}

void HCMap::ScreenToMap(s32 &xPos, s32 &yPos)
{
	xPos = ToCol(xPos);
	yPos = ToRow(yPos);
}

void HCMap::MapToScreen(s32 &col, s32 &row)
{
	col = ToX(col);
	row = ToY(row);

	// Adds the map offset and the distance to the baseline's mid-point
	col += (s32)pos.x + cellWidth/2;
	row += (s32)Y() + cellHeight - 1;
}

s32 HCMap::ToCol(s32 xx)
{
	// Removes map offsets
	xx -= (s32)pos.x;

	if (xx < 0)
	{
		return 0;
	}

	xx /= cellWidth;
	
	return (xx > cols-1 ? cols-1 : xx);
}

s32 HCMap::ToRow(s32 yy)
{
	// Removes map offsets
	yy -= (s32)pos.y;

	if (yy < 0)
	{
		return 0;
	}

	yy /= cellHeight;

	return (yy > rows-1 ? rows-1 : yy);
}

s32 HCMap::ToX(s32 col)
{
	col *= cellWidth;

	// Adds the map offset and the distance to the baseline's mid-point
	col += (s32)pos.x + cellWidth/2;
	return col;
}

s32 HCMap::ToY(s32 row)
{
	row *= cellHeight;

	// Adds the map offset and the distance to the baseline's mid-point
	row += (s32)pos.y + cellHeight - 1;
	return row;
}

void HCMap::Resize(s32 newRows, s32 newCols, bool growRight, bool growDown)
{
	if (newRows == 0 || newCols == 0)
	{
		// Only destroys the cells
		Destroy();

		return;
	}

	HCCell ***newCells;

	// Creates the map cells
	newCells = new HCCell **[newRows];
	
	for (s32 j = 0; j < newRows; ++j)
	{
		newCells[j] = new HCCell *[newCols];
		memset(newCells[j], 0, sizeof(HCCell*) * newCols);
	}

	if (cells != 0)
	{
		s32 maxRows = rows > newRows ? newRows : rows;
		s32 maxCols = cols > newCols ? newCols : cols;

		// Copies the existing cells, reducing, if necessary from the top right
		// corner. Defaults the rest of the cells, in case of enlargement, to blank
		for (s32 row = 0; row < newRows; ++row)
		{
			for (s32 col = 0; col < newCols; ++col)
			{
				if (row < maxRows && col < maxCols)
				{
					// while reducing
					if (growDown)
					{
						if (growRight)
						{
							newCells[row][col] = cells[row][col];
							cells[row][col] = 0;
						}
						else
						{
							newCells[row][newCols - 1 - col] = cells[row][cols - col - 1];
							cells[row][cols - col - 1] = 0;
						}
					}
					else
					{
						if (growRight)
						{
							newCells[newRows - 1 - row][col] = cells[rows - row - 1][col];
							cells[rows - row - 1][col] = 0;
						}
						else
						{
							newCells[newRows - 1 - row][newCols - 1 - col] = cells[rows - row - 1][cols - col - 1];
							cells[rows - row - 1][cols - col - 1] = 0;
						}
					}
				}
				else
				{
					// If enlarging
					if (growDown)
					{
						if (growRight)
						{
							newCells[row][col] = new HCCell;                                // Grows down-right
						}
						else
						{
							newCells[row][newCols - 1 - col] = new HCCell;                  // Grows down-left
						}
					}
					else
					{
						if (growRight)
						{
							newCells[newRows - 1 - row][col] = new HCCell;                  // Grows up-left
						}
						else
						{
							newCells[newRows - 1 - row][newCols - 1 - col] = new HCCell;    // Grows up-right
						}
					}
				}
			}
		}
		
		// Destroy the rest of the original cells if reducing
		Destroy();
	}
	else
	{
		// Initializes the map to empty cells
		for (s32 row = 0; row < newRows; ++row)
		{
			for (s32 col = 0; col < newCols; ++col)
			{
				newCells[row][col] = new HCCell;
			}
		}
	}

	cells = newCells;
	rows = newRows;
	cols = newCols;
	width = cellWidth * cols;
	height = cellHeight * rows;
	
	Pos(pos.x, pos.y);
	
	BuildCellLinkList();
}

void HCMap::Destroy()
{
  if (0 != cells)
  {
    for (s32 row = 0; row < rows; ++row)
    {
      for (s32 col = 0; col < cols; ++col)
      {
        JDELETE(cells[row][col]);
      }

      delete[] cells[row];
      cells[row] = 0;
    }

    delete[] cells;
    cells = 0;
  }
	
	DestroyCellLinkList();
}

void HCMap::BuildContFloorOnce(s32 row, s32 col)
{
	if (cells[row][col]->Type() == HCCELLTYPE_CONTFLOOR)
	{
		((HCContFloor *)cells[row][col])->Build(theme->ContFloor(cells[row][col]->Subtype()), 
																						cells[row-1][col-1]->Type() == HCCELLTYPE_CONTFLOOR,
																						cells[row-1][col]->Type() == HCCELLTYPE_CONTFLOOR,
																						cells[row-1][col+1]->Type() == HCCELLTYPE_CONTFLOOR,
																						cells[row][col-1]->Type() == HCCELLTYPE_CONTFLOOR,
																						cells[row][col+1]->Type() == HCCELLTYPE_CONTFLOOR,
																						cells[row+1][col-1]->Type() == HCCELLTYPE_CONTFLOOR,
																						cells[row+1][col]->Type() == HCCELLTYPE_CONTFLOOR,
																						cells[row+1][col+1]->Type() == HCCELLTYPE_CONTFLOOR);
	}
}

void HCMap::BuildContFloor(s32 row, s32 col)
{
	BuildContFloorOnce(row - 1, col - 1);
	BuildContFloorOnce(row - 1, col);
	BuildContFloorOnce(row - 1, col + 1);
	BuildContFloorOnce(row, col - 1);
	BuildContFloorOnce(row, col);
	BuildContFloorOnce(row, col + 1);
	BuildContFloorOnce(row + 1, col - 1);
	BuildContFloorOnce(row + 1, col);
	BuildContFloorOnce(row + 1, col + 1);
}

void HCMap::BuildCellLinkList()
{
	DestroyCellLinkList();

	HCMapCell *mc = 0, *mcOld = 0;

	for (s32 row = 0; row < rows; ++row)
	{
		for (s32 col = 0; col < cols; ++col)
		{
			if (cells[row][col]->Type() != HCCELLTYPE_BLANK)
			{
				mc = new HCMapCell;
				mc->cell = cells[row][col];

				if (mcOld)
				{
					mcOld->next = mc;
				}

				if (!linkCells)
				{
					linkCells = mc;
				}
        
        mcOld = mc;
			}
		} 
	}
	
	if (mc)
	{
		mc->next = 0;
	}
}

void HCMap::DestroyCellLinkList()
{
	if (0 != linkCells)
	{
		HCMapCell *mc = linkCells, *tmp;

		while (mc)
		{
			tmp = mc->next;
			delete mc;
			mc = tmp;
		}
		
		linkCells = 0;
	}
}

u32 HCMap::Load(JRW &f)
{
	Destroy();

	// Loads the number of rows and cols, etc.
	if (0 == f.ReadLE32(&gravity) ||
			0 == f.ReadLE32(&rows) ||
			0 == f.ReadLE32(&cols) ||
			0 == f.ReadLE32(&exitRow) ||
			0 == f.ReadLE32(&exitCol))
	{
		return 1;
	}

	Resize(rows, cols);

	// Loads the cells
	s32 ret = 1;
	s32 t, subt;
	HCBreak *prevBreak = 0;
	
	for (s32 row = 0; ret != 0 && row < rows; ++row)
	{
		for (s32 col = 0; ret != 0 && col < cols; ++col)
		{
			ret = f.ReadLE32(&t);
			ret = f.ReadLE32(&subt);

			if (t != HCCELLTYPE_BREAK)
			{
				prevBreak = 0;
			}

			JDELETE(cells[row][col]);

			switch (t)
			{
			case HCCELLTYPE_FLOOR:
				cells[row][col] = new HCFloorCell(&(theme->Floor(subt)));
				cells[row][col]->Subtype(subt);
				break;

			case HCCELLTYPE_CONTFLOOR:
				cells[row][col] = new HCContFloor;
				cells[row][col]->Subtype(subt);
				break;

			case HCCELLTYPE_BAR:
				cells[row][col] = new HCBarCell(&(theme->Bar(subt)));
				cells[row][col]->Subtype(subt);
				break;

			case HCCELLTYPE_LADDER:
				cells[row][col] = new HCLadderCell(&(theme->Ladder(subt)));
				cells[row][col]->Subtype(subt);
				break;

			case HCCELLTYPE_BREAK:
				cells[row][col] = prevBreak = new HCBreak(theme->Break(subt),	prevBreak);
				cells[row][col]->Subtype(subt);
				break;

			case HCCELLTYPE_BLANK:
			default:
				cells[row][col] = new HCCell;
				cells[row][col]->Subtype(subt);
				break;
			}

			t = cells[row][col]->Type();
		}
	}

	for (s32 row = 1; ret != 0 && row < rows - 1; ++row)
	{
		for (s32 col = 1; ret != 0 && col < cols - 1; ++col)
		{
			if (cells[row][col]->Type() == HCCELLTYPE_CONTFLOOR)
				BuildContFloor(row, col);
		}
	}
	
	BuildCellLinkList();

	return 0;
}

u32 HCMap::Save(JRW &f)
{
	// Saves the number of rows and cols, etc.
	if (0 == f.WriteLE32(&gravity) ||
			0 == f.WriteLE32(&rows) ||
			0 == f.WriteLE32(&cols) ||
			0 == f.WriteLE32(&exitRow) ||
			0 == f.WriteLE32(&exitCol))
	{
		return 1;
	}
	
	// Saves the cells
	s32 ret = 1;
	s32 data, index;

	for (s32 row = 0; ret != 0 && row < rows; ++row)
	{
		for (s32 col = 0; ret != 0 && col < cols; ++col)
		{
			data = cells[row][col]->Type();
			index = cells[row][col]->Subtype();
			ret = f.WriteLE32(&data);
			ret = f.WriteLE32(&index);
		} 
	}
	
	return 0;
}
