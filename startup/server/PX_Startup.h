#ifndef PX_STARTUP_H
#define PX_STARTUP_H

#define _WIN32_WINNT 0x0601  // Windows 7
#define UNICODE

#include "../../pxlib/platform/windows/Platform_Windows_D2D.h"
#include "../../pxlib/platform/windows/px_udp.h"

#include "../../pxlib/arch/PX_Console.h"
#include "../../pxlib/arch/PX_MessageBox.h"
#include "../network/GameServer_Network.h"

#define PX_GAMESERVER_RESULT_PORT 47760

typedef struct {
    px_ushort dataport;
    px_ushort gameport;
    px_dword playerCount;
} PX_GameServerStartUp_Info;

typedef struct {
    px_dword clientID;
    px_dword serverID;
} PX_GameServerStartUp_PlayerNetInfo;

typedef struct {
    px_char account[32];
    px_dword group;
    px_dword rank;
    px_dword gameType;
    px_dword score;
} PX_GameServer_PlayerResult;

typedef struct {
    PX_Runtime runtime;
    PX_Console console;
    PX_SoundPlay soundplay;
} PX_Instance;

typedef struct {
    px_byte *buffer;
    px_uint size;
} PX_IO_Data;

px_bool PX_InstanceInitialize(PX_Instance *Instance, px_char *name, px_int width, px_int height, px_uint ui_size, px_uint resource_size, px_uint game_size);
PX_IO_Data PX_LoadFileToIOData(px_char *path);
px_void PX_FreeIOData(PX_IO_Data *io);
px_bool PX_LoadTextureFromFile(px_memorypool *mp, px_texture *tex, px_char *path);
px_bool PX_LoadShapeFromFile(px_memorypool *mp, px_shape *shape, px_char *path);
px_bool PX_LoadSoundFromFile(px_memorypool *mp, px_shape *shape, px_char *path);
px_bool PX_LoadAnimationLibraryFromFile(px_memorypool *mp, PX_Animationlibrary *lib, px_char *path);
px_bool PX_LoadScriptFromFile(px_memory *code, px_char *path);
px_bool PX_LoadJsonFromFile(PX_Json *json, const px_char *path);
px_bool PX_LoadScriptInstanceFromFile(px_memorypool *mp, PX_ScriptVM_Instance *ins, px_char *path);
px_bool PX_LoadTextureToResource(PX_Runtime *runtime, px_char Path[], px_char key[]);
px_bool PX_LoadShapeToResource(PX_Runtime *runtime, px_char Path[], px_char key[]);
px_bool PX_LoadAnimationToResource(PX_Runtime *runtime, px_char Path[], px_char key[]);
px_bool PX_LoadScriptToResource(PX_Runtime *runtime, px_char Path[], px_char key[]);
px_bool PX_LoadSoundToResource(PX_Runtime *runtime, px_char Path[], px_char key[]);

px_bool PX_CopyFile(const px_char resFile[], const px_char DestFile[]);
#endif
