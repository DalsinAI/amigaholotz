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

/** Execution block for Holotz's Castle's script engine.
 * @file    HCScriptBlock.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    03/07/2004
 * @version 0.0.1 - 03/07/2004 - Primera versión.
 */

#include <HCScriptBlock.h>

bool HCScriptBlock::Finished()
{
	bool finished = true;

	for (s32 i = 0; finished && i < numActions; ++i)
	{
		finished = actions[i]->Finished();
	}
	
	return finished;
}

s32 HCScriptBlock::Update()
{
	s32 ret = 0;

	for (s32 i = 0; i < numActions; ++i)
	{
		if (!actions[i]->Finished())
		{
			ret |= actions[i]->Update();
		}
	}
	
	return ret;
}

bool HCScriptBlock::Load(JTextFile &f)
{
	// Find the beginning of the block
	if (f.FindNext("{"))
	{
		s8 * start = f.GetPos();
		
		// Find the end of the block
		if (f.FindNext("}"))
		{
			s8 * end = f.GetPos();
			f.SetPos(start);
			JDELETE_POINTER_ARRAY(actions, numActions);
			numActions = 0;
			
			// Counts the number of actions in the block
			while (f.FindNext("[") && f.GetPos() < end)
			{
				f.SkipNextWord();
				++numActions;
			}

			f.SetPos(start);

			if (numActions == 0)
			{
				fprintf(stderr, "No actions in block!\n");
				return false;
			}

			// Creates the action array
		  actions = new HCScriptAction* [numActions];
			
			for (s32 i = 0; i < numActions; ++i)
			{
				f.FindNext("[");
				actions[i] = HCScriptAction::Load(f);
			}
		}
		else
		{
			fprintf(stderr, "Mismatched block brace!\n");
		}
	}
	else
	{
		fprintf(stderr, "No block to load!\n");
	}

	return false;
}

void HCScriptBlock::Skip()
{
	for (s32 i = 0; i < numActions; ++i)
	{
		if (actions[i]->type == HCSAT_DIALOG)
		{
			actions[i]->Skip();
		}
	}
}

void HCScriptBlock::Current()
{
	for (s32 i = 0; i < numActions; ++i)
	{
		actions[i]->Current();
	}
}

