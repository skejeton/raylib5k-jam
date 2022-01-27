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
#include "structures.h"
#include <math.h>
#include "timer.h"
#include "levels.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

// NOTE: This is implementation of timer.h,
// I moved it here to not create another TU
bool UpdateTimer(Timer *timer)
{
    timer->counter += GetFrameTime();
    if (timer->counter > timer->treshold) {
        timer->counter = 0;
        return true;
    }
    return false;
}


//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static int currentLevelId = 0;
static PlayField playField = { 0 };

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

Rectangle MakePlayFieldRectangle() 
{
    return (Rectangle) { CenterAtX(256), CenterAtY(256), 256, 256 };
}

void AdvanceLevel()
{
    playField = GLOBAL_game_levels[currentLevelId++];
    playField.rect = MakePlayFieldRectangle();
    playField.playerState.position.x = playField.rect.width / 2 - 15;
    playField.playerState.position.y = playField.rect.height / 2 - 15;
}

// Summons the next room arrow on the right of the room
void SummonNextRoomArrow(PlayField *field)
{
    int w = 30, h = 80, x = field->rect.width - w, y = (field->rect.height - h) / 2;
    Entity *ent = SummonEntity(field);
    ent->art = ENT_ARROW_NEXT;
    ent->behaviour = ENTB_NEXT_LEVEL;
    ent->rect = (Rectangle) { x, y, w, h };
}

Rectangle MakePlayerRectangle(Player *player) 
{
    return (Rectangle) { player->position.x, player->position.y, 20, 20 };
}

Rectangle MakePlayerTensionFieldRectangle(Player *player)
{
    return (Rectangle) {player->position.x-10, player->position.y-10, 40, 40};
}

int CenterAtX(int w) 
{
    return (GetScreenWidth()-w)/2;
}

int CenterAtY(int h) 
{
    return (GetScreenHeight()-h)/2;
}

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    AdvanceLevel();
}

double Clamp(double low, double value, double hi)
{
    if (value < low)
        return low;
    if (value > hi)
        return hi;
    return value;
}

