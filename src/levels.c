#include "levels.h"

PlayField GLOBAL_game_levels[] = {
    {
        .name = "...",
        .tensionBar = { 1.0 }
    },
    {
        .name = "LEVEL 2",
        .spawnerProtos = {
            {
                .timer = { 0, 0.5 },
                .rule = SPWR_RANDOM,
                .prototype = {
                    .alive = true,
                    .lifetime = 3,
                    .tensionDelta = 0.5,
                    .speed = 3,
                    .behaviour = ENTB_FOLLOW_DIRECTION,
                    .art = ENT_POINTY,
                    .rect = { 0, 0, 20, 20 }
                }
            }
        },
        .spawnerProtoCount = 1,
        .tensionBar = { 0.1 }
    }   
};

int GLOBAL_game_level_count = 2;
