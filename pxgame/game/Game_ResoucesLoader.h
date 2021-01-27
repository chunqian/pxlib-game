#ifndef GAME_RESOURCELOADER_H
#define GAME_RESOURCELOADER_H

#include "Game_Text.h"
#include "Game_Startup.h"

typedef enum { GAME_RESOURCESLOADER_TYPE_CLIENT, GAME_RESOURCESLOADER_TYPE_SERVER } GAME_RESOURCESLOADER_TYPE;

px_bool Game_ResourcesLoaderInitialize(PX_Instance *Ins, GAME_RESOURCESLOADER_TYPE type);

#endif