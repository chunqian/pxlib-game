#ifndef PX_2DX_MAKECHAIN_H
#define PX_2DX_MAKECHAIN_H

#include "../../pxlib/kernel/PX_2dxCommon.h"
#include "../../pxlib/kernel/PX_Lexer.h"

#include <stdio.h>

typedef struct {
    px_string path;  // image file path
    px_string map;
} PX_2dxMake_textureInfo;

typedef struct {
    px_string tag;  // image file path
    px_dword addr;
} PX_2dxMake_tagInfo;

px_bool PX_2dx_Make(px_char *script, px_char *out);
#endif
