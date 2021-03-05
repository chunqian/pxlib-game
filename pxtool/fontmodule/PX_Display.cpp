#include "PX_Display.h"

#include <d2d1.h>
#include <d2d1helper.h>
#include <windowsx.h>

#define WIN_MAX_INPUT_STRING_LEN 64
#define WIN_MAX_INPUT_SPECKEY_LEN 0xff

#define WIN_KEYDOWN_BACKSPACE 1
#define WIN_KEYDOWN_RETURN 2
#define WIN_KEYDOWN_LEFT 3
#define WIN_KEYDOWN_RIGHT 4
#define WIN_KEYDOWN_UP 5
#define WIN_KEYDOWN_DOWN 6

#define WIN_MESSAGESTACK_SIZE 32
#define WM_CHARSTACK_SIZE 64

HWND Win_Hwnd;
int Win_Height;
int Win_Width;
BOOL Win_bFullScreen;
BOOL Win_Activated;
WM_MESSAGE Win_messageStack[WIN_MESSAGESTACK_SIZE] = {0};
WM_MESSAGE Win_messageCharacterStack[WIN_MESSAGESTACK_SIZE] = {0};
unsigned char DInput_KeyBoardState[256];
char DInput_AccepyFile[MAX_PATH] = {0};
POINT DInput_MousePosition;
POINT DInput_MouseWheelPosition;
POINT DInput_MouseWheelDelta;

ID2D1Bitmap *D2D_pSurface;
ID2D1Factory *D2D_pDirect2dFactory;
ID2D1HwndRenderTarget *D2D_pRenderTarget;

static WNDCLASSEXA Win_Wcx;

LRESULT CALLBACK AppWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void DInput_Update() {
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(Win_Hwnd, &point);
    DInput_MousePosition = point;
}

BOOL PX_D3DReset(HWND hWnd, int Width, int Height, BOOL bfullScreen) { return TRUE; }

char Win_Str[WIN_MAX_INPUT_STRING_LEN] = {0};
char Win_SpecKey[WIN_MAX_INPUT_SPECKEY_LEN] = {0};
int Win_CurrentIndex = 0;
#include <stdio.h>
LRESULT CALLBACK AppWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // if(uMsg == 0x246)
    // printf("%x\n", uMsg);
    WM_MESSAGE message;
    WM_MESSAGE message2;
    int i;

    switch (uMsg) {
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            break;
        case WM_KILLFOCUS: {
            Win_Activated = FALSE;
        } break;
        case WM_SETFOCUS: {
            Win_Activated = TRUE;
        } break;
        case WM_KEYDOWN: {
            Win_SpecKey[wParam] = 1;
        } break;
        case WM_MOUSEWHEEL: {
            DInput_MouseWheelPosition.x = GET_X_LPARAM(lParam);
            DInput_MouseWheelPosition.y = GET_Y_LPARAM(lParam);
            DInput_MouseWheelDelta.x = 0;
            DInput_MouseWheelDelta.y = GET_WHEEL_DELTA_WPARAM(wParam);
        } break;
        case WM_CHAR: {
            message = {0};
            message.uMsg = uMsg;
            message.wparam = wParam;
            message.lparam = lParam;
            PX_PushWinMessage(Win_messageCharacterStack, &message);

            if (Win_CurrentIndex < WIN_MAX_INPUT_STRING_LEN - 1) {
                if (wParam >= 32 || wParam == 8) {
                    Win_Str[Win_CurrentIndex++] = (char)wParam;
                    Win_Str[Win_CurrentIndex] = '\0';
                }
            }
        } break;
        case WM_DROPFILES: {
            HDROP hdrop = (HDROP)wParam;
            DragQueryFileA(hdrop, 0, DInput_AccepyFile, MAX_PATH);
            DragFinish(hdrop);
        } break;
        case WM_CLOSE: {
            PostQuitMessage(0);
            exit(0);
        } break;
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        } break;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    // message.uMsg = uMsg;
    // message.wparam = wParam;
    // message.lparam = lParam;
    // for (i = 0; i < WIN_MESSAGESTACK_SIZE; i++) {
    //     if (Win_messageStack[i].uMsg == 0) {
    //         Win_messageStack[i] = message;
    //         break;
    //     }
    // }

    message.uMsg = uMsg;
    message.wparam = wParam;
    message.lparam = lParam;
    if (uMsg != WM_CHAR) PX_PushWinMessage(Win_messageStack, &message);

    message = {0};
    if (!PX_ShiftWinMessage(Win_messageCharacterStack, &message)) return TRUE;
    if (message.wparam <= 127) {
        if (!PX_PushWinMessage(Win_messageStack, &message)) return TRUE;
    } else {
        if (!PX_ShiftWinMessage(Win_messageCharacterStack, &message2)) {
            PX_PushWinMessage(Win_messageCharacterStack, &message);
            return TRUE;
        }
        px_char ch_utf8[6] = {0};
        px_byte low = message.wparam;
        px_byte high = message2.wparam;
        const px_char ch_gb[2] = {(char)low, (char)high};
        gb_to_utf8(ch_gb, ch_utf8, 6);
        for (i = 0; i < 6; i++) {
            px_byte ch_byte = (px_byte)ch_utf8[i];
            if ((px_int)ch_byte == 0) break;
            message.wparam = (WPARAM)ch_byte;
            PX_PushWinMessage(Win_messageStack, &message);
        }
    }

    return TRUE;
}

