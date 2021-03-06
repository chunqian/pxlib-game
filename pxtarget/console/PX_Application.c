#include "PX_Application.h"

#include <windows.h>

PX_Application App;

px_bool PX_ApplicationInitialize(PX_Application *App) {
    PX_ConsoleShow(&App->Instance.console, PX_TRUE);
    return PX_TRUE;
}

px_void PX_ApplicationUpdate(PX_Application *App, px_dword elpased) { PX_ConsoleUpdate(&App->Instance.console, elpased); }

px_void PX_ApplicationRender(PX_Application *App, px_dword elpased) {
    PX_ConsoleRender(&App->Instance.console, elpased);
    Sleep(10);
}

px_void PX_ApplicationPostEvent(PX_Application *App, PX_Object_Event e) {
    if (e.Param_uint[0] == 36) {
        return;
    }
    PX_ConsolePostEvent(&App->Instance.console, e);
}
