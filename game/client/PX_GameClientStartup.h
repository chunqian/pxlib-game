#ifndef PX_GAMECLIENTSTARTUP_H
#define PX_GAMECLIENTSTARTUP_H

#include "../../pxlib/core/PX_Core.h"

typedef struct {
    px_char IpAddr[32];
    px_ushort syncDataPort;
    px_ushort syncFramePort;
    px_dword clientID;
    px_dword serverID;
} PX_GameClientStartUp;

#endif