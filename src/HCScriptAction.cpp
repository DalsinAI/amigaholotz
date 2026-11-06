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

/** Definition of script actions.
 * @file    HCScriptAction.cpp
 * @author  Juan Carlos Seijo Pérez
 * @date    03/07/2004
 * @version 0.0.1 - 03/07/2004 - Primera versión.
 */

#include <HCScriptAction.h>
#include <HolotzCastle.h>

s32 HCScriptAction::Update()
{
	finished = true;
	return 0;
}

HCCharacter * HCScriptAction::Character(const char *name, s32 index)
{
	HCApp *app = (HCApp *)JApp::App();
	
	if (0 == strcmp(name, "main"))
	{
		return app->Level()->Character();
	}
	else
	if (0 == strcmp(name, "enemy"))
	{
		if (index >= app->Level()->NumEnemies())
		{
			fprintf(stderr, "No such enemy: %d\n", index);
		}
		
		return app->Level()->Enemies()[index];
	}
	else
	{
		fprintf(stderr, 
						"HCScriptAction: Unknown character %s!\n"
						"Available are main and enemy\n", name);
	}

	return 0;
}

HCScriptAction * HCScriptAction::Load(JTextFile &f)
{
	char str[1024];

	if (f.FindNext("["))
	{
		s8 * start = f.GetPos();
		
		if (f.SkipNextWord())
		{
			if (f.ReadWord(str))
			{
				if (strcmp(str, "MOVE") == 0)
				{
					// Move action
					HCScriptActionMove *action = new HCScriptActionMove;
					f.SetPos(start);

					if (action->Load(f))
					{
						f.FindNext("]");

						return action;
					}
				}
				else
				if (strcmp(str, "DIALOG") == 0)
				{
					// Dialog action
					HCScriptActionDialog *action = new HCScriptActionDialog;
					f.SetPos(start);

					if (action->Load(f))
					{
						f.FindNext("]");

						return action;
					}
				}
				else
				if (strcmp(str, "NARRATIVE") == 0)
				{
					// Narrative action
					HCScriptActionNarrative *action = new HCScriptActionNarrative;
					f.SetPos(start);

					if (action->Load(f))
					{
						f.FindNext("]");

						return action;
					}
				}
				else
				if (strcmp(str, "SOUND") == 0)
				{
					// Sound action
					HCScriptActionSound *action = new HCScriptActionSound;
					f.SetPos(start);

					if (action->Load(f))
					{
						f.FindNext("]");
						
						return action;
					}
				}
				else
				if (strcmp(str, "WAIT") == 0)
				{
					// Wait action
					HCScriptActionWait *action = new HCScriptActionWait;
					f.SetPos(start);

					if (action->Load(f))
					{
						f.FindNext("]");
						
						return action;
					}
				}
				else
				{
					// Unknown action
					fprintf(stderr, "Unknown action found!\n");
					
					return 0;
				}
			}
		}
	}
	else
	{
		fprintf(stderr, "HCScriptAction: No action found!\n");
	}

	return 0;
}

bool HCScriptActionMove::Init(HCCharacter *_character, s32 dir, s32 amount)
{
	if (0 == _character)
	{
		return false;
	}

	character = _character;
	totalAmount = amount;
	direction = dir;

	return true;
}

bool HCScriptActionMove::Load(JTextFile &f)
{
	char charName[32];
	s32 index;

	if (4 == sscanf(f.GetPos(), "[ MOVE %[A-Za-z] ( %d ) dir=%d amount=%d ]", 
									charName, &index, &direction, &totalAmount))
	{
		HCCharacter *tmpChar;

		tmpChar = Character(charName, index);
		
		if (0 == tmpChar)
		{
			return false;
		}
		
		if (direction != 2 && (direction < 4 || direction > 9))
		{
			fprintf(stderr, "HCScriptActionMove: Direction must be one of 2 or 4 to 9!\n");
			
			return false;
		}

		return Init(tmpChar, direction, totalAmount);
	}

	fprintf(stderr, "HCScriptActionMove: Failed to load!\n");

	return false;
}

