#ifndef PX_UDP_H
#define PX_UDP_H

#include "../../core/PX_Core.h"

#include <windows.h>  //header
#include <winsock.h>
#include <strsafe.h>
#include <mmsystem.h>
#include <assert.h>
// #pragma comment(lib,"ws2_32.lib")                                               // Platform of MSVC
// #pragma comment( lib,"winmm.lib" )

typedef enum { PX_UDP_IP_TYPE_IPV4, PX_UDP_IP_TYPE_IPV6 } PX_UDP_IP_TYPE;

typedef struct {
    px_uint listenPort;
    px_uint socket;
    PX_UDP_IP_TYPE type;
} PX_UDP;

typedef struct {
    px_dword port;
    union {
        px_dword ipv4;
        px_dword ipv6[4];
    };
} PX_UDP_ADDR;

PX_UDP_ADDR PX_UDP_ADDR_IPV4(px_dword ipv4, px_ushort port);
px_dword PX_UDPaton(px_char *ipaddr);
px_bool PX_UDPListen(PX_UDP *udp, px_ushort listenPort);
px_bool PX_UDPInit(PX_UDP *udp, PX_UDP_IP_TYPE type);
px_bool PX_UDPSend(PX_UDP *udp, PX_UDP_ADDR addr, px_void *buffer, px_int size);
px_bool PX_UDPReceived(PX_UDP *udp, PX_UDP_ADDR *from_addr, px_void *buffer, px_int buffersize, px_int *readsize);
px_void PX_UDPFree(PX_UDP *udp);

#endif
