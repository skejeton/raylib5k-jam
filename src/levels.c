#include "levels.h"

#define pre_level_wait (Spawner) {\
    .timer = { 0, 2.0 },\
    .rule = SPWR_WAIT\
}

#define GENERIC_ENT(d, sp, lt) (Entity) { \
    .alive = true, \
    .lifetime = lt, \
    .tensionDelta = d, \
    .speed = sp, \
    .behaviour = ENTB_FOLLOW_DIRECTION, \
    .art = ENT_POINTY, \
    .amplitude = 128,\
    .rect = { 0, 0, 20, 20 } \
}

#define GENERIC_ENT_EX(d, sp, lt, r) (Entity) { \
    .alive = true, \
    .lifetime = lt, \
    .tensionDelta = d, \
    .speed = sp, \
    .behaviour = r, \
    .amplitude = 128,\
    .art = ENT_POINTY, \
    .rect = { 0, 0, 20, 20 } \
}

PlayField GLOBAL_game_levels[] = {
    {
        .name = "...",
        .tensionBar = { 1.0 }
    },
    {
        .name = "ATTENTION",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.5 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT(0.5, 3, 3)
            },
        },
        .spawnerProtoCount = 2,
        .tensionBar = { 0.0 }
    },
    {
        .name = "HYPER",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_VERTICAL,
                .prototype = GENERIC_ENT(0.25, 5, 3)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT(0.25, 5, 3)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_VERTICAL,
                .prototype = GENERIC_ENT(0.25, 5, 3)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT(0.25, 5, 3)
            },
        },
        .spawnerProtoCount = 5,
        .tensionBar = { 0.0 }
    },
    {
        .name = "TENSION",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.5 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT(0.1, 3, 3)
            },
        },
        .spawnerProtoCount = 2,
        .tensionBar = { 0.0 }
    },
    {
        .name = "TARGETED",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(1/3.0, 2, 4, ENTB_TARGET_PLAYER)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(1/3.0, 3, 3, ENTB_TARGET_PLAYER)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(1/3.0, 4, 2, ENTB_TARGET_PLAYER)
            },
        },
        .spawnerProtoCount = 4,
        .tensionBar = { 0.0 }
    },
    {
        .name = "CIRCLED",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.08 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT_EX(0.2, 2, 3, ENTB_TARGET_PLAYER)
            },
        },
        .spawnerProtoCount = 2,
        .tensionBar = { 0.0 }
    },
    {
        .name = "PINNED",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT(0.2, 6, 3)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_VERTICAL,
                .prototype = GENERIC_ENT(0.2, 6, 3)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT(0.2, 6, 3)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_VERTICAL,
                .prototype = GENERIC_ENT(0.2, 6, 3)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT(0.2, 6, 3)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_VERTICAL,
                .prototype = GENERIC_ENT_EX(0.2, 2, 3, ENTB_TARGET_PLAYER)
            },
            {
                .timer = { 0, 0.001 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT(0.2, 2, 3)
            },
        },
        .spawnerProtoCount = 8,
        .tensionBar = { 0.0 }
    },
    {
        .name = "PUNCHED",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.08 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT(0.2, 7, 3)
            },
            {
                .timer = { 0, 0.08 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT(0.2, 7, 3)
            },
            {
                .timer = { 0, 0.08 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT(0.2, 7, 3)
            },
            {
                .timer = { 0, 0.08 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT(0.2, 7, 3)
            },
{
                .timer = { 0, 0.08 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT(0.2, 7, 3)
            },
            {
                .timer = { 0, 0.08 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT(0.2, 7, 3)
            },

        },
        .spawnerProtoCount = 7,
        .tensionBar = { 0.0 }
    },
    {
        .name = "WAVY",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT_EX(0.2, 4, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT_EX(0.2, 4, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT_EX(0.2, 4, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.3 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 4, 2, ENTB_TARGET_PLAYER)
            },
            {
                .timer = { 0, 0.3 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 2, 3, ENTB_TARGET_PLAYER)
            },
            {
                .timer = { 0, 0.3 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 2, ENTB_TARGET_PLAYER)
            },
        },
        .spawnerProtoCount = 7,
        .tensionBar = { 0.0 }
    },
    {
        .name = "SPEEDY",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT_EX(0.2, 20, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT_EX(0.2, 20, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT_EX(0.2, 20, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT_EX(0.2, 20, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT_EX(0.2, 20, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT_EX(0.2, 20, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT_EX(0.2, 20, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_HORIZONTAL,
                .prototype = GENERIC_ENT_EX(0.2, 20, 10, ENTB_WAVY)
            },
        },
        .spawnerProtoCount = 9,
        .tensionBar = { 0.0 }
    },
    {
        .name = "PITY",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.05, 4, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.05, 4, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.05, 4, 10, ENTB_WAVY)
            },           
            {
                .timer = { 0, 0.3 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.5, 3.5, 3, ENTB_TARGET_PLAYER)
            },
        },
        .spawnerProtoCount = 5,
        .tensionBar = { 0.0 }
    },
    {
        .name = "GREEDY",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT(0.1, 1, 3)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT(0.15, 1, 3)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT(0.1, 1, 3)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT(0.05, 1, 3)
            },
        },
        .spawnerProtoCount = 5,
        .tensionBar = { 0.0 }
    },
    {
        .name = "CHAOS",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
        },
        .spawnerProtoCount = 9,
        .tensionBar = { 0.0 }
    },
    {
        .name = "MADNESS",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_VERTICAL,
                .prototype = GENERIC_ENT(0.2, 3, 10)
            },
        },
        .spawnerProtoCount = 13,
        .tensionBar = { 0.0 }
    },
    {
        .name = "DESPAIR",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 3, 1, ENTB_TARGET_PLAYER)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 10, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 10, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 10, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT(0.1, 5, 2)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT(0.05, 5, 2)
            },
            {
                .timer = { 0, 0.08 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT(0.2, 7, 3)
            },
            {
                .timer = { 0, 0.08 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT(0.2, 7, 3)
            },
        },
        .spawnerProtoCount = 9,
    },
    {
        .name = "SADNESS",
        .spawnerProtos = {
            pre_level_wait,
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_RANDOM,
                .prototype = GENERIC_ENT_EX(0.2, 2, 10, ENTB_WAVY)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT(0.2, 1, 1)
            },
            {
                .timer = { 0, 0.1 },
                .rule = SPWR_CIRCLE,
                .prototype = GENERIC_ENT(0.2, 1, 2)
            },
        },

        .spawnerProtoCount = 4,
        .tensionBar = { 0.0 }
    },

};

int GLOBAL_game_level_count = 16;