s32 HCScriptActionMove::Update()
{
	if (finished)
	{
		return 0;
	}

	// Finished if not moving leftright or updown.
	switch (direction)
	{
	case 2:	
		character->Act(HCCA_DOWN);
		if (character->State() != HCCS_DOWN)  
		{
			character->State(HCCS_STOP);
			character->Veloccity().y = 0;
			finished = true; 
			return 0;
		}
		break;

	case 4:	
		character->Act(HCCA_LEFT);
		if (character->Acceleration().x > -0.001f/* || character->State() != HCCS_LEFT*/)
		{
			character->Veloccity().x = character->Acceleration().x = 0;
			character->State(HCCS_STOP);
			finished = true; 
			return 0;
		}
		break;

	case 5:	
		character->Act(HCCA_JUMP);
		if (character->Veloccity().y > 0.0f || character->State() != HCCS_JUMP)
		{
			finished = true; 
			return 0;
		}
		break;

	case 6:	
		character->Act(HCCA_RIGHT);
		if (character->Acceleration().x < 0.001f /*|| character->State() != HCCS_RIGHT*/) 
		{
			character->Veloccity().x = character->Acceleration().x = 0;
			character->State(HCCS_STOP);
			finished = true; 
			return 0;
		}
		break;

	case 7:	
		character->Act(HCCA_JUMP | HCCA_LEFT);
		if (character->Acceleration().x > -0.001f || character->State() != HCCS_JUMPLEFT)
		{
			finished = true; 
			return 0;
		}
		break;

	case 8:	
		character->Act(HCCA_UP);
		if (character->State() != HCCS_UP)
		{
			character->Veloccity().y = 0;
			character->State(HCCS_STOP);
			finished = true; 
			return 0;
		}
		break;

	case 9:	
		character->Act(HCCA_JUMP | HCCA_RIGHT);
		if (character->Acceleration().x < 0.001f || character->State() != HCCS_JUMPRIGHT)
		{
			finished = true; 
			return 0;
		}
		break;

	default:
		character->Veloccity().y = 0;
		character->Veloccity().x = character->Acceleration().x = 0;
		character->Act(HCCA_STOP);
		break;
	}

	s32 row = character->Row();
	s32 col = character->Col();
	
	// Cell changed?
	if (row != lastRow)
	{
		lastRow = row;
		switch (direction)
		{
			// Only counts for up and down actions
		case 8:
		case 2:
			++curAmount;
			break;
			
		default:
			break;
		}
	}

	if (col != lastCol)
	{
		// This governs also the jumpxxx actions
		lastCol = col;
		++curAmount;
	}

	// Travelled the specified amount of cells
	if (curAmount >= totalAmount)
	{
		finished = true;
	}

	if (finished)
	{
		switch (direction)
		{
		case 5:
			character->Act(HCCA_JUMP);
			break;
			
		default:
			character->Act(HCCA_STOP);
			break;
		}

		direction = 0;
		return 0;
	}

	return 1;
}

void HCScriptActionMove::Current()
{
	lastRow = orgRow = character->Row();
	lastCol = orgCol = character->Col();
	curAmount = 0;
}

bool HCScriptActionDialog::Init(HCCharacter *_character, 
																const char *text, 
																HCTheme *theme, 
																JFont *font,
																JFontAlign align,
																bool left,
																s32 subtype,
																u8 r, u8 g, u8 b)
{
	if (0 == _character || 0 == theme || 0 == font)
	{
		return false;
	}

	character = _character;
	if (!dialog.Init(HCTEXTTYPE_DIALOG, text, theme, font, align, left, subtype, r, g, b))
	{
		fprintf(stderr, "Couldn't initialize dialog action.\n");
		return false;
	}
	
	float *x = (float*)&((character->JDrawable::Pos()).x);
	float *y = (float*)&((character->JDrawable::Pos()).y);
	
	dialog.Track(x, y);
	if (left)
		dialog.Pos(character->CurSprite()->MaxW()/2, -character->CurSprite()->MaxH()/2);
	else
		dialog.Pos(-character->CurSprite()->MaxH()/2, -character->CurSprite()->MaxH()/2);
		

	return true;
}

