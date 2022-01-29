/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Ending Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
double scoreDeathCount = 0.0;
double scoreTime = 0.0;
double scoreKicks = 0.0;

//----------------------------------------------------------------------------------
// Ending Screen Functions Definition
//----------------------------------------------------------------------------------

// Ending Screen Initialization logic
void InitEndingScreen(void)
{
    // TODO: Initialize ENDING screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

// Ending Screen Update logic
void UpdateEndingScreen(void)
{
}

// Ending Screen Draw logic
void DrawEndingScreen(void)
{
    // TODO: Draw ENDING screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    Vector2 pos = { 20, 20 };
    DrawTextEx(font, TextFormat("DEATHS: %d", (int)scoreDeathCount), pos, font.baseSize*3, 3, DARKRED);
    pos.y += font.baseSize*3+10;
    DrawTextEx(font, TextFormat("TIME: %d seconds", (int)scoreTime), pos, font.baseSize*3, 3, DARKBLUE);
    pos.y += font.baseSize*3+10;
    DrawTextEx(font, TextFormat("KICKS: %d", (int)scoreKicks), pos, font.baseSize*3, 3, GOLD);
    pos.y += font.baseSize*3+10;
    DrawTextEx(font, TextFormat("SCORE: %d (more is better)", (int)((scoreKicks/(scoreTime*scoreDeathCount))*10000)), pos, font.baseSize*3, 3, RAYWHITE);
}

// Ending Screen Unload logic
void UnloadEndingScreen(void)
{
    // TODO: Unload ENDING screen variables here!
}

// Ending Screen should finish?
int FinishEndingScreen(void)
{
    return finishScreen;
}
