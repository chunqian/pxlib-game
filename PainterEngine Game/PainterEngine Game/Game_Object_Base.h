#ifndef GAME_OBJECT_BASE_H
#define GAME_OBJECT_BASE_H
#include "PainterEngine_Startup.h"
#include "Game_Text.h"

#define GAME_VERSION 0x12

#define GAME_BLOCKHOLE_Z 9
#define GAME_AMMO_Z 8
#define GAME_PICKUP_Z 7
#define GAME_PARTICAL_FIRE_Z 6
#define GAME_TREASURE_Z 5
#define GAME_STONE_Z 4
#define GAME_ALIEN_Z 3
#define GAME_SHIP_Z 2
#define GAME_EXPLOSION_Z 1

#define GAME_IMPACT_TYPE_NONE 0
#define GAME_IMPACT_TYPE_GROUP0 1
#define GAME_IMPACT_TYPE_GROUP1 2
#define GAME_IMPACT_TYPE_GROUP2 4
#define GAME_IMPACT_TYPE_GROUP3 8
#define GAME_IMPACT_TYPE_GROUP4 16
#define GAME_IMPACT_TYPE_GROUP5 32
#define GAME_IMPACT_TYPE_GROUP6 64
#define GAME_IMPACT_TYPE_GROUP7 128         // players
#define GAME_IMPACT_TYPE_GROUP8 256         // players
#define GAME_IMPACT_TYPE_GROUP9 512         // players
#define GAME_IMPACT_TYPE_GROUP10 1024       // players
#define GAME_IMPACT_TYPE_GROUP11 2048       // players
#define GAME_IMPACT_TYPE_NATURE 4096        // nature
#define GAME_IMPACT_TYPE_AMMO 8192          // ammo
#define GAME_IMPACT_TYPE_SPECIALAMMO 16384  // special ammo
#define GAME_IMPACT_TYPE_PICKER 32768       // picker

#define GAME_IMPACT_TYPE_ALL_OBJECTGROUP 8191
#define GAME_IMPACT_TYPE_ALL_PLAYERGROUP 4095

#define GAME_MESSAGE_LEN 64

#define GAME_OBJECT_EVENT_CONTROLLER 0x1000
#define GAME_OBJECT_EVENT_DAMAGE 0x1001
#define GAME_OBJECT_EVENT_HEALTH 0x1002
#define GAME_OBJECT_EVENT_SWITCH 0x1003
#define GAME_OBJECT_EVENT_ADDWEAPON 0x1004
#define GAME_OBJECT_EVENT_POWERUP 0x1005
#define GAME_OBJECT_EVENT_ADDSHIELD 0x1006
#define GAME_OBJECT_EVENT_FIRE 0x1007
#define GAME_OBJECT_EVENT_CHANGEVELOCITY 0x1008
#define GAME_OBJECT_EVENT_SPEEDRUP 0x100a
#define GAME_OBJECT_EVENT_ADDSCORE 0x100b
#define GAME_OBJECT_EVENT_MESSAGE 0x100c
#define GAME_OBJECT_EVENT_DEFEAT 0x100d
#define GAME_OBJECT_EVENT_SETNAME 0x100e

typedef enum {
    GAME_OBJECT_TYPE_SHIP,
    GAME_OBJECT_TYPE_ALIEN,
    GAME_OBJECT_TYPE_DISPLAY,
    GAME_OBJECT_TYPE_SHOTGUN,
    GAME_OBJECT_TYPE_BLACKHOLE,
    GAME_OBJECT_TYPE_BLACKHOLE_REGION,
    GAME_OBJECT_TYPE_MINE,
    GAME_OBJECT_TYPE_MISSILE,
    GAME_OBJECT_TYPE_NUCLEAR,
    GAME_OBJECT_TYPE_RAILGUN,
    GAME_OBJECT_TYPE_PICKUP,
    GAME_OBJECT_TYPE_STONE,
    GAME_OBJECT_TYPE_TREASURE,
    GAME_OBJECT_TYPE_TREASURETAG,
    GAME_OBJECT_TYPE_PIERCER,
} GAME_OBJECT_TYPE;

typedef enum {
    GAME_PLAY_GAMETYPE_8P,
    GAME_PLAY_GAMETYPE_6P,
    GAME_PLAY_GAMETYPE_4P,
    GAME_PLAY_GAMETYPE_2P,
    GAME_PLAY_GAMETYPE_1P,
    GAME_PLAY_GAMETYPE_5V5,
    GAME_PLAY_GAMETYPE_4V4,
    GAME_PLAY_GAMETYPE_3V3,
    GAME_PLAY_GAMETYPE_2V2,
} GAME_PLAY_GAMETYPE;