bool HCScriptActionDialog::Load(JTextFile &f)
{
	char charName[32];
	char text[256];
	s32 index, left, speed, r, g, b, size, txtAlign, st;
	JFontAlign fontAlign;

	memset(text, 0, sizeof(text));
	memset(charName, 0, sizeof(charName));

	if (11 == sscanf(f.GetPos(), "[ DIALOG %[A-Za-z](%d) text=\"%[^\"]\" txtAlign=%d speed=%d size=%d align=%d r=%d g=%d b=%d subtype=%d ]", charName, &index, text, &txtAlign, &speed, &size, &left, &r, &g, &b, &st))
	{
		HCApp *app = (HCApp*)JApp::App();
		HCCharacter *tmpChar = Character(charName, index);

		JClamp(size, 1, 3);
		JFont *fnt;
		
		switch (size)
		{
		default:
		case 1: fnt = app->FontSmall(); break;
		case 2: fnt = app->FontMedium(); break;
		case 3: fnt = app->FontLarge(); break;
		}
		
		s32 oldSpeed = HCText::Speed();

		switch (txtAlign % 3)
		{
		case 0:
			fontAlign = JFONTALIGN_RIGHT;
			break;
    default:
		case 1:
			fontAlign = JFONTALIGN_LEFT;
			break;
		case 2:
			fontAlign = JFONTALIGN_CENTER;
			break;
		}

		Init(tmpChar, text, &app->Level()->Theme(), fnt, fontAlign, left != 0, st, r, g, b);

		HCText::Speed(oldSpeed);
	}
	else
	{
		fprintf(stderr, "HCScriptActionDialog: Failed to load!\n");

		return false;
	}

	return true;
}

s32 HCScriptActionDialog::Update()
{
	if (!finished)
	{
		if (!dialog.Visible())
		{
			finished = true;
		}

		return 1;
	}

	return 0;
}

void HCScriptActionDialog::Current()
{
	dialog.Reset();
	character->Dialog(&dialog);
}

bool HCScriptActionNarrative::Init(s32 alignment, 
																	 const char *text, 
																	 HCTheme *theme, 
																	 JFont *font,
																	 JFontAlign align,
																	 s32 subtype,
																	 u8 r, u8 g, u8 b)
{
	if (0 == theme || 0 == font)
	{
		return false;
	}

	if (!narrative.Init(HCTEXTTYPE_NARRATIVE, text, theme, font, align, false, subtype, r, g, b))
	{
		fprintf(stderr, "Couldn't initialize narrative action.\n");
		return false;
	}

	// Aligns the frame within the screen
	s32 x, y;
	switch (alignment)
	{
	case 1:
		x = 0;
		y = JApp::App()->Height() - narrative.Image().Height();
		break;
	case 2:
		x = (JApp::App()->Width() - narrative.Image().Width())/2;
		y = JApp::App()->Height() - narrative.Image().Height();
		break;
	case 3:
		x = JApp::App()->Width() - narrative.Image().Width();
		y = JApp::App()->Height() - narrative.Image().Height();
		break;
	case 4:
		x = 0;
		y = (JApp::App()->Height() - narrative.Image().Height())/2;
		break;
	case 5:
		x = (JApp::App()->Width() - narrative.Image().Width())/2;
		y = (JApp::App()->Height() - narrative.Image().Height())/2;
		break;
	case 6:
		x = JApp::App()->Width() - narrative.Image().Width();
		y = (JApp::App()->Height() - narrative.Image().Height())/2;
		break;
	default:
	case 7:
		x = 0;
		y = 0;
		break;
	case 8:
		x = (JApp::App()->Width() - narrative.Image().Width())/2;
		y = 0;
		break;
	case 9:
		x = JApp::App()->Width() - narrative.Image().Width();
		y = 0;
		break;
	}

	narrative.Pos(x, y);
	
	return true;
}

