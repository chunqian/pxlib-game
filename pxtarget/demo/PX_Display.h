#ifndef PX_DISPLAY_H
#define PX_DISPLAY_H

#include <stdio.h>
#include <windows.h>

// #pragma comment(lib, "user32.lib")
// #pragma comment(lib, "shell32.lib")
// #pragma comment(lib, "ole32.lib")
// #pragma comment(lib, "winmm.lib")

typedef struct {
    UINT uMsg;
    LPARAM lparam;
    WPARAM wparam;
} WM_MESSAGE;

#ifdef __cplusplus
extern "C" {
#endif

#include "../../pxlib/core/PX_Typedef.h"
#include "../../pxlib/plat/PX_UTF8.h"

BOOL PX_CreateWindow(int Width, int Height, const char *name, BOOL bfullScreen);
HWND PX_GetWindowHwnd();
VOID PX_SystemReadDeviceState();
BOOL PX_SystemLoop();
BOOL PX_SystemRender(void *raw, int width, int height);
BOOL PX_SystemisAvtivated();
BOOL PX_KeyboardDown(unsigned char X);
char *PX_KeyboardString();
char *PX_DragfileString();
BOOL PX_MouseLButtonDown();
BOOL PX_MouseRButtonDown();
BOOL PX_MouseMButtonDown();
POINT PX_MousePosition();
BOOL PX_KeyDown(unsigned char key);
BOOL PX_MouseWheel(int *x, int *y, int *delta);
BOOL PX_ShiftWinMessage(WM_MESSAGE *Stack, WM_MESSAGE *Msg);
BOOL PX_PushWinMessage(WM_MESSAGE *Stack, WM_MESSAGE *Msg);
// char *PX_OpenFileDialog(const char Filter[]);
// char *PX_SaveFileDialog(const char Filter[], const char ext[]);
// char *PX_MultFileDialog(const char Filter[]);
// char *PX_GetFileName(const char filePath[]);
int PX_SystemMessageBox(px_void *wnd, px_char *text, px_char *caption, px_uint type);

#ifdef __cplusplus
}
#endif

#endif