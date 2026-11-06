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

/** Defines a chaser enemy.
 * @file    HCEnemyChaser.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    27/05/2004
 * @version 0.0.1 - 27/05/2004 - First version.
 */

#include <HCEnemyChaser.h>

HCEnemyChaser::HCEnemyChaser() : HCEnemy(HCENEMYTYPE_CHASER)
{
	chased = 0;
	state = HCCS_LEFT;
	cells = 0;
	chasePath = 0;
	tmpPath = 0;
	chaseIndex = 0;
}

bool HCEnemyChaser::Init(JImageSprite *sprites, HCMap *_map, HCRope **_ropes, s32 nRopes)
{
	if (HCEnemy::Init(sprites, _map, _ropes, nRopes))
	{
		// Uses param2 to set search depth for this enemy.
		// Param2 represents the percentage of the map covered by the search in the greates direction (H or V).
		searchDepth = (s32)JMax(param2 * (map->Rows() + map->Cols())/10, 3);
		ResetChaseMap();
		
		chasePath = new HCChasePath[searchDepth + 2];
		tmpPath = new HCChasePath[searchDepth + 2];
		
		return true;
	}
	
	return false;
}

void HCEnemyChaser::ResetChaseMap()
{
	if (!cells)
	{
		// Creates the cell array from the map
		cells = new HCChaseCell *[map->Rows()];

		for (s32 j = 0; j < map->Rows(); ++j)
		{
			cells[j] = new HCChaseCell[map->Cols()];
			for (s32 i = 0; i < map->Cols(); ++i)
			{
				cells[j][i].cur = map->Cell(j, i);
				cells[j][i].prev = 0;
				cells[j][i].tried = 0;
				cells[j][i].path.row = j;
				cells[j][i].path.col = i;
				cells[j][i].path.action = 0;
				cells[j][i].visited = false;
				cells[j][i].depth = 0;
				cells[j][i].numJumpedRows = 0;
			}
		}
	}
	else
	{
		// Only zeroes the chase map
		for (s32 j = 0; j < map->Rows(); ++j)
		{
			for (s32 i = 0; i < map->Cols(); ++i)
			{
				cells[j][i].prev = 0;

				// Search optimization
				switch (map->Cell(j, i)->Type())
				{
				case HCCELLTYPE_FLOOR:
				case HCCELLTYPE_CONTFLOOR:
					// Blocks this cell and boundary cells towards this cell
					cells[j][i].tried = HCCHASECELL_CLOSED;
					if (j > 0) cells[j - 1][i].tried |= HCCA_DOWN;
					if (i > 0) cells[j][i - 1].tried |= HCCA_RIGHT;
					if (j < map->Rows()) cells[j + 1][i].tried |= HCCA_UP;
					if (i < map->Cols()) cells[j][i + 1].tried |= HCCA_LEFT;
					break;
					
				case HCCELLTYPE_LADDER:
					if (map->Cell(j + 1, i - 1)->Type() == HCCELLTYPE_BLANK)
					{
						// Can't go left while climbing because there's no floor bellow-left (limitation)
						cells[j][i].tried |= HCCA_LEFT;
					}

					if (map->Cell(j + 1, i - 1)->Type() == HCCELLTYPE_BLANK)
					{
						// Can't go right while climbing because there's no floor bellow-right (limitation)
						cells[j][i].tried |= HCCA_LEFT;
					}
					break;

				case HCCELLTYPE_BAR:
					// Cannot go left or right while sliding
					cells[j][i].tried |= HCCA_LEFT | HCCA_RIGHT;
					break;

				case HCCELLTYPE_BREAK:
					if (((HCBreak *)map->Cell(j, i))->State() == HCBREAKSTATE_NORMAL)
					{
						// Treats it as a floor
						if (j > 0) cells[j - 1][i].tried |= HCCA_DOWN;
						if (i > 0) cells[j][i - 1].tried |= HCCA_RIGHT;
						if (j < map->Rows()) cells[j + 1][i].tried |= HCCA_UP;
						if (i < map->Cols()) cells[j][i + 1].tried |= HCCA_LEFT;
						break;
					}
					break;
					
				default:
					break;
				}

				cells[j][i].path.row = j;
				cells[j][i].path.col = i;
				cells[j][i].path.action = 0;
				cells[j][i].visited = false;
				cells[j][i].depth = 0;
				cells[j][i].numJumpedRows = 0;
			}
		}
	}
}