bool HCScriptActionNarrative::Load(JTextFile &f)
{
	char text[256];
	s32 align, speed, r, g, b, size, txtAlign, st;
	JFontAlign fontAlign;
	memset(text, 0, sizeof(text));

	if (9 == sscanf(f.GetPos(), 
									"[ NARRATIVE text=\"%[^\"]\" txtAlign=%d speed=%d size=%d align=%d r=%d g=%d b=%d subtype=%d ]", 
									text, &txtAlign, &speed, &size, &align, &r, &g, &b, &st))
	{
		HCApp *app = (HCApp*)JApp::App();

		JClamp(size, 1, 3);
		JFont *fnt;
		
		switch (size)
		{
		default:
		case 1: fnt = app->FontSmall(); break;
		case 2: fnt = app->FontMedium(); break;
		case 3: fnt = app->FontLarge(); break;
		}
		
		s32 oldSpeed = HCText::Speed();

		switch (txtAlign % 3)
		{
		case 0:
			fontAlign = JFONTALIGN_RIGHT;
			break;
    default:
		case 1:
			fontAlign = JFONTALIGN_LEFT;
			break;
		case 2:
			fontAlign = JFONTALIGN_CENTER;
			break;
		}

		Init(align, text, &app->Level()->Theme(), fnt, fontAlign, st, r, g, b);

		HCText::Speed(oldSpeed);
	}
	else
	{
		fprintf(stderr, "HCScriptActionNarrative: Failed to load!\n");

		return false;
	}

	return true;
}

s32 HCScriptActionNarrative::Update()
{
	if (!finished)
	{
		if (!narrative.Visible())
		{
			finished = true;
		}

		return 1;
	}

	return 0;
}

void HCScriptActionNarrative::Current()
{
	narrative.Reset();
	((HCApp*)JApp::App())->Level()->Narrative(&narrative);
}

bool HCScriptActionSound::Init(const char *filename, s32 loops, bool waitToEnd)
{
	numLoops = loops;
	
	if (!waitToEnd)
	{
		finished = true;
	}
	
	if (JApp::App()->SoundEnabled())
	{
		return (sound.LoadWave(filename));
	}
	else
	{
		return true;
	}
	
	return false;
}

void HCScriptActionSound::Current()
{
	if (JApp::App()->SoundEnabled())
	{
		sound.Play(-1, numLoops);
	}
}

bool HCScriptActionSound::Load(JTextFile &f)
{
	s32 waitToEnd, loops;
  char file[1024];

	if (3 == sscanf(f.GetPos(), "[ SOUND file=\"%[^\"]\" loops=%d waitToEnd=%d ]", 
									file, &loops, &waitToEnd))
	{
		char filename[1024];
		snprintf(filename, sizeof(filename), "%s%s", HC_DATA_DIR, file);
		return Init(filename, loops, waitToEnd != 0);
	}

	return false;
}

s32 HCScriptActionSound::Update()
{
	if (!finished)
	{
		if (!sound.IsPlaying())
		{
			finished = true;
		}
	}

	return 0;
}

void HCScriptActionWait::Current()
{
	timer.Start(ms);
}

bool HCScriptActionWait::Init(s32 millis)
{
	ms = millis;

	return true;
}

bool HCScriptActionWait::Load(JTextFile &f)
{
	s32 millis;

	if (1 == sscanf(f.GetPos(), "[ WAIT millis=%d ]", &millis))
	{
		return Init(millis);
	}

	return false;
}

s32 HCScriptActionWait::Update()
{
	if (!finished)
	{
		if (timer.Changed())
		{
			finished = true;
		}
	}

	return 0;
}
