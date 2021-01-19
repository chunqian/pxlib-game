#ifndef PX_LOG_H
#define PX_LOG_H

#include "PX_Typedef.h"

void PX_LOG(px_char fmt[]);
void PX_ERROR(px_char fmt[]);
px_char *PX_GETLOG();
void PX_ASSERT();

#endif