/** Goes to the given cell. The cell must be in the same row or in the same column.
 * @return 0 if going, 1 if reached, 2 before leaving it (at the edge), -1 if unreachable by simple means.
 */
s32 HCEnemyChaser::GoToSimple(s32 row, s32 col)
{
	s32 curRow = map->ToRow((s32)pos.y), curCol = map->ToCol((s32)pos.x);
	
	if (row != curRow && col != curCol)
	{
		return -1;
	}
	
	if (row != curRow)
	{
		if (row > curRow)
		{
			// Going down, test the cells between
			for (s32 i = curRow; i <= row; ++i)
			{
				if (!(map->Cell(i, curCol)->Actions() & HCACTION_DOWN) &&
						!(map->Cell(i, curCol)->Actions() & HCACTION_FALL))
				{
					return -1;
				}
			}

			actions = HCCA_DOWN;
		}
		else
		{
			// Going up, test the cells between
			for (s32 i = curRow; i >= row; --i)
			{
				if (!(map->Cell(i, curCol)->Actions() & HCACTION_UP))
				{
					return -1;
				}
			}

			actions = HCCA_UP;
		}
	}
	else
  if (col != curCol)
	{
		if (col > curCol)
		{
			// Going right, test the cells between
			for (s32 i = curCol; i <= col; ++i)
			{
				if (!(map->Cell(curRow, i)->Actions() & HCACTION_RIGHT))
				{
					return -1;
				}
			}

			actions = HCCA_RIGHT;
		}
		else
		{
			// Going left, test the cells between
			for (s32 i = curCol; i >= col; --i)
			{
				if (!(map->Cell(curRow, i)->Actions() & HCACTION_LEFT))
				{
					return -1;
				}
			}

			actions = HCCA_LEFT;
		}
	}
	else
	{
		// Reached
		return 1;
	}
	
	nextRow = row;
	nextCol = col;
	
	return 0;
}

void HCEnemyChaser::CheckBestPath()
{
	// Compares the length of each path
	if (chasePathNumCells > tmpPathNumCells)
	{
		//fprintf(stderr, "Mejor path con %d celdas frente a %d\n", tmpPathNumCells, chasePathNumCells);
		// Tmp path is shorter
		memcpy(chasePath, tmpPath, (searchDepth + 1) * sizeof(HCChasePath));
		chasePathNumCells = tmpPathNumCells;
	}
}

