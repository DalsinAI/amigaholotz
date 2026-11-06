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

/** Script engine for Holotz's Castle.
 * @file    HCScript.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    03/07/2004
 * @version 0.0.1 - 03/07/2004 - Primera versión.
 */

#include <HCScript.h>

bool HCScript::Init(HCLevel *_level)
{
	if (!_level)
	{
		return false;
	}
	
	level = _level;

	return true;
}

bool HCScript::Load(const char *filename)
{
	JTextFile f;
	
	if (!f.Load(filename, "rt"))
	{
		return false;
	}

	// Creates the necessary blocks
	numBlocks = f.CountString("{");
	
	if (numBlocks == 0)
	{
		fprintf(stderr, "HCScript error: No execution blocks.\n");
		return false;
	}

	f.StartOfDocument();

	if (numBlocks != (s32)f.CountString("}"))
	{
		fprintf(stderr, "HCScript error: Mismatched braces!\n");
		return false;
	}

	blocks = new HCScriptBlock[numBlocks];
	
	// Process each block
	s8 *begin, *end; 

	f.StartOfDocument();

	for (s32 i = 0; i < numBlocks; ++i)
	{
		f.FindNext("{");
		
		begin = f.GetPos();
		f.FindNext("}");
		end = f.GetPos();

		f.SetPos(begin);

		// Load the blocks
		blocks[i].Load(f);
	}

	// Start!
	blocks[0].Current();

	return true;
}

s32 HCScript::Update()
{
	if (curBlock == numBlocks)
	{
		return -1;
	}
	
	s32 ret = blocks[curBlock].Update();

	if (blocks[curBlock].Finished())
	{
		++curBlock;
		
		if (curBlock == numBlocks)
		{
			return -1;
		}

		// Prepares the block for execution
		blocks[curBlock].Current();
	}

	return ret;
}

bool HCScript::Finished() 
{
	return curBlock == numBlocks;
}
