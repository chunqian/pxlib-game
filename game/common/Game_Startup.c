#include "Game_Startup.h"

//////////////////////////////////////////////////////////////////////////
// Functions
px_uint64 game_rand_seed = (31415 * 16807) % (0xefffffff);

px_bool PX_InstanceInitialize(PX_Instance *Instance, const px_char name[], px_int width, px_int height, px_uint ui_size, px_uint resource_size,
                              px_uint game_size) {
    px_int allocSize;

    allocSize = ui_size + resource_size + game_size + 1024 * 1024 * 16;

    if (!PX_RuntimeInitialize(&Instance->runtime, width, height, malloc(allocSize), allocSize, ui_size, resource_size, game_size)) return PX_FALSE;

    if (height && width) {
        if (!PX_CreateWindow(width, height, name, PX_FALSE)) {
            return 0;
        }
    }

    return PX_TRUE;
}

px_uint32 Game_rand() { return (px_uint32)(game_rand_seed = PX_randEx(game_rand_seed)) % 0xffff; }

PX_IO_Data PX_LoadFileToIOData(const px_char path[]) {
    PX_IO_Data io;
    px_int fileoft = 0;
    FILE *pf = fopen(path, "rb");
    px_int filesize;
    if (!pf) {
        goto _ERROR;
    }
    fseek(pf, 0, SEEK_END);
    filesize = ftell(pf);
    fseek(pf, 0, SEEK_SET);

    io.buffer = (px_byte *)malloc(filesize + 1);
    if (!io.buffer) {
        goto _ERROR;
    }

    while (!feof(pf)) {
        fileoft += (px_int)fread(io.buffer + fileoft, 1, 1024, pf);
    }
    fclose(pf);

    io.buffer[filesize] = '\0';
    io.size = filesize;
    return io;
_ERROR:
    io.buffer = PX_NULL;
    io.size = 0;
    return io;
}

px_void PX_FreeIOData(PX_IO_Data *io) {
    if (io->size && io->buffer) {
        free(io->buffer);
        io->size = 0;
        io->buffer = PX_NULL;
    }
}

