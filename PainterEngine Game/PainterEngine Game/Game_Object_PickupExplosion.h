#ifndef GAME_OBJECT_PICKUPEXPLOSION_H
#define GAME_OBJECT_PICKUPEXPLOSION_H

#include "Game_Object_Base.h"

#define GAME_OBJECT_PICKUPEXPLOSION_ALIVE 500

typedef enum {
    GAME_OBJECT_PICKETUP_TYPE_BLACKHOLE,
    GAME_OBJECT_PICKETUP_TYPE_MINE,
    GAME_OBJECT_PICKETUP_TYPE_MISSILE,
    GAME_OBJECT_PICKETUP_TYPE_NUCLEAR,
    GAME_OBJECT_PICKETUP_TYPE_RAILGUN,
    GAME_OBJECT_PICKETUP_TYPE_MACHINEGUN,
    GAME_OBJECT_PICKETUP_TYPE_HEALTH,
    GAME_OBJECT_PICKETUP_TYPE_SHIELD,
    GAME_OBJECT_PICKETUP_TYPE_SPEEDUP,
    GAME_OBJECT_PICKETUP_TYPE_POWERUP,
    GAME_OBJECT_PICKETUP_TYPE_PROTECTSHIELD,
    GAME_OBJECT_PICKETUP_TYPE_PIERCER,
} GAME_OBJECT_PICKETUP_TYPE;

typedef struct {
    PX_World *pWorld;
    PX_Instance *pIns;
    px_shape *logo;
    px_dword elpased;
    GAME_OBJECT_PICKETUP_TYPE type;
} Game_Object_PickupExplosion;

px_void Game_Object_PickupExplosionCreate(PX_Instance *pIns, PX_World *pWorld, px_point position, GAME_OBJECT_PICKETUP_TYPE type);

#endif