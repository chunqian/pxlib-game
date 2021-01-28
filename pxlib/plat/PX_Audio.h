#ifndef PX_AUDIO_H
#define PX_AUDIO_H

#include "PX_Display.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/PX_Log.h"
#include "../core/PX_Sound.h"

int PX_AudioInitialize(PX_SoundPlay *soundPlay, int startThread);
void PX_AudioSetVolume(unsigned long volume);

#ifdef __cplusplus
}
#endif

#endif
