/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
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
#include "timer.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct {
    // Relative to the play field, starts from top left corner
    Vector2 position;
} Player;

typedef struct {
    double tensionPercentage;
} TensionBar;

typedef enum {
    ENT_MISSINGNO,
    ENT_ARROW_NEXT,
} EntityArt;

typedef enum {
    ENTB_IDLE,
    // (Move to next stage), preferrably used with ENT_ARROW_NEXT
    ENTB_NEXT_LEVEL,
    ENTB_FOLLOW_LOCK_TARGET,
} EntityBehaviour;

typedef struct {
    bool alive;
    double angle;
    int group_id;
    Vector2 locked_target;
    EntityBehaviour behaviour;
    EntityArt art;
    Rectangle rect;
} Entity;

#define MAX_ENTS 1024

typedef struct {
    const char *name;
    Rectangle rect;
    Entity entities[MAX_ENTS];
} PlayField;

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static Player playerState = { 0 };
static TensionBar tensionBar = { 0.5 };
static PlayField playField = { 0 };
static Timer testEntitySummonTimer = { .treshold = 3 };

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

static Entity* SummonEntity(PlayField *field) 
{
    for (int i = 0; i < MAX_ENTS; i += 1) {
        if (!field->entities[i].alive) {
            field->entities[i] = (Entity) { 0 };
            field->entities[i].alive = true;
            return &field->entities[i];
        }
    }

    // Can't allocate
    return NULL;
}

Rectangle TransformRectanglePlayField(PlayField *field, Rectangle rect) 
{
    return (Rectangle) { rect.x + field->rect.x, rect.y + field->rect.y, rect.width, rect.height };
}

// Summons the next room arrow on the right of the room
void SummonNextRoomArrow(PlayField *field)
{
    int w = 30, h = 80, x = field->rect.width - w, y = (field->rect.height - h) / 2;
    Entity *ent = SummonEntity(field);
    ent->art = ENT_ARROW_NEXT;
    ent->rect = (Rectangle) { x, y, w, h };
}

void SummonRandomEntity(PlayField *field)
{
    Entity *ent = SummonEntity(field);
    ent->rect.x = rand()%(int)GetScreenWidth()-field->rect.x;
    ent->rect.y = rand()%(int)GetScreenHeight()-field->rect.y;
    ent->rect.width = 30;
    ent->rect.height = 30;
}

int CenterAtX(int w) 
{
    return (GetScreenWidth()-w)/2;
}

int CenterAtY(int h) 
{
    return (GetScreenHeight()-h)/2;
}

Rectangle MakePlayFieldRectangle() 
{
    return (Rectangle) { CenterAtX(256), CenterAtY(256), 256, 256 };
}

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
 
    playField.name = "ATTENTION";
    playField.rect = MakePlayFieldRectangle();
    SummonNextRoomArrow(&playField);
}

double Clamp(double low, double value, double hi)
{
    if (value < low)
        return low;
    if (value > hi)
        return hi;
    return value;
}

void UpdatePlayer(Player *player, int playFieldWidth, int playFieldHeight) 
{
    const int PLAYER_SPEED = 3;
    const int PLAYER_SIZE  = 20;

    if (IsKeyDown(KEY_LEFT))
        player->position.x -= PLAYER_SPEED;
    if (IsKeyDown(KEY_RIGHT))
        player->position.x += PLAYER_SPEED;
    if (IsKeyDown(KEY_UP))
        player->position.y -= PLAYER_SPEED;
    if (IsKeyDown(KEY_DOWN))
        player->position.y += PLAYER_SPEED;

    player->position.x = Clamp(0, player->position.x, playFieldWidth-PLAYER_SIZE);
    player->position.y = Clamp(0, player->position.y, playFieldHeight-PLAYER_SIZE);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    UpdatePlayer(&playerState, 256, 256);
    playField.rect = MakePlayFieldRectangle();
    if (UpdateTimer(&testEntitySummonTimer)) {
        SummonRandomEntity(&playField);
    }
}

void DrawPlayField(PlayField *field) 
{
    int x = field->rect.x, y = field->rect.y, width = field->rect.width, height = field->rect.height;
    DrawRectangle(x-4, y-4, width+8, height+8, RAYWHITE);
    DrawRectangle(x, y, width, height, BLACK);
}

// X, Y specify play field offset
void DrawPlayer(Player *player, PlayField *field) 
{
    int x = field->rect.x, y = field->rect.y;
    DrawRectangle(player->position.x+x, player->position.y+y, 20, 20, DARKRED);
    // Draw tension rectangle
    DrawRectangleLinesEx((Rectangle) {player->position.x+x-10, player->position.y+y-10, 40, 40}, 1, ColorAlpha(WHITE, 0.2));
}

void DrawTensionBar(TensionBar *bar, PlayField *field) 
{
    int x = field->rect.x + 256 + 12, y = field->rect.y, h = field->rect.height;
    int actualHeight = h*bar->tensionPercentage;
    Color barColor = ColorFromHSV(bar->tensionPercentage*60.0, 1.0, 0.8);
    DrawRectangle(x, y+(h-actualHeight)-4, 4, actualHeight+8, barColor);
}

Rectangle ExpandRectangleFromCenter(int x, int y, int radius) 
{
    return (Rectangle) { x - radius, y - radius, radius, radius };
}

void DrawEntityCollider(PlayField *field, Entity *entity)
{
    Rectangle destination = TransformRectanglePlayField(field, entity->rect);

    DrawRectangleRec(destination, ColorAlpha(BLUE, 0.3));
}

void DrawEntity(PlayField *field, Entity *entity) 
{
    Rectangle destination = TransformRectanglePlayField(field, entity->rect);

    switch (entity->art) {
    case ENT_MISSINGNO:
        static int counter = 0;
        srand(counter/200);
        for (int i = 0; i < 10; i += 1) {
            int randx = rand()%(int)destination.width+destination.x; 
            int randy = rand()%(int)destination.width+destination.y; 
            int rands = rand()%20; 

            DrawRectangleRec(ExpandRectangleFromCenter(randx, randy, rands), RAYWHITE);
        }
        counter += 1;
        break;
    case ENT_ARROW_NEXT:
        DrawRectangleRec(destination, GRAY);
        DrawText(">", destination.x + destination.width / 2 - 3, destination.y + destination.height / 2 - 10, 20, BLACK);
        break;
    }
}

void DrawPlayFieldEntities(PlayField *field) 
{
    for (int i = 0; i < MAX_ENTS; i += 1) {
        Entity *ent = &field->entities[i];
        if (ent->alive) {
            DrawEntity(field, ent);
            DrawEntityCollider(field, ent);
        }
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    DrawPlayField(&playField);
    DrawTensionBar(&tensionBar, &playField);
    if (IsKeyDown(KEY_W)) {
        tensionBar.tensionPercentage += 0.01;
    }
    if (IsKeyDown(KEY_S)) {
        tensionBar.tensionPercentage -= 0.01;
    }
    DrawTextEx(font, playField.name, (Vector2){ 10, GetScreenHeight()-font.baseSize*2-10 }, font.baseSize*2, 4, GRAY);
    tensionBar.tensionPercentage = Clamp(0.0, tensionBar.tensionPercentage, 1.0);
    DrawPlayFieldEntities(&playField);
    DrawPlayer(&playerState, &playField);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}