px_bool PX_LoadTextureFromFile(px_memorypool *mp, px_texture *tex, const px_char path[]) {
    PX_IO_Data io;

#ifdef PX_IMAGEDECODER_H
    if (PX_TextureCreateFromPngFile(mp, tex, path)) {
        return PX_TRUE;
    }
    if (PX_TextureCreateFromJpegFile(mp, tex, path)) {
        return PX_TRUE;
    }
#endif

    io = PX_LoadFileToIOData(path);
    if (!io.size) {
        return PX_FALSE;
    }
    if (PX_TextureCreateFromMemory(mp, io.buffer, io.size, tex)) {
        PX_FreeIOData(&io);
        return PX_TRUE;
    }
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadShapeFromFile(px_memorypool *mp, px_shape *shape, const px_char path[]) {
    PX_IO_Data io = PX_LoadFileToIOData(path);
    if (!io.size) {
        return PX_FALSE;
    }
    if (PX_ShapeCreateFromMemory(mp, io.buffer, io.size, shape)) {
        PX_FreeIOData(&io);
        return PX_TRUE;
    }
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadSoundFromFile(px_memorypool *mp, px_shape *shape, const px_char path[]) {
    PX_IO_Data io = PX_LoadFileToIOData(path);
    if (!io.size) {
        return PX_FALSE;
    }
    if (PX_ShapeCreateFromMemory(mp, io.buffer, io.size, shape)) {
        PX_FreeIOData(&io);
        return PX_TRUE;
    }
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadAnimationLibraryFromFile(px_memorypool *mp, PX_Animationlibrary *lib, const px_char path[]) {
    PX_IO_Data io;
    io = PX_LoadFileToIOData(path);
    if (!io.size) {
        return PX_FALSE;
    }
    if (PX_AnimationLibraryCreateFromMemory(mp, lib, io.buffer, io.size)) {
        PX_FreeIOData(&io);
        return PX_TRUE;
    }
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadScriptFromFile(px_memory *code, const px_char path[]) {
    PX_IO_Data io = PX_LoadFileToIOData(path);
    if (!io.size) {
        return PX_FALSE;
    }
    if (!PX_MemoryCopy(code, io.buffer, 0, io.size)) goto _ERROR;

    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadScriptInstanceFromFile(px_memorypool *mp, PX_ScriptVM_Instance *ins, const px_char path[]) {
    PX_IO_Data io = PX_LoadFileToIOData(path);
    if (!io.size) {
        return PX_FALSE;
    }
    if (!PX_ScriptVM_InstanceInit(ins, mp, io.buffer, io.size)) goto _ERROR;

    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadTextureToResource(PX_Runtime *runtime, const px_char Path[], const px_char key[]) {
    PX_IO_Data io;
    io = PX_LoadFileToIOData(Path);
    if (!io.size) goto _ERROR;
    if (!PX_ResourceLibraryLoad(&runtime->ResourceLibrary, PX_RESOURCE_TYPE_TEXTURE, io.buffer, io.size, key)) goto _ERROR;
    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadShapeToResource(PX_Runtime *runtime, const px_char Path[], const px_char key[]) {
    PX_IO_Data io;
    io = PX_LoadFileToIOData(Path);
    if (!io.size) goto _ERROR;
    if (!PX_ResourceLibraryLoad(&runtime->ResourceLibrary, PX_RESOURCE_TYPE_SHAPE, io.buffer, io.size, key)) goto _ERROR;
    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadAnimationToResource(PX_Runtime *runtime, const px_char Path[], const px_char key[]) {
    PX_IO_Data io;
    io = PX_LoadFileToIOData(Path);
    if (!io.size) goto _ERROR;
    if (!PX_ResourceLibraryLoad(&runtime->ResourceLibrary, PX_RESOURCE_TYPE_ANIMATIONLIBRARY, io.buffer, io.size, key)) goto _ERROR;
    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadScriptToResource(PX_Runtime *runtime, const px_char Path[], const px_char key[]) {
    PX_IO_Data io;
    io = PX_LoadFileToIOData(Path);
    if (!io.size) goto _ERROR;
    if (!PX_ResourceLibraryLoad(&runtime->ResourceLibrary, PX_RESOURCE_TYPE_SCRIPT, io.buffer, io.size, key)) goto _ERROR;
    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadSoundToResource(PX_Runtime *runtime, const px_char Path[], const px_char key[]) {
    PX_IO_Data io;
    io = PX_LoadFileToIOData(Path);
    if (!io.size) goto _ERROR;
    if (!PX_ResourceLibraryLoad(&runtime->ResourceLibrary, PX_RESOURCE_TYPE_SOUND, io.buffer, io.size, key)) goto _ERROR;
    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadFontModuleFromFile(PX_FontModule *fm, const px_char Path[]) {
    PX_IO_Data io;
    io = PX_LoadFileToIOData(Path);
    if (!io.size) goto _ERROR;
    if (!PX_FontModuleLoad(fm, io.buffer, io.size)) goto _ERROR;
    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_void PX_PlayLocalSoundFromResource(PX_Instance *pIns, PX_World *pWorld, px_float x, px_float y, const px_char key[], px_bool loop) {
    PX_SoundData *pdata;

    if (!PX_isPointInRect(PX_POINT(x, y, 0), PX_RECT((px_float)pWorld->offsetx - 128, (px_float)pWorld->offsety - 128, (px_float)pWorld->surface_width + 256,
                                                     (px_float)pWorld->surface_height + 256)))
        return;

    pdata = PX_ResourceLibraryGetSound(&pIns->runtime.ResourceLibrary, key);
    if (pdata) {
        PX_SoundPlayAdd(&pIns->soundplay, PX_SoundCreate(pdata, loop));
    }
}

px_void PX_PlayGlobalSoundFromResource(PX_Instance *pIns, const px_char key[], px_bool loop) {
    PX_SoundData *pdata;
    pdata = PX_ResourceLibraryGetSound(&pIns->runtime.ResourceLibrary, key);
    if (pdata) {
        PX_SoundPlayAdd(&pIns->soundplay, PX_SoundCreate(pdata, loop));
    }
}