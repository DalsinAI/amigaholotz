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
 * @file    HCScript.h
 * @author  Juan Carlos Seijo Pérez
 * @date    03/07/2004
 * @version 0.0.1 - 03/07/2004 - Primera versión.
 */

#ifndef _HCSCRIPT_INCLUDED
#define _HCSCRIPT_INCLUDED

#include <JLib/Util/JTextFile.h>
#include <HCScriptBlock.h>
#include <HCLevel.h>

class HCScriptBlock;

class HCScript
{
 protected:
	HCScriptBlock *blocks;                /**< Execution blocks. */
	s32 numBlocks;                        /**< Number of execution blocks. */
	s32 curBlock;                         /**< Current block index. */
	HCLevel *level;                       /**< Level for this script. */

 public:
	/** Creates an empty script. Init and Load must be called in order to begin to use it.
	 */
	HCScript() : blocks(0), numBlocks(0), curBlock(0), level(0)
	{}

	/** Initializes this script.
	 * @param  _level Associated level.
	 * @return <b>true</b> if succeeded, <b>false</b> if not.
	 */
	bool Init(HCLevel *_level);

	/** Loads a script.
	 * @param  filename Name of the file containing the script.
	 * @return <b>true</b> if succeeded, <b>false</b> otherwise.
	 */
	bool Load(const char *filename);

	/** Skips the dialog actions in the current block.
	 */
  void Skip() {if (blocks) blocks[curBlock].Skip();}

	/** Updates this script. Checks for block termination and procceeds with
	 * the next block if so.
	 * @return 0 if the execution block hasn't changed, 1 if changed, -1 if the
	 * script has finished.
	 */
	s32 Update();

	/** Checks if the script has finished its execution.
	 * @return <b>true<b> if it has finished, <b>false<b> otherwise.
	 */
	bool Finished();
	
	/** Destroys the object.
	 */
	void Destroy() {JDELETE_ARRAY(blocks); numBlocks = curBlock = 0; level = 0;}
	
	/** Destroys the object.
	 */
	virtual ~HCScript() {Destroy();}
};

#endif // _HCSCRIPT_INCLUDED