s32 HCEnemyChaser::GoTo(s32 row, s32 col)
{
	if (finalRow != row || finalCol != col || chasePathNumCells == (u32)-1)
	{
		s32 curRow = Row(), curCol = Col();

		// New final target cell, computes the best path
		ResetChaseMap();
		HCChaseCell *cc = &cells[curRow][curCol];       // Current chase cell
		HCChaseCell *sc = cc;                           // Search cell
		HCChaseCell *tc = &cells[row][col];             // Target cell

		if (sc == tc)
		{
			return 0;
		}

		HCChaseCell *tmp;                               // Temporary cell
		HCCell *c, bellow;                              // Current map cell and cell bellow
		sc->visited = true;
		
		s32 index = 0;                                  // Index into the search path
		chasePathNumCells = (u32)-1;                    // Init to greatest u32 so it's always worse
		tmpPathNumCells = 0;

		s32 pathNum = 0;
		
		while (1)
		{
			c = sc->cur;
//  			SDL_Rect rc;
//  			rc.x = map->ToX(sc->path.col) - 2;
//  			rc.y = map->ToY(sc->path.row) - (map->CellHeight()/2 - 2);
//  			rc.w = 2;
//  			rc.h = 2;
		
//  			SDL_FillRect(SDL_GetVideoSurface(), &rc, 0xffffffff);
//  			SDL_UpdateRect(SDL_GetVideoSurface(), rc.x, rc.y, rc.w, rc.h);
// 			fprintf(stderr, "Path %d: Estoy en %d %d  index es %d searchDepth es %d ", pathNum, sc->path.row, sc->path.col, index, searchDepth);
// 			fprintf(stderr, 
// 							"ACCIONES PROBADAS:  %s %s %s %s (%s)\n", 
// 							(sc->tried & HCCA_LEFT) ? "L" : " ", 
// 							(sc->tried & HCCA_RIGHT) ? "R" : " ", 
// 							(sc->tried & HCCA_UP) ? "U" : " ", 
// 							(sc->tried & HCCA_DOWN) ? "D" : " ",
// 							sc->visited ? "*** VISITED! ***" : "not visited");
// 			getchar();

			if (sc->depth > searchDepth)
			{
				// Maximum reachable search depth reached!, stop searching in this cell
				sc->tried = HCCHASECELL_CLOSED;
			}

			if (map->Cell(sc->path.row + 1, sc->path.col)->Type() == HCCELLTYPE_BLANK && sc->prev && !(sc->prev->path.action & HCCA_JUMP))
			{
				// Only go by floor, don't jump or fall off in ladders
				sc->tried |= (HCCA_LEFT | HCCA_RIGHT);
			}

			if (!(sc->tried & HCCA_DOWN))
			{
				// Goes to the neighbour bellow
				sc->tried |= HCCA_DOWN;
				tmp = &cells[sc->path.row + 1][sc->path.col];
				
				if (sc->path.row < map->Rows() &&
						((c->Actions() & HCACTION_DOWN) || 
						 (c->Actions() & HCACTION_FALL) || 
						 (map->Cell(sc->path.row + 1, sc->path.col)->Actions() & HCACTION_DOWN)) &&
						!tmp->visited)
				{
					tmp->prev = sc;
					tmpPath[index].row = sc->path.row;
					tmpPath[index].col = sc->path.col;
					tmpPath[index].action = HCCA_DOWN;
					++index;
					sc->visited = true;
					tmp->depth = sc->depth + 1;
					sc = tmp;
					sc->tried |= HCCA_UP;
				}
			}
			else
			if (!(sc->tried & HCCA_LEFT))
			{
				// Goes to the left neighbour
				sc->tried |= HCCA_LEFT;
				tmp = &cells[sc->path.row][sc->path.col - 1];
				
				if ((c->Actions() & HCACTION_LEFT) && sc->path.col > 0 && !tmp->visited)
				{
					tmp->prev = sc;
					tmpPath[index].row = sc->path.row;
					tmpPath[index].col = sc->path.col;
					tmpPath[index].action = HCCA_LEFT;
					++index;
					sc->visited = true;
					tmp->depth = sc->depth + 1;
					sc = tmp;
					sc->tried |= HCCA_RIGHT;
				}
			}
			else
			if (!(sc->tried & HCCA_RIGHT))
			{
				// Goes to the right neighbour
				sc->tried |= HCCA_RIGHT;
				tmp = &cells[sc->path.row][sc->path.col + 1];
				
				if ((c->Actions() & HCACTION_RIGHT) && sc->path.col < map->Cols() && !tmp->visited)
				{
					tmp->prev = sc;
					tmpPath[index].row = sc->path.row;
					tmpPath[index].col = sc->path.col;
					tmpPath[index].action = HCCA_RIGHT;
					++index;
					sc->visited = true;
					tmp->depth = sc->depth + 1;
					sc = tmp;
					sc->tried |= HCCA_LEFT;
				}
			}
			else
			if (!(sc->tried & HCCA_UP))
			{
				// Goes to the upper neighbour
				sc->tried |= HCCA_UP;
				tmp = &cells[sc->path.row - 1][sc->path.col];
				/*				bool cellBellow = sc->path.row < map->Rows();
				HCCell *lower = 0;
				if (cellBellow)
				{
					//lower = map->Cell(sc->path.row + 1, sc->path.col);
					lower = map->Cell(sc->path.row, sc->path.col);
					}*/
				
				if (sc->path.row > 0 && !tmp->visited)
				{
					// Tests up
					if ((c->Actions() & HCACTION_UP)/* ||                       // Current cell allows going up or
																						 (cellBellow && (lower->Actions() & HCACTION_UP))*/)      // lower allows going up (since sprite's hotspot for up is above)
					{
						tmp->prev = sc;
						sc->visited = true;
						tmpPath[index].row = sc->path.row;
						tmpPath[index].col = sc->path.col;
						tmpPath[index].action = HCCA_UP;
						++index;
						tmp->depth = sc->depth + 1;
						sc = tmp;
						sc->tried |= HCCA_DOWN;
					}
				}
			}
			/*
			else
			if (!(sc->tried & HCCA_JUMP))
			{
				// Goes to the upper neighbour
				sc->tried |= HCCA_JUMP;
				tmp = &cells[sc->path.row - 1][sc->path.col];
				bool cellBellow = sc->path.row < map->Rows();
				HCCell *lower = 0;
				if (cellBellow)
				{
					lower = map->Cell(sc->path.row + 1, sc->path.col);
				}
				
				if (sc->path.row > 0 && !tmp->visited)
				{
					// Tests jump
					if ((c->Actions() & HCACTION_FALL) &&                                  // current allows jumping and, or 
							((cellBellow && !(lower->Actions() & HCACTION_FALL)) ||            // there is floor bellow, or 
							 (sc->prev && (sc->prev->action & HCCA_JUMP) && sc->numJumpedRows < maxJumpRows))) // previous action was jump and not reached yet 
					                                                                       // the jump limit (phew!)
					{
						fprintf(stderr, "Salto funciona %d saltadas %d max\n", sc->numJumpedRows, maxJumpRows);
						getchar();
						tmp->prev = sc;
						sc->visited = true;
						tmpPath[index].row = sc->path.row;
						tmpPath[index].col = sc->path.col;
						tmpPath[index].action = HCCA_JUMP;
						++index;
						tmp->depth = sc->depth + 1;
						sc = tmp;
						sc->tried |= HCCA_DOWN;
						
						if (sc->prev)
						{
							sc->numJumpedRows = sc->prev->numJumpedRows + 1;
						}
						else
						{
							sc->numJumpedRows = 1;
						}
					}
				}
				}*/
			else
			{
// 				rc.x = map->ToX(sc->path.col) - 2;
// 				rc.y = map->ToY(sc->path.row) - (map->CellHeight()/2 - 2);
// 				rc.w = 2;
// 				rc.h = 2;
				
// 				SDL_FillRect(SDL_GetVideoSurface(), &rc, 0x00000000);
// 				SDL_UpdateRect(SDL_GetVideoSurface(), rc.x, rc.y, rc.w, rc.h);
// 				getchar();

				// Closed cell
				sc->tried = 0;
				sc->visited = false;
				sc->depth = 0;
				sc->numJumpedRows = 0;
				sc = sc->prev;

				if (index > 0)
					--index;

				if (sc)
				{
					sc->visited = false;
				}
				else
				{
					// Starting cell closed, done
					//fprintf(stderr, "CLOSED!\n");
					break;
				}
			}

			if (sc == tc)
			{
				// The map now contains the search path
				//fprintf(stderr, "Encontrado, index es %d searchDepth es %d!\n", index, searchDepth);
				
				if (sc)
				{
					sc->visited = false;

					if (sc->prev)
					{
						tmpPath[index].row = sc->path.row;
						tmpPath[index].col = sc->path.col;
						tmpPath[index].action = sc->path.action;
					}

					finalRow = row;
					finalCol = col;
				
					++pathNum;
					tmpPathNumCells = index + 1;
					CheckBestPath();
					
					// Continue searching paths
					sc->visited = false;
					sc->depth = 0;
					sc->numJumpedRows = 0;
					sc = sc->prev;
					
					if (sc)
					{
						sc->visited = false;
					}
					
					if (index > 0)
						--index;

					// Searchs ends after param2 successfull paths have been tried
					if (pathNum > param2)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
		}

		if (pathNum != 0)
		{
			chaseIndex = 0;
			return 0;
		}
		else
		{
			//fprintf(stderr, "NO PATH FOUND!\n");
			return -1;
		}
	}

	// OK, going
	return 0;
}

s32 HCEnemyChaser::UpdateChase()
{
	if (chased == 0)
	{
		return 0;
	}

	actions = 0;
	s32 r = map->ToRow((s32)chased->Y()), c = map->ToCol((s32)chased->X());
	s32 curRow = Row(), curCol = Col();
	
	if (chased->State() == HCCS_JUMP ||
			chased->State() == HCCS_JUMPLEFT ||
			chased->State() == HCCS_JUMPRIGHT || 
			chased->State() == HCCS_HANG ||
			(abs(curRow - r) + abs(curCol - c) <= searchDepth && 0 == GoTo(r, c)))
		{
		if ((chasePath[chaseIndex].col != curCol || chasePath[chaseIndex].row != curRow) && chaseIndex < searchDepth - 1)
		{
			// Updates the chase index within the path when the row or column changes
			++chaseIndex;
		}

		if (chasePath[chaseIndex].col == curCol && chasePath[chaseIndex].row == curRow)
		{
			// OK, path hasn't been lost
			actions = chasePath[chaseIndex].action;
			
			// Resets idle movement
			nextRow = curRow;
			nextCol = curCol;
			return 0;
		}
		else
		{
			// Path lost
			chasePathNumCells = (u32)-1;
		}
	}

	// No path to target, moves-simple to the randomly generated cell; if reached, select another one
	if (-1 == GoToSimple(nextRow, nextCol))
	{
		// Selects a random cell within the search depth square and tries to simple move over there,
		// when the chased character is not reachable
		s32 sign;

		switch (rand()%4)
		{
		case 0:
		case 1:
		case 3:
			sign = (rand()%2) == 0 ? 1 : -1;
			r = sign * ((rand()%searchDepth) + 1);
			if (0 != GoToSimple(curRow + r, curCol))
			{
				c = sign * ((rand()%searchDepth) + 1);
				return GoToSimple(curRow, curCol + c);
			}
			break;
			
		case 2:
			sign = (rand()%2) == 0 ? 1 : -1;
			c = sign * (rand()%searchDepth) + 1;
			if (0 != GoToSimple(curRow, curCol + c))
			{
				r = sign * ((rand()%searchDepth) + 1);
				return GoToSimple(curRow + r, curCol);
			}
			break;
		}
	}

	return 0;
}

s32 HCEnemyChaser::Update()
{
	states[state].Update();
	UpdateChase();
	Act(actions);
	
	if (state == HCCS_HANG)
	{
		UpdateHang();
	}
	else
	{
		UpdateVeloccity();
		
		// OJITO, igual se hace pesado!!
		CheckHang();
	}

	// Collisions
	UpdateCollisions();

	Pos(pos.x + v.x, pos.y + v.y);

	// Updates dialogs
	UpdateDialog();

	return 0;
}

void HCEnemyChaser::Destroy()
{
	if (cells)
	{
		for (s32 j = 0; j < map->Rows(); ++j)
		{
			JDELETE_ARRAY(cells[j]);
		}

		// Creates the cell array from the map
		delete[] cells;
	}
	
	JDELETE_ARRAY(chasePath);
	JDELETE_ARRAY(tmpPath);
}
