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

#include "interp.h"
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
        timer->counter -= timer->treshold;
        return true;
    }
    return false;
}

Rectangle ShrinkRectangle(Rectangle rect, double by) {
    return (Rectangle) { rect.x + by, rect.y + by, rect.width - by * 2, rect.height - by * 2 };
}

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static int currentLevelId = 0;
static double tensionBarDisplayValue = 0.0;
static double levelTransitionAnimation = 0.0;
static bool movingPlayerToCenter = false;
static double tensionRectTransitionAnimation = 0.0;
static double deathAnimation = 0.0;
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
            SetSoundVolume(fxShoot, 0.1);
            SetSoundPitch(fxShoot, 0.2);
            PlaySound(fxShoot);
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

void AdvanceLevel()
{
    levelTransitionAnimation = 1.0;
    Vector2 prev_player_pos = playField.playerState.position;
    playField = GLOBAL_game_levels[currentLevelId++];
    playField.rect = MakePlayFieldRectangle();
    playField.playerState.position = prev_player_pos;
    movingPlayerToCenter = true;
}

// Summons the next room arrow on the right of the room
void SummonNextRoomArrow(PlayField *field)
{
    int w = 30, h = 80, x = field->rect.width - w, y = (field->rect.height - h) / 2;
    Entity *ent = SummonEntity(field);
    ent->lifetime = 1234567; // Practically forever
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
    return (Rectangle) {player->position.x-12, player->position.y-12, 44, 44};
}

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    AdvanceLevel();
}

double MyClamp(double low, double value, double hi)
{
    if (value < low)
        return low;
    if (value > hi)
        return hi;
    return value;
}


void SetSpawner(PlayField *field, int id) 
{
    if (id >= field->spawnerProtoCount) {
        id = 0;
    }
    Spawner *current_spawner = &field->spawnerProtos[field->activeSpawner];
    current_spawner->intermediate = 0;
    field->activeSpawner = id;
}

void NextSpawner(PlayField *field)
{
    SetSpawner(field, ++field->activeSpawner);
}

void UpdateActiveSpawner(PlayField *field)
{
    if (field->won) {
        return;
    }
    Spawner *spawner = &field->spawnerProtos[field->activeSpawner];
    int id = field->activeSpawner;
    while (id == field->activeSpawner && UpdateTimer(&spawner->timer)) {
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
                NextSpawner(field);
            }
        } break;
        case SPWR_HORIZONTAL: {
            Entity *ent = SummonEntity(field);
            *ent = spawner->prototype;
            int r = (rand()%2);
            ent->angle = r * M_PI;
            ent->rect.x = field->playerState.position.x - 200 + 400 * r;
            ent->rect.y = field->playerState.position.y;
            NextSpawner(field);
        } break;
        case SPWR_VERTICAL: {
            Entity *ent = SummonEntity(field);
            *ent = spawner->prototype;
            int r = (rand()%2);
            ent->angle = r * M_PI + M_PI/2;
            ent->rect.x = field->playerState.position.x;
            ent->rect.y = field->playerState.position.y - 200 + 400 * r;
            NextSpawner(field);
        } break;
        case SPWR_WAIT: {
            NextSpawner(field);
        } break;
        case SPWR_RANDOM: {
            Entity *ent = SummonEntity(field);
            *ent = spawner->prototype;
            int random_angle = (rand()%360);
            double random_angle_rad = random_angle*(M_PI/180.0);
            ent->angle = random_angle_rad;
            ent->rect.x = field->playerState.position.x - cos(random_angle_rad) * 200.0;
            ent->rect.y = field->playerState.position.y - sin(random_angle_rad) * 200.0;
            NextSpawner(field);
        } break;
        }
    }
}

void UpdatePlayer(Player *player, int playFieldWidth, int playFieldHeight) 
{
    const int PLAYER_SPEED = 3;
    const int PLAYER_SIZE  = 20;

    if (deathAnimation < 0.5 && !movingPlayerToCenter) {
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
            player->position.x -= PLAYER_SPEED;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
            player->position.x += PLAYER_SPEED;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
            player->position.y -= PLAYER_SPEED;
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
            player->position.y += PLAYER_SPEED;

        player->position.x = MyClamp(0, player->position.x, playFieldWidth-PLAYER_SIZE);
        player->position.y = MyClamp(0, player->position.y, playFieldHeight-PLAYER_SIZE);
    }
}

