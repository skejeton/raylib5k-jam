#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "raylib.h"
#include "timer.h"

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
    ENT_POINTY,
} EntityArt;

typedef enum {
    ENTB_IDLE,
    // (Move to next stage), preferrably used with ENT_ARROW_NEXT
    ENTB_NEXT_LEVEL,
    ENTB_FOLLOW_DIRECTION,
    ENTB_TARGET_PLAYER,
    ENTB_WAVY,
} EntityBehaviour;

typedef struct {
    bool alive;
    double amplitude;
    double lifetime;
    double tensionDelta;
    double angle;
    double speed;
    Vector2 lockedTarget;
    EntityBehaviour behaviour;
    EntityArt art;
    Rectangle rect;
} Entity;

typedef enum {
    SPWR_WAIT,
    SPWR_CIRCLE,
    SPWR_HORIZONTAL,
    SPWR_VERTICAL,
    SPWR_RANDOM
} SpawnRule;

typedef struct {
    bool active;
    SpawnRule rule;
    // Generic value preserved between states
    double intermediate;
    Entity prototype;
    Timer timer;
} Spawner;

#define MAX_ENTS 1024
#define MAX_SPAWNERS 1024

typedef struct {
    bool won;
    const char *name;
    Rectangle rect;
    Entity entities[MAX_ENTS];
    Spawner spawnerProtos[MAX_SPAWNERS];
    int spawnerProtoCount;
    int activeSpawner;
    Player playerState;
    TensionBar tensionBar;
} PlayField;

#endif 
