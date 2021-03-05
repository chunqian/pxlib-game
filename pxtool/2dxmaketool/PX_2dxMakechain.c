#include "PX_2dxMakechain.h"

px_byte memoryPool[1024 * 1024 * 64];  // 64M memory

#define PX_2DX_MNEMONIC_TEXTURE "TEXTURE"
#define PX_2DX_MNEMONIC_FRAME "FRAME"
#define PX_2DX_MNEMONIC_SLEEP "SLEEP"
#define PX_2DX_MNEMONIC_TAG "TAG"
#define PX_2DX_MNEMONIC_LOOP "LOOP"
#define PX_2DX_MNEMONIC_GOTO "GOTO"
#define PX_2DX_MNEMONIC_ANIMATION "ANIMATION"
#define PX_2DX_MNEMONIC_END "END"

px_char *PX_2dx_GetTexturePath(px_char *script, px_char *texName) {
    int i;
    static px_char path[260];
    PX_strcpy(path, script, sizeof(path));

    for (i = PX_strlen(path); i >= 0; i--) {
        if (path[i] == '/' || path[i] == '\\') {
            break;
        } else {
            path[i] = '\0';
        }
    }

    PX_strcat(path, texName);

    return path;
}

px_void MemoryPoolError(PX_MEMORYPOOL_ERROR err) {
    printf("ERROR:Out of cache\n");
    while (1)
        ;
}

PX_LEXER_LEXEME_TYPE PX_2dx_NextLexer(px_lexer *lexer) {
    PX_LEXER_LEXEME_TYPE type;
    do {
        type = PX_LexerGetNextLexeme(lexer);
    } while (type == PX_LEXER_LEXEME_TYPE_SPACER);

    return type;
}

px_bool PX_2dx_WriteFile(FILE *pf, px_byte *buffer, px_int size) {
    px_int oft = 0;
    while (size) {
        if (size > 4096) {
            if (!fwrite(buffer + oft, 4096, 1, pf)) return PX_FALSE;
            oft += 4096;
            size -= 4096;
        } else {
            if (!fwrite(buffer + oft, size, 1, pf)) return PX_FALSE;
            size = 0;
        }
    }
    return PX_TRUE;
}

px_bool PX_2dx_LoadFileToMemory(px_char *path, px_memorypool *mp, px_byte **bin, px_int *size) {
    FILE *pf;
    px_int fsize, oft = 0;
    if (!(pf = fopen(path, "rb"))) {
        return PX_FALSE;
    }
    fseek(pf, 0, SEEK_END);
    fsize = ftell(pf);
    *size = fsize;
    if (fsize == 0) {
        return PX_FALSE;
    }
    fseek(pf, 0, SEEK_SET);
    *bin = (px_byte *)MP_Malloc(mp, fsize + 1);
    if (*bin == NULL) {
        fclose(pf);
        return PX_FALSE;
    }
    while (!feof(pf)) {
        oft += fread((*bin) + oft, 1, 1024, pf);
    }
    fclose(pf);
    (*bin)[fsize] = 0;
    return PX_TRUE;
}

