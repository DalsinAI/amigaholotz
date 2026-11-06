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
 * @file    HCScriptBlock.h
 * @author  Juan Carlos Seijo Pérez
 * @date    03/07/2004
 * @version 0.0.1 - 03/07/2004 - Primera versión.
 */

#ifndef _HCSCRIPTBLOCK_INCLUDED
#define _HCSCRIPTBLOCK_INCLUDED

#include <JLib/Util/JObject.h>
#include <HCScriptAction.h>

class HCScriptAction;

class HCScriptBlock
{
 protected:
	HCScriptAction **actions;             /**< Actions of this block. */
	s32 numActions;                       /**< Number of actions of this block. */
	
 public:
	/** Creates an empty block of actions.
	 */
	HCScriptBlock() : actions(0), numActions(0)
	{}

	/** Checks if the block has finished its execution.
	 * @return <b>true<b> if it has finished, <b>false<b> otherwise.
	 */
	bool Finished();
	
	/** Updates the actions of this block;
	 * @return 0 if no changes had been made, 1 otherwise.
	 */
	s32 Update();

	/** Loads the block from the file. If the method encounters '{ ... }', it will
	 * search for valid actions in the '...' section.
	 * @param  f File already opened and positioned before the block to read.
	 * @return <b>true</b> if successfull, <b>false</b> otherwise.
	 */
	bool Load(JTextFile &f);

	/** Skips the dialog actions in this block.
	 */
  void Skip();

	/** Prepares the block for execution. The script calls this method when its time 
	 * to execute the block. The block prepares its actions to be executed.
	 */
	virtual void Current();

	/** Destroys the block and frees resources.
	 */
	virtual ~HCScriptBlock() {JDELETE_POINTER_ARRAY(actions, numActions);}
};

#endif // _HCSCRIPTBLOCK_INCLUDED
