/*
 *  JLib - Jacob's Library.
 *  Copyright (C) 2003, 2004  Juan Carlos Seijo PÈrez
 * 
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 * 
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 * 
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 *  Juan Carlos Seijo PÈrez
 *  jacob@mainreactor.net
 */

/** Clase base de todos los objetos.
 * @file    JObject.cpp
 * @author  Juan Carlos Seijo P√©rez
 * @date    15/11/2003
 * @version 0.0.1 - 15/11/2003 - Primera versi√≥n
 */

#include <JLib/Util/JObject.h>

#ifdef _JLIB_DEBUG
#include <SDL/SDL.h>
#include <stdio.h>

s32 JObject::instanceCount;
s32 JObject::instanceID;
std::vector<JObject *> JObject::objects;

JObject::JObject() 
{
	objects.push_back(this);
	++instanceCount;
  instanceNum = ++instanceID;
  fprintf(stderr, "#%d Created: 0x%x. JObjects: %d.\n", instanceNum, this, instanceCount);
}

JObject::~JObject()
{
	--instanceCount;
  fprintf(stderr, 
					"#%d Destroyed: 0x%x. Remaining JObjects: %d - %s", 
					instanceNum, this, instanceCount, instanceCount == 0 ? "NO LEAKS!" : "");
	
	s32 first = 0, last;
	first = (*objects.begin())->instanceNum;
	last = first + 1;

	for (std::vector<JObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if (*it == this)
		{
			fprintf(stderr, "*** %d *** ", (*it)->instanceNum);
			objects.erase(it);
			--it;
		}
		else
		{
			// Calculates ranges of instance numbers so the output is not too big
			if (last == (*it)->instanceNum)
			{
				// Correlatives: only increments the range
				++last;
			}
			else
			{
				// Not correlatives, prints the range and jumps to the next
				fprintf(stderr, "[%d-%d] ", first, last - 1);
				first = (*it)->instanceNum;
				last = first + 1;
			}
		}
	}

	if (first != last - 1)
		fprintf(stderr, "[%d-%d] ", first, last - 1);
	
	fprintf(stderr, "\n");
}

#endif // _JLIB_DEBUG