px_bool PX_2dx_Make(px_char *script, px_char *out) {
    px_int i, bufferSize;
    px_lexer lexer;
    px_memorypool mp;
    PX_LEXER_LEXEME_TYPE type;
    px_byte *buffer;
    px_dword instrAddr = 0;
    px_int lastAnimationAddr = -1;
    FILE *pf;
    px_texture texture;

    PX_2dxMake_tagInfo taginfo;
    PX_2dxMake_textureInfo textureInfo;

    PX_2DX_Header header;
    PX_2DX_INSTR instr;
    px_vector texVector;
    px_vector tagVector;
    PX_LEXER_STATE state;

    mp = MP_Create(memoryPool, sizeof(memoryPool));
    MP_ErrorCatch(&mp, MemoryPoolError);
    PX_LexerInit(&lexer, &mp);
    PX_LexerRegisterSpacer(&lexer, ' ');
    PX_LexerRegisterSpacer(&lexer, '\t');
    PX_LexerRegisterContainer(&lexer, "\"", "\"");

    PX_LexerSetNumericMatch(&lexer, PX_TRUE);
    PX_LexerSetTokenCase(&lexer, PX_LEXER_LEXEME_CASE_UPPER);

    PX_VectorInit(&mp, &tagVector, sizeof(PX_2dxMake_tagInfo), 16);
    PX_VectorInit(&mp, &texVector, sizeof(PX_2dxMake_textureInfo), 16);

    printf("Painter Engine 2dx maker\n");
    printf("compile %s\n", script);

    if (!PX_2dx_LoadFileToMemory(script, &mp, &buffer, &bufferSize)) {
        printf("could not load target script.\n");
        goto _RESERROR;
    }
    if (!PX_LexerLoadSourceFromMemory(&lexer, (px_char *)buffer)) {
        MP_Free(&mp, buffer);
        printf("could not solve target script.\n");
        goto _RESERROR;
    }

    MP_Free(&mp, buffer);

    // Scan
    instrAddr = 0;

    printf("Script Scanning....\n");

    state = PX_LexerGetState(&lexer);
    while (PX_TRUE) {
        type = PX_2dx_NextLexer(&lexer);

        if (type == PX_LEXER_LEXEME_TYPE_END) {
            break;
            ;
        }

        if (type == PX_LEXER_LEXEME_TYPE_NEWLINE) {
            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_TEXTURE)) {
            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_CONATINER) {
                goto _LEXER_ERROR;
            }
            PX_StringInit(&mp, &textureInfo.map);
            PX_StringInit(&mp, &textureInfo.path);
            PX_LexerGetIncludedString(&lexer, &lexer.CurLexeme);
            PX_StringCat(&textureInfo.path, lexer.CurLexeme.buffer);

            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_TOKEN) {
                PX_StringFree(&textureInfo.map);
                PX_StringFree(&textureInfo.path);
                goto _LEXER_ERROR;
            }

            PX_StringCat(&textureInfo.map, lexer.CurLexeme.buffer);

            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE) {
                PX_StringFree(&textureInfo.map);
                PX_StringFree(&textureInfo.path);
                goto _LEXER_ERROR;
            }

            printf("Texture:<%s> %s--> index %d\n", textureInfo.path.buffer, textureInfo.map.buffer, texVector.size);

            PX_VectorPushback(&texVector, &textureInfo);
            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_FRAME)) {
            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_TOKEN) {
                goto _LEXER_ERROR;
            }

            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE) {
                goto _LEXER_ERROR;
            }

            instrAddr += sizeof(px_dword) * 1;

            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_SLEEP)) {
            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_TOKEN) {
                goto _LEXER_ERROR;
            }
            if (!PX_strIsInt(lexer.CurLexeme.buffer)) {
                goto _LEXER_ERROR;
            }

            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE) {
                goto _LEXER_ERROR;
            }

            instrAddr += sizeof(px_dword) * 1;

            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_LOOP)) {
            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_TOKEN) {
                goto _LEXER_ERROR;
            }
            if (!PX_strIsInt(lexer.CurLexeme.buffer)) {
                goto _LEXER_ERROR;
            }

            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE) {
                goto _LEXER_ERROR;
            }

            instrAddr += sizeof(px_dword) * 1;

            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_TAG)) {
            type = PX_2dx_NextLexer(&lexer);

            if (type != PX_LEXER_LEXEME_TYPE_TOKEN) {
                goto _LEXER_ERROR;
            }

            PX_StringInit(&mp, &taginfo.tag);
            PX_StringCat(&taginfo.tag, lexer.CurLexeme.buffer);

            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE) {
                PX_StringFree(&taginfo.tag);
                goto _LEXER_ERROR;
            }
            taginfo.addr = instrAddr;
            printf("Tag:%s--> addr %d\n", taginfo.tag.buffer, taginfo.addr);
            PX_VectorPushback(&tagVector, &taginfo);
            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_GOTO)) {
            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_TOKEN) {
                goto _LEXER_ERROR;
            }

            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE) goto _LEXER_ERROR;

            instrAddr += sizeof(px_dword);

            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_ANIMATION)) {
            type = PX_2dx_NextLexer(&lexer);

            if (type != PX_LEXER_LEXEME_TYPE_TOKEN) {
                goto _LEXER_ERROR;
            }

            PX_StringInit(&mp, &taginfo.tag);
            PX_StringCat(&taginfo.tag, lexer.CurLexeme.buffer);

            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE) {
                PX_StringFree(&taginfo.tag);
                goto _LEXER_ERROR;
            }
            taginfo.addr = instrAddr;

            printf("Animation Tag:%s--> addr %d\n", taginfo.tag.buffer, taginfo.addr);

            PX_VectorPushback(&tagVector, &taginfo);
            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_END)) {
            type = PX_2dx_NextLexer(&lexer);

            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE && type != PX_LEXER_LEXEME_TYPE_END) {
                PX_StringFree(&taginfo.tag);
                goto _LEXER_ERROR;
            }
            instrAddr += sizeof(px_dword);
            continue;
        }
        printf("unknow script.\n");
        goto _LEXER_ERROR;
    }

    // Create output
    printf("Create output file : %s\n", out);
    pf = fopen(out, "wb");
    if (pf == PX_NULL) {
        goto _RESERROR;
    }

    // Generate header
    PX_memcpy(&header.magic, "2DX", sizeof("2DX"));
    header.framecount = texVector.size;
    header.animationCount = tagVector.size;
    printf("2DX stream %d animations with %d frames\n", header.animationCount, header.framecount);

    if (!PX_2dx_WriteFile(pf, (px_byte *)&header, sizeof(header))) {
        goto _RESERROR;
    }

    // Write AnimationTag
    for (i = 0; i < tagVector.size; i++) {
        PX_2dxMake_tagInfo *pTag = PX_VECTORAT(PX_2dxMake_tagInfo, &tagVector, i);
        if (!PX_2dx_WriteFile(pf, (px_byte *)&pTag->addr, sizeof(pTag->addr)))  // write ip
        {
            goto _RESERROR;
        }
        if (!PX_2dx_WriteFile(pf, (px_byte *)pTag->tag.buffer, PX_strlen(pTag->tag.buffer) + 1)) {
            goto _RESERROR;
        }
        printf("2DX Animation %s ->ip: %d \n", pTag->tag.buffer, pTag->addr);
    }

    // load textures
    printf("Textures Loading:\n");

    for (i = 0; i < texVector.size; i++) {
        if (!PX_2dx_LoadFileToMemory(PX_2dx_GetTexturePath(script, PX_VECTORAT(PX_2dxMake_textureInfo, &texVector, i)->path.buffer), &mp, &buffer,
                                     &bufferSize)) {
            printf("Could not load target file:%s", PX_2dx_GetTexturePath(script, PX_VECTORAT(PX_2dxMake_textureInfo, &texVector, i)->path.buffer));
            goto _RESERROR;
        }
        if (!PX_TextureCreateFromMemory(&mp, buffer, bufferSize, &texture)) {
            MP_Free(&mp, buffer);
            fclose(pf);
            goto _RESERROR;
        }

        MP_Free(&mp, buffer);

        printf("Loaded:%s\n", PX_VECTORAT(PX_2dxMake_textureInfo, &texVector, i)->path.buffer);

        PX_TRawBuild(&texture, PX_NULL, &bufferSize);

        buffer = (px_byte *)MP_Malloc(&mp, bufferSize);

        if (!PX_TRawBuild(&texture, buffer, &bufferSize)) {
            printf("TRaw built error\n");
            fclose(pf);
            goto _RESERROR;
        }

        if (!PX_2dx_WriteFile(pf, buffer, bufferSize)) {
            printf("Disk out of space\n");
            fclose(pf);
            goto _RESERROR;
        }

        MP_Free(&mp, buffer);

        PX_TextureFree(&texture);
    }

    // shellbin size
    if (!PX_2dx_WriteFile(pf, (px_byte *)&instrAddr, 4)) {
        printf("Disk out of space\n");
        fclose(pf);
        goto _RESERROR;
    }
    printf("shell size:%d bytes\n", instrAddr);
    // compile
    printf("Compiling...\n");

    PX_LexerSetState(state);

    while (PX_TRUE) {
        type = PX_2dx_NextLexer(&lexer);

        if (type == PX_LEXER_LEXEME_TYPE_END) {
            break;
        }

        if (type == PX_LEXER_LEXEME_TYPE_NEWLINE) {
            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_TEXTURE)) {
            PX_2dx_NextLexer(&lexer);
            PX_2dx_NextLexer(&lexer);
            PX_2dx_NextLexer(&lexer);
            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_FRAME)) {
            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_TOKEN) {
                goto _LEXER_ERROR;
            }

            for (i = 0; i < texVector.size; i++) {
                if (PX_strequ(PX_VECTORAT(PX_2dxMake_textureInfo, &texVector, i)->map.buffer, lexer.CurLexeme.buffer)) {
                    instr.opcode = PX_2DX_OPCODE_FRAME;
                    instr.param = i;
                    break;
                }
            }

            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE) {
                fclose(pf);
                goto _LEXER_ERROR;
            }

            if (!PX_2dx_WriteFile(pf, (px_byte *)&instr, sizeof(px_dword))) {
                printf("Disk out of space\n");
                fclose(pf);
                goto _RESERROR;
            }

            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_SLEEP)) {
            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_TOKEN) {
                fclose(pf);
                goto _LEXER_ERROR;
            }
            if (!PX_strIsInt(lexer.CurLexeme.buffer)) {
                fclose(pf);
                goto _LEXER_ERROR;
            }

            instr.opcode = PX_2DX_OPCODE_SLEEP;

            if (PX_atoi(lexer.CurLexeme.buffer) > 0 && PX_atoi(lexer.CurLexeme.buffer) < 65535) {
                instr.param = PX_atoi(lexer.CurLexeme.buffer);
            } else {
                fclose(pf);
                goto _LEXER_ERROR;
            }
            if (!PX_2dx_WriteFile(pf, (px_byte *)&instr, sizeof(px_dword))) {
                printf("Disk out of space\n");
                fclose(pf);
                goto _RESERROR;
            }

            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE) {
                goto _LEXER_ERROR;
            }

            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_LOOP)) {
            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_TOKEN) {
                fclose(pf);
                goto _LEXER_ERROR;
            }
            if (!PX_strIsInt(lexer.CurLexeme.buffer)) {
                fclose(pf);
                goto _LEXER_ERROR;
            }

            instr.opcode = PX_2DX_OPCODE_LOOP;

            if (PX_atoi(lexer.CurLexeme.buffer) > 0 && PX_atoi(lexer.CurLexeme.buffer) < 65535) {
                instr.param = PX_atoi(lexer.CurLexeme.buffer);
            } else if (PX_atoi(lexer.CurLexeme.buffer) == -1) {
                instr.param = 0xffff;
            } else {
                fclose(pf);
                goto _LEXER_ERROR;
            }
            if (!PX_2dx_WriteFile(pf, (px_byte *)&instr, sizeof(px_dword))) {
                printf("Disk out of space\n");
                fclose(pf);
                goto _RESERROR;
            }

            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE) {
                goto _LEXER_ERROR;
            }

            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_TAG)) {
            PX_2dx_NextLexer(&lexer);
            PX_2dx_NextLexer(&lexer);
            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_GOTO)) {
            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_TOKEN) {
                goto _LEXER_ERROR;
            }

            for (i = 0; i < tagVector.size; i++) {
                if (PX_strequ(PX_VECTORAT(PX_2dxMake_tagInfo, &tagVector, i)->tag.buffer, lexer.CurLexeme.buffer)) {
                    instr.opcode = PX_2DX_OPCODE_GOTO;
                    instr.param = PX_VECTORAT(PX_2dxMake_tagInfo, &tagVector, i)->addr;
                    break;
                }
            }

            type = PX_2dx_NextLexer(&lexer);

            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE) goto _LEXER_ERROR;

            if (!PX_2dx_WriteFile(pf, (px_byte *)&instr, sizeof(px_dword))) {
                printf("Disk out of space\n");
                fclose(pf);
                goto _RESERROR;
            }

            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_ANIMATION)) {
            if (lastAnimationAddr != -1) {
                goto _LEXER_ERROR;
            }

            type = PX_2dx_NextLexer(&lexer);
            if (type != PX_LEXER_LEXEME_TYPE_TOKEN) {
                goto _LEXER_ERROR;
            }

            for (i = 0; i < tagVector.size; i++) {
                if (PX_strequ(PX_VECTORAT(PX_2dxMake_tagInfo, &tagVector, i)->tag.buffer, lexer.CurLexeme.buffer)) {
                    lastAnimationAddr = PX_VECTORAT(PX_2dxMake_tagInfo, &tagVector, i)->addr;
                    break;
                }
            }

            type = PX_2dx_NextLexer(&lexer);

            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE) goto _LEXER_ERROR;

            continue;
        }

        if (PX_strequ(lexer.CurLexeme.buffer, PX_2DX_MNEMONIC_END)) {
            if (lastAnimationAddr == -1) {
                goto _LEXER_ERROR;
            }

            type = PX_2dx_NextLexer(&lexer);

            if (type != PX_LEXER_LEXEME_TYPE_NEWLINE && type != PX_LEXER_LEXEME_TYPE_END) {
                PX_StringFree(&taginfo.tag);
                goto _LEXER_ERROR;
            }
            instr.opcode = PX_2DX_OPCODE_END;
            instr.param = lastAnimationAddr;
            lastAnimationAddr = -1;

            if (!PX_2dx_WriteFile(pf, (px_byte *)&instr, sizeof(px_dword))) {
                printf("Disk out of space\n");
                fclose(pf);
                goto _RESERROR;
            }

            continue;
        }

        printf("unknow script.\n");
        goto _LEXER_ERROR;
    }

    if (lastAnimationAddr != -1) {
        goto _LEXER_ERROR;
    }

    fclose(pf);

    for (i = 0; i < tagVector.size; i++) {
        PX_StringFree(&PX_VECTORAT(PX_2dxMake_tagInfo, &tagVector, i)->tag);
    }
    for (i = 0; i < texVector.size; i++) {
        PX_StringFree(&PX_VECTORAT(PX_2dxMake_textureInfo, &texVector, i)->map);
        PX_StringFree(&PX_VECTORAT(PX_2dxMake_textureInfo, &texVector, i)->path);
    }

    PX_VectorFree(&texVector);
    PX_VectorFree(&tagVector);
    PX_LexerFree(&lexer);
    MP_DEBUG_AID(&mp);
    MP_Release(&mp);
    return PX_TRUE;
_LEXER_ERROR:
    printf("2dx script error:\n");
    lexer.Sources[lexer.SourceOffset] = '\0';
    printf(lexer.Sources);
    printf("<----Error\n");
_RESERROR:
    for (i = 0; i < tagVector.size; i++) {
        PX_StringFree(&PX_VECTORAT(PX_2dxMake_tagInfo, &tagVector, i)->tag);
    }
    for (i = 0; i < texVector.size; i++) {
        PX_StringFree(&PX_VECTORAT(PX_2dxMake_textureInfo, &texVector, i)->map);
        PX_StringFree(&PX_VECTORAT(PX_2dxMake_textureInfo, &texVector, i)->path);
    }

    PX_VectorFree(&texVector);
    PX_VectorFree(&tagVector);
    PX_LexerFree(&lexer);
    MP_Release(&mp);
    return PX_FALSE;
}