#define GAME_OBJECT_SHIP_WEAPON_MACHINEGUN_DURATION 200
#define GAME_OBJECT_SHIP_WEAPON_NUCLEAR_DURATION 2000
#define GAME_OBJECT_SHIP_WEAPON_BLACKHOLE_DURATION 2000
#define GAME_OBJECT_SHIP_WEAPON_MISSILE_DURATION 500
#define GAME_OBJECT_SHIP_WEAPON_MINE_DURATION 1000
#define GAME_OBJECT_SHIP_WEAPON_RAILGUN_DURATION 500
#define GAME_OBJECT_SHIP_WEAPON_PROTECTSHIELD_DURATION 1000
#define GAME_OBJECT_SHIP_WEAPON_PIERCER_DURATION 200

#define GAME_OBJECT_SHIP_WEAPON_MACHINEGUN_MAX_CARGO 50
#define GAME_OBJECT_SHIP_WEAPON_NUCLEAR_MAX_CARGO 10
#define GAME_OBJECT_SHIP_WEAPON_BLACKHOLE_MAX_CARGO 10
#define GAME_OBJECT_SHIP_WEAPON_MISSILE_MAX_CARGO 100
#define GAME_OBJECT_SHIP_WEAPON_MINE_MAX_CARGO 20
#define GAME_OBJECT_SHIP_WEAPON_RAILGUN_MAX_CARGO 100
#define GAME_OBJECT_SHIP_WEAPON_PROTECTSHIELD_MAX_CARGO 2
#define GAME_OBJECT_SHIP_WEAPON_PIERCER_MAX_CARGO 100

typedef enum {
    GAME_OBJECT_SHIP_WEAPON_TYPE_NONE,
    GAME_OBJECT_SHIP_WEAPON_TYPE_MACHINEGUN,
    GAME_OBJECT_SHIP_WEAPON_TYPE_NUCLEAR,
    GAME_OBJECT_SHIP_WEAPON_TYPE_BLACKHOLE,
    GAME_OBJECT_SHIP_WEAPON_TYPE_MISSILE,
    GAME_OBJECT_SHIP_WEAPON_TYPE_MINE,
    GAME_OBJECT_SHIP_WEAPON_TYPE_RAILGUN,
    GAME_OBJECT_SHIP_WEAPON_TYPE_PROTECTSHIELD,
    GAME_OBJECT_SHIP_WEAPON_TYPE_PIERCER,
} GAME_OBJECT_SHIP_WEAPON_TYPE;

typedef enum {
    GAME_OBJECT_PLUGIN_TURRET_TYPE_GUN = 1,
    GAME_OBJECT_PLUGIN_TURRET_TYPE_MISSILE = 2,
    GAME_OBJECT_PLUGIN_TURRET_TYPE_CHARGER = 4,
    GAME_OBJECT_PLUGIN_TURRET_TYPE_LAUNCHER = 8,
} GAME_OBJECT_PLUGIN_TURRET_TYPE;

typedef struct {
    px_point offsetPoint;
    px_bool bActivate;
    px_dword type;
} Game_Object_Plugin_Turret;

typedef struct {
    px_char account[32];
    PX_Object *pShipObject;
    px_float Score;
    px_dword aliveTime;
} Game_Play_Player;

#define GAME_PLAY_USERDATA_PHOTOSIZE (64 * 64 * 4 + sizeof(PX_TRaw_Header))

typedef struct {
    px_dword mode;
} PX_GameStartUp_GameDesc;

typedef struct {
    // render description
    px_char account[32];
    px_byte Photo_Data[GAME_PLAY_USERDATA_PHOTOSIZE];
    px_word NickName[32];
    // update description
    px_dword shiptype;
    px_dword group;
    px_float maxshield;
    px_float maxlife;
    px_float maxforce;
    px_float maxspeed;
    px_float Armor;
    px_dword base_weapon_generator_time;
    px_dword last_fire_recovery;
    px_dword slientTime;
} PX_GameStartUp_PlayerDataDesc;

px_dword Game_Object_GroupImpactTestType(px_int group);
px_dword Game_Object_GroupImpactType(px_int group);

#endif