BOOL PX_SystemLoop() {
    MSG Msg;
    ZeroMemory(&Msg, sizeof(MSG));

    if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    } else {
        Sleep(10);
    }
    return TRUE;
}

BOOL PX_SystemRender(void *raw, int width, int height) {
    D2D1_RECT_U size;
    size.left = 0;
    size.right = width;
    size.top = 0;
    size.bottom = height;

    D2D_pRenderTarget->BeginDraw();
    D2D_pRenderTarget->Clear();
    D2D_pSurface->CopyFromMemory(&size, raw, width * sizeof(DWORD));
    D2D_pRenderTarget->DrawBitmap(D2D_pSurface);
    D2D_pRenderTarget->EndDraw();

    return TRUE;
}

BOOL PX_SystemisAvtivated() { return Win_Activated; }

BOOL PX_KeyboardDown(unsigned char X) { return (DInput_KeyBoardState[X] & 0x80 ? TRUE : FALSE); }

BOOL PX_MouseLButtonDown() { return GetAsyncKeyState(VK_LBUTTON) & 0xFF00; }

BOOL PX_MouseRButtonDown() { return GetAsyncKeyState(VK_RBUTTON) & 0xFF00; }

BOOL PX_MouseMButtonDown() { return GetAsyncKeyState(VK_MBUTTON) & 0xFF00; }

POINT PX_MousePosition() { return DInput_MousePosition; }

char *PX_KeyboardString() {
    if (Win_CurrentIndex != 0) {
        Win_CurrentIndex = 0;
        return Win_Str;
    } else {
        return 0;
    }
}

VOID PX_SystemReadDeviceState() { DInput_Update(); }

char *PX_DragfileString() { return DInput_AccepyFile; }

HWND PX_GetWindowHwnd() { return Win_Hwnd; }

BOOL PX_KeyDown(unsigned char key) {
    if (Win_SpecKey[key]) {
        Win_SpecKey[key] = 0;
        return TRUE;
    }
    return FALSE;
}

BOOL PX_MouseWheel(int *x, int *y, int *delta) {
    if (DInput_MouseWheelDelta.y != 0) {
        *x = DInput_MouseWheelPosition.x;
        *y = DInput_MouseWheelPosition.y;
        *delta = DInput_MouseWheelDelta.y;
        DInput_MouseWheelDelta.y = 0;
        return TRUE;
    }
    return FALSE;
}

