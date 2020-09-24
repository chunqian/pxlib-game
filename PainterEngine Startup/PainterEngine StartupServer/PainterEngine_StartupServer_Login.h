#ifndef DEMO_SERVERLOGIN_H
#define DEMO_SERVERLOGIN_H

#include "PainterEngine_Startup.h"
#include "../PainterEngine Network/GameServer_Network.h"
#include "PainterEngine_StartupServer_Database.h"
#include "../../PainterEngine Game/PainterEngine Game/Game_Object_Base.h"

#define PX_STARTUPSERVER_LOGIN_MAXCONNECTION 256
#define PX_STARTUPSERVER_LOGIN_CONNECTION_TIMEOUT 3000
#define PX_STARTUPSERVER_LOGIN_LISTEN_PORT 31415

#define PX_STARTUPSERVER_LOGIN_TEXT_INITIALIZE_LOGIN_UDP_FAILED "��ʼ����¼������UDP��ʼ��ʧ��."
#define PX_STARTUPSERVER_LOGIN_TEXT_LISTEN_LOGIN_UDP_FAILED "������¼����˿�ʧ��."
#define PX_STARTUPSERVER_LOGIN_INVALID_LOGIN_DATA L"��¼ʧ��,�˻��������"
#define PX_STARTUPSERVER_LOGIN_INVALID_VERSION L"�汾����,�����:QȺ419410284��matrixcascade@gmail.com"

typedef struct
{
    px_bool activate;
    PX_UDP_ADDR Addr;
    px_byte  privatekey[32];
    px_byte  publickey[32];
    px_byte  theirkey[32];
    px_byte  sharekey[32];
    px_dword v_code;
    px_dword elpased;
}PX_StartupServer_LoginConnection;


typedef struct  
{
    PX_StartupServer_LoginConnection *connections;
    px_int max_connections;
    px_memorypool mp;
    PX_UDP udp;
}PX_StartupServer_Login;

px_bool PX_StartupServer_LoginInitialize(PX_StartupServer_Login *pLogin,px_int max_connections);
px_void PX_StartupServer_LoginUpdate(PX_StartupServer_Login *pLogin,px_dword elpased);
px_void PX_StartupServer_LoginFree(PX_StartupServer_Login *pLogin);

#endif