void UpdateActiveSpawner(PlayField *field)
{
    if (field->won) {
        return;
    }
    Spawner *spawner = &field->spawnerProtos[field->activeSpawner];
    if (spawner && UpdateTimer(&spawner->timer)) {
        switch (spawner->rule) {
        case SPWR_CIRCLE: {
            double angle = spawner->intermediate / 10.0 * M_PI * 2;
            Entity *ent = SummonEntity(field);
            *ent = spawner->prototype;
            ent->angle = angle;
            ent->rect.x = field->playerState.position.x - cos(angle) * 200.0;
            ent->rect.y = field->playerState.position.y - sin(angle) * 200.0;
            spawner->intermediate += 1;
            if (spawner->intermediate >= 10) {
                field->activeSpawner++;
            }
        } break;
        case SPWR_HORIZONTAL: {
            Entity *ent = SummonEntity(field);
            *ent = spawner->prototype;
            int r = (rand()%2);
            ent->angle = r * M_PI;
            ent->rect.x = field->playerState.position.x - 200 + 400 * r;
            ent->rect.y = field->playerState.position.y;
            spawner->intermediate = 0;
            spawner->timer.counter = 0;
            field->activeSpawner++;
        } break;
        case SPWR_VERTICAL: {
            Entity *ent = SummonEntity(field);
            *ent = spawner->prototype;
            int r = (rand()%2);
            ent->angle = r * M_PI + M_PI/2;
            ent->rect.x = field->playerState.position.x;
            ent->rect.y = field->playerState.position.y - 200 + 400 * r;
            field->activeSpawner++;
        } break;
        case SPWR_WAIT: {
            field->activeSpawner++;
        } break;
        case SPWR_RANDOM: {
            Entity *ent = SummonEntity(field);
            *ent = spawner->prototype;
            int random_angle = (rand()%360);
            double random_angle_rad = random_angle*(M_PI/180.0);
            ent->angle = random_angle_rad;
            ent->rect.x = field->playerState.position.x - cos(random_angle_rad) * 200.0;
            ent->rect.y = field->playerState.position.y - sin(random_angle_rad) * 200.0;
            field->activeSpawner++;
        } break;
        }
    }
    if (field->activeSpawner >= field->spawnerProtoCount) {
        field->activeSpawner = 0;
    }
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

void UpdateEntity(Entity *ent)
{
    // NOTE: The exits will never disappear
    if (ent->behaviour != ENTB_NEXT_LEVEL && ent->lifetime < 0) {
        ent->alive = false;
    }
    ent->lifetime -= GetFrameTime();
    switch (ent->behaviour) {
    case ENTB_FOLLOW_DIRECTION:
        ent->rect.x += cos(ent->angle)*ent->speed;
        ent->rect.y += sin(ent->angle)*ent->speed;
        break;
    case ENTB_NEXT_LEVEL:
        // Pass here, because this is handled in collision
        break;
    case ENTB_IDLE:
        break;
    }
}

void UpdatePlayFieldEntities(PlayField *field)
{
    for (int i = 0; i < MAX_ENTS; i += 1) {
        Entity *ent = &field->entities[i];
        if (ent->alive) {
            UpdateEntity(ent);
        }
    }
}

void DeathCondition(PlayField *field, TensionBar *bar) 
{
    // We can die when we won because we can collide with the bullet as we
    // are winning, so I need to reset this here;
    field->won = false;
    // Clear out all entities
    for (int i = 0; i < MAX_ENTS; i += 1) {
        field->entities[i].alive = false;

        // HACK: Global access
        bar->tensionPercentage = 0.0;
    }
}

void WinCondition(PlayField *field)
{
    field->won = true;
    SummonNextRoomArrow(&playField);
}

typedef enum {
    EVENT_NONE,
    EVENT_DEATH,
    EVENT_NEXT_ROOM
} InteractEvent;

InteractEvent PlayerInteract(Player *player, PlayField *field)
{
    Rectangle player_rect = MakePlayerRectangle(player);
    // Classic bullet hell...
    player_rect.x += 4;
    player_rect.y += 4;
    player_rect.width -= 8;
    player_rect.height -= 8;
    Rectangle player_tension_rect = MakePlayerTensionFieldRectangle(player);

    for (int i = 0; i < MAX_ENTS; i += 1) {
        Entity *ent = &field->entities[i];
        if (ent->alive) {
            if (CheckCollisionRecs(player_rect, ent->rect)) {
                if (ent->behaviour == ENTB_NEXT_LEVEL) {
                    return EVENT_NEXT_ROOM;
                }
                else {
                    return EVENT_DEATH;
                }
            }
            if (CheckCollisionRecs(player_tension_rect, ent->rect)) {
                field->tensionBar.tensionPercentage += ent->tensionDelta;
                ent->tensionDelta = 0;
            }
        }
    }
    return EVENT_NONE;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    UpdatePlayFieldEntities(&playField);
    UpdatePlayer(&playField.playerState, 256, 256);
    if (playField.tensionBar.tensionPercentage >= 0.999 && !playField.won) {
        WinCondition(&playField);
    }
    playField.tensionBar.tensionPercentage = Clamp(0.0, playField.tensionBar.tensionPercentage, 1.0);
    UpdateActiveSpawner(&playField);
    switch (PlayerInteract(&playField.playerState, &playField)) {
        case EVENT_DEATH:
            DeathCondition(&playField, &playField.tensionBar);
            break;
        case EVENT_NEXT_ROOM:
            AdvanceLevel();
            break;  
        default:         
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
    Rectangle player_rect = TransformRectanglePlayField(field, MakePlayerRectangle(player));
    DrawRectangleRec(player_rect, DARKRED);
    // Draw tension rectangle
    Rectangle tension_rect = TransformRectanglePlayField(field, MakePlayerTensionFieldRectangle(player));
    DrawRectangleLinesEx(tension_rect, 1, ColorAlpha(WHITE, 0.2));
}

void DrawTensionBar(TensionBar *bar, PlayField *field) 
{
    int x = field->rect.x + 256 + 12, y = field->rect.y, h = field->rect.height;
    double percentage = Clamp(0.0, bar->tensionPercentage, 1.0);
    int actualHeight = h*percentage;
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
    double fade = Clamp(0, entity->lifetime, 1.0);
    switch (entity->art) {
    case ENT_MISSINGNO:
        static int counter = 0;
        srand(counter/200);
        for (int i = 0; i < 10; i += 1) {
            int randx = rand()%(int)destination.width+destination.x; 
            int randy = rand()%(int)destination.width+destination.y; 
            int rands = rand()%20; 

            DrawRectangleRec(ExpandRectangleFromCenter(randx, randy, rands), ColorAlpha(RAYWHITE, fade));
        }
        counter += 1;
        break;
    case ENT_POINTY:
        DrawPoly((Vector2) { destination.x+10, destination.y+10 }, 3, 20, entity->angle*(180/M_PI)+30, ColorAlpha(RAYWHITE, fade)); 
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
    DrawTensionBar(&playField.tensionBar, &playField);
    DrawTextEx(font, playField.name, (Vector2){ 10, GetScreenHeight()-font.baseSize*2-10 }, font.baseSize*2, 4, GRAY);
    DrawPlayFieldEntities(&playField);
    DrawPlayer(&playField.playerState, &playField);
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