BOOL PX_PushWinMessage(WM_MESSAGE *Stack, WM_MESSAGE *Msg) {
    int i;
    if (Stack == PX_NULL) Stack = Win_messageStack;

    for (i = 0; i < WIN_MESSAGESTACK_SIZE; i++) {
        if (Stack[i].uMsg == 0) {
            Stack[i] = *Msg;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL PX_ShiftWinMessage(WM_MESSAGE *Stack, WM_MESSAGE *Msg) {
    int i;
    if (Stack == PX_NULL) Stack = Win_messageStack;

    for (i = 0; i < WIN_MESSAGESTACK_SIZE; i++) {
        if (Stack[i].uMsg != 0) {
            *Msg = Stack[i];
            memset(&Stack[i], 0, sizeof(WM_MESSAGE));
            return TRUE;
        }
    }
    return FALSE;
}

char *PX_OpenFileDialog(const char Filter[]) {
    OPENFILENAMEA ofn;
    static char szFile[MAX_PATH];
    px_char Filter2[MAX_PATH] = {0};
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = TEXT('\0');
    ofn.nMaxFile = sizeof(szFile);
    // ofn.lpstrFilter = Filter;  // TEXT("Mirror文件(.mirror)\0*.mirror");
    utf8_to_gb(Filter, Filter2, MAX_PATH);
    ofn.lpstrFilter = Filter2;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.hwndOwner = Win_Hwnd;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn)) {
        return szFile;
    }
    return NULL;
}

char *PX_MultFileDialog(const char Filter[]) {
    OPENFILENAMEA ofn;
    static char szFile[MAX_PATH * 64];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = TEXT('\0');
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = Filter;  // TEXT("Mirror文件(.mirror)\0*.mirror");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.hwndOwner = Win_Hwnd;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn)) {
        if (szFile[strlen(szFile) + 1] == '\0') {
            int oft = strlen(szFile) - 1;
            while (oft >= 0) {
                if (szFile[oft] == '\\' || szFile[oft] == '/') {
                    szFile[oft] = 0;
                    break;
                }
                oft--;
            }
        }
        return szFile;
    }
    return NULL;
}

char *PX_SaveFileDialog(const char Filter[], const char ext[]) {
    OPENFILENAMEA ofn;
    static char szFile[MAX_PATH];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = TEXT('\0');
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = Filter;  // TEXT("Mirror文件(.mirror)\0*.mirror");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.hwndOwner = Win_Hwnd;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetSaveFileNameA(&ofn)) {
        if (ext) {
            char uprFile[MAX_PATH] = {0};
            char uprExt[MAX_PATH] = {0};
            // strcpy_s(uprExt, ext);
            PX_strcpy(uprExt, ext, PX_strlen(uprExt));
            // _strupr_s(uprExt, strlen(uprExt) + 1);
            PX_strupr(uprExt);
            // strcpy_s(uprFile, szFile);
            PX_strcpy(uprFile, szFile, PX_strlen(uprFile));
            // _strupr_s(uprFile, strlen(uprExt) + 1);
            PX_strupr(uprFile);
            if (!strstr(uprFile, uprExt)) {
                // strcat_s(szFile, sizeof(szFile), ext);
                PX_strcat(szFile, ext);
            }
        }
        return szFile;
    }
    return NULL;
}

char *PX_GetFileName(const char filePath[]) {
    int offset = strlen(filePath) - 1;
    while (offset) {
        if (filePath[offset] == '/' || filePath[offset] == '\\') {
            offset++;
            break;
        }
        offset--;
    }
    return (char *)filePath + offset;
}

int PX_SystemMessageBox(px_void *wnd, px_char *text, px_char *caption, px_uint type) {
    px_char dstTest[256] = {0};
    px_char dstCaption[64] = {0};
    utf8_to_widechar(text, dstTest, 256);
    utf8_to_widechar(caption, dstCaption, 64);
    return MessageBoxW((HWND)wnd, (LPCWSTR)dstTest, (LPCWSTR)dstCaption, (UINT)type);
}