Vector2 VecNormalize(Vector2 vec) 
{
    double len = sqrt(vec.x*vec.x + vec.y*vec.y);
    return (Vector2) { vec.x / len, vec.y / len };
}

Vector2 VecSubtract(Vector2 vecA, Vector2 vecB) 
{
    return (Vector2) { vecA.x - vecB.x, vecA.y - vecB.y };
}

void UpdateEntity(Entity *ent)
{
    // NOTE: The exits will never disappear
    if (ent->lifetime < 0) {
        ent->alive = false;
    }
    ent->lifetime -= GetFrameTime();
    switch (ent->behaviour) {
    case ENTB_TARGET_PLAYER: {
        Vector2 dist = VecSubtract(playField.playerState.position, (Vector2) { ent->rect.x, ent->rect.y });
        double dist_len = sqrt(dist.x * dist.x + dist.y * dist.y);
        Vector2 dir = VecNormalize(dist);
        if (dist_len > 10.0) {
            ent->angle = atan2(dir.y, dir.x);
        }
    } 
    case ENTB_FOLLOW_DIRECTION:
        ent->rect.x += cos(ent->angle)*ent->speed*ent->lifetime/2.0;
        ent->rect.y += sin(ent->angle)*ent->speed*ent->lifetime/2.0;
        break;
    case ENTB_WAVY:
        ent->rect.x += sin(ent->angle)*(GetFrameTime() * sin(ent->lifetime+M_PI/2) * ent->amplitude);
        ent->rect.y += cos(ent->angle)*(GetFrameTime() * cos(ent->lifetime+M_PI/2) * ent->amplitude);
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
    if (deathAnimation < 0.01) {
        deathAnimation = 1.0;
    }
    // Clear out all exits
    for (int i = 0; i < MAX_ENTS; i += 1) {
        if (field->entities[i].lifetime > 1.0) {
        field->entities[i].lifetime = 1.0;
        }
    }
    field->activeSpawner = 0;
    bar->tensionPercentage = 0.0;
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
    if (deathAnimation < 0.1) {
        // Classic bullet hell...
        Rectangle player_rect = ShrinkRectangle(MakePlayerRectangle(player), 5);
        Rectangle player_tension_rect = MakePlayerTensionFieldRectangle(player);

        for (int i = 0; i < MAX_ENTS; i += 1) {
            Entity *ent = &field->entities[i];
            if (ent->alive) {
                if (CheckCollisionRecs(player_rect, ent->rect)) {
                    if (ent->behaviour == ENTB_NEXT_LEVEL) {
                        if (ent->lifetime > 1.0) {
                            return EVENT_NEXT_ROOM;
                        }
                    }
                    else {
                        if (ent->lifetime > 0.5) {
                            return EVENT_DEATH;
                        }
                    }
                }
                if (CheckCollisionRecs(player_tension_rect, ent->rect) && ent->lifetime > 0.2) {
                    field->tensionBar.tensionPercentage += ent->tensionDelta;
                    if (ent->tensionDelta) {
                        PlaySound(fxKick);
                        scoreKicks += 1;
                        tensionRectTransitionAnimation += 0.3;
                    }
                    ent->tensionDelta = 0;
                }
            }
        }
    }
    return EVENT_NONE;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    scoreTime += GetFrameTime();
    UpdatePlayFieldEntities(&playField);
    UpdatePlayer(&playField.playerState, 256, 256);
    if (playField.tensionBar.tensionPercentage >= 0.9999 && !playField.won) {
        WinCondition(&playField);
    }

    
    levelTransitionAnimation -= GetFrameTime()*3.0;
    if (levelTransitionAnimation < 0)
        levelTransitionAnimation = 0.0;

    tensionRectTransitionAnimation -= GetFrameTime()*3.0;
    if (tensionRectTransitionAnimation < 0)
        tensionRectTransitionAnimation = 0.0;
 
    deathAnimation -= GetFrameTime();
    if (deathAnimation < 0)
        deathAnimation = 0.0;


    playField.tensionBar.tensionPercentage = MyClamp(0.0, playField.tensionBar.tensionPercentage, 1.0);
    UpdateActiveSpawner(&playField);
    switch (PlayerInteract(&playField.playerState, &playField)) {
        case EVENT_DEATH:
            scoreDeathCount += 1;
            DeathCondition(&playField, &playField.tensionBar);
            break;
        case EVENT_NEXT_ROOM:
            if (currentLevelId >= GLOBAL_game_level_count) {
                finishScreen = 3;
                return;
            }
            AdvanceLevel();
            break;  
        default:;
    }
    if (movingPlayerToCenter) {
        Vector2 p_pos = playField.playerState.position;
        Vector2 p_delta = { 
            (128 - 15) - p_pos.x,
            (128 - 15) - p_pos.y
        };
        double length = sqrt(p_delta.x * p_delta.x + p_delta.y + p_delta.y);
        p_delta.x /= length;
        p_delta.y /= length;

        if (length > 0.1) {
            playField.playerState.position.x += p_delta.x * (length/30.0+2.0);
            playField.playerState.position.y += p_delta.y * (length/30.0+2.0);
        }
        if (length <= 5) {
            playField.playerState.position.x = 128-15;
            playField.playerState.position.y = 128-15;
            movingPlayerToCenter = false;
        }
    }
}

void DrawPlayFieldRectangle(Rectangle rect, Color color)
{
    DrawRectangleRec(ShrinkRectangle(rect, -4), color);
    DrawRectangleRec(rect, BLACK);
}

void DrawPlayField(PlayField *field) 
{
    DrawPlayFieldRectangle(field->rect, RAYWHITE);
}

// X, Y specify play field offset
void DrawPlayer(Player *player, PlayField *field) 
{
    Rectangle player_rect = ShrinkRectangle(TransformRectanglePlayField(field, MakePlayerRectangle(player)), -deathAnimation*10);
    DrawRectangleRec(player_rect, ColorAlpha(DARKRED, 1.0-deathAnimation));
    // Draw tension rectangle
    Rectangle tension_rect = TransformRectanglePlayField(field, MakePlayerTensionFieldRectangle(player));
    DrawRectangleLinesEx(tension_rect, 1, ColorAlpha(GOLD, tensionRectTransitionAnimation));
}

void DrawTensionBar(TensionBar *bar, PlayField *field) 
{
    tensionBarDisplayValue += (bar->tensionPercentage - tensionBarDisplayValue) / 6.0;
    int x = field->rect.x + 256 + 12, y = field->rect.y, h = field->rect.height;
    double percentage = MyClamp(0.0, tensionBarDisplayValue, 1.0);
    int actualHeight = h*percentage;
    Color barColor = ColorFromHSV(percentage*60.0, 1.0, 0.8);
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
    double fade = MyClamp(0, entity->lifetime, 1.0);
    static int counter = 0;
    switch (entity->art) {
    case ENT_MISSINGNO:
        srand(counter/200);
        for (int i = 0; i < 10; i += 1) {
            int randx = rand()%(int)destination.width+destination.x+10; 
            int randy = rand()%(int)destination.width+destination.y+10; 
            int rands = rand()%20; 

            DrawRectangleRec(ExpandRectangleFromCenter(randx, randy, rands), ColorAlpha(RAYWHITE, fade));
        }
        counter += 1;
        break;
    case ENT_POINTY:
        DrawPoly((Vector2) { destination.x+10, destination.y+10 }, 3, 20, entity->angle*(180/M_PI)+30, ColorAlpha(RAYWHITE, fade)); 
        break;
    case ENT_ARROW_NEXT:
        DrawRectangleRec(destination, ColorAlpha(GRAY, fade));
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
            /*
                DrawEntityCollider(field, ent);
            */
        }
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    double level_trans = InterpolateSineBare(levelTransitionAnimation);
    
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

    Camera2D cam = { 0 };
    cam.zoom = 1.0;
    cam.offset.x += level_trans * (256 + 64);

    playField.rect = MakePlayFieldRectangle();

    BeginMode2D(cam);
        DrawPlayField(&playField);
        DrawTensionBar(&playField.tensionBar, &playField);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, level_trans));
        {
            Rectangle shadow_rect = MakePlayFieldRectangle();
            shadow_rect.x -= 256 + 64;
            DrawPlayFieldRectangle(shadow_rect, ColorAlpha(RAYWHITE, level_trans));
        }
        DrawPlayFieldEntities(&playField);
    EndMode2D();
    DrawPlayer(&playField.playerState, &playField);
    DrawTextEx(font, playField.name, (Vector2){ 10, GetScreenHeight()-font.baseSize*2-10 }, font.baseSize*2, 4, GRAY);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen == 3;
}

