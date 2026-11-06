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

/** Definition of a generic enemy.
 * @file    HCEnemy.h
 * @author  Juan Carlos Seijo Pérez
 * @date    27/05/2004
 * @version 0.0.1 - 27/05/2004 - First version.
 */

#include <HCEnemy.h>

float HCEnemy::maxXVeloccity = 3.0f;
float HCEnemy::maxYVeloccity = 3.0f;

bool HCEnemy::Init(JImageSprite *sprites, HCMap *_map, HCRope **_ropes, s32 nRopes)
{
	if (HCCharacter::Init(sprites, _map, _ropes, nRopes))
	{
		v.x = 0;

		// Uses param1 to set veloccities for this enemy, depends also on difficulty
		// Easy - from 0 to 62.5% MaxV stepping (62.5% MaxV)/10 (10 is the number of possible param1)
		// Normal - from 0 to 87.5% MaxV stepping (87.5% MaxV)/10
		// Hard - from 0 to MaxV stepping MaxV/10
		// i.e.: if maxXVeloccity is 5 and param1 is 1, enemy at EASY level moves with vx = 0.325 pels/sec, at HARD with vx=0.5

		float delta, percent;
		percent = 1.0f - (float(HCPreferences::Prefs()->Difficulty() - HCPREFERENCES_HARD)/(2.0f * float(HCPREFERENCES_EASY)));

		delta = (maxXVeloccity * percent)/10.0f;
		vMax.x = JMax(HCENEMY_MIN_MAX_V, delta * float(param1));
		aMax.x = vMax.x;
		
		delta = (maxYVeloccity * percent)/10.0f;
		aMax.y = vMax.y = JMax(HCENEMY_MIN_MAX_V, delta * float(param1));
		
		return true;
	}

	return false;
}

s32 HCEnemy::Update()
{
	Act(actions);

	s32 ret = states[state].Update();

	UpdateVeloccity();
	UpdateCollisions();

	// Only enters here if the sprite has changed
	switch (state)
	{
	case HCCS_STOP:
		UpdateStop();
		break;

	case HCCS_RIGHT:
		UpdateRight();
		break;

	case HCCS_LEFT:
		UpdateLeft();
		break;

	case HCCS_UP:
		UpdateUp();
		break;

	case HCCS_SLIDE:
		UpdateSlide();
		break;

	case HCCS_DOWN:
		UpdateDown();
		break;

	case HCCS_JUMPLEFT:
		UpdateJumpLeft();
		break;
	case HCCS_JUMPRIGHT:
		UpdateJumpRight();
		break;

	case HCCS_FALL:
		UpdateFall();
		break;

	case HCCS_JUMP:
		UpdateJump();
		break;

	case HCCS_DIE:
		UpdateDie();
		break;

	case HCCS_HANG:
		UpdateHang();
		break;

	default:
		break;
	}

	Pos(pos.x + v.x, pos.y + v.y);

	return ret;
}

u32 HCEnemy::Load(JRW &file, HCTheme &theme, HCMap *_map)
{
	if (0 == file.ReadLE32((s32 *)&type) ||
			0 == file.ReadLE32(&param1) ||
			0 == file.ReadLE32(&param2) ||
			0 != HCCharacter::Load(file))
	{
		fprintf(stderr, "Error reading enemy's common parameters.\n");
		return 1;
	}

	bool ret = false;

	switch (type)
	{
	default:
	case HCENEMYTYPE_BALL:
		ret = Init(theme.Ball(subtype), _map);
		break;

	case HCENEMYTYPE_RANDOM:
		ret = Init(theme.Random(subtype), _map);
		break;

	case HCENEMYTYPE_STATIC:
		ret = Init(theme.Static(subtype), _map);
		break;

	case HCENEMYTYPE_MAKER:
		ret = Init(theme.Maker(subtype), _map);
		break;

	case HCENEMYTYPE_CHASER:
		ret = Init(theme.Chaser(subtype), _map);
		break;
	}

	if (!ret)
	{
		return 2;
	}

	// Adjusts the sprite's framerate according to 1st param
	for (s32 i = 0; i < HCCS_COUNT; ++i)
	{
		states[i].FPS(states[i].FPS() + param1 - 5);
	}

	return 0;
}

u32 HCEnemy::Save(JRW &file)
{
	if (0 == file.WriteLE32((s32 *)&type) ||
			0 == file.WriteLE32(&param1) ||
			0 == file.WriteLE32(&param2) ||
			0 != HCCharacter::Save(file))
	{
		fprintf(stderr, "Error writing enemy's common parameters.\n");
		return 1;
	}

	return 0;
}

