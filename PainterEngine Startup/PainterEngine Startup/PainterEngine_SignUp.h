#ifndef PAINTERENGINE_SIGNUP_H
#define PAINTERENGINE_SIGNUP_H

#include "PainterEngine_Startup.h"

#define PX_SIGNUP_EXCHAGE_TIME 300
#define PX_SIGNUP_EXCHAGE_TIMES 10
#define PX_SIGNUP_REG_TIME 500
#define PX_SIGNUP_REG_TIMES 10

#define PX_SIGNUP_CONNECTION_TIMEOUT 3000
#define PX_SIGNUP_FAILED_TIMEOUT 3000

#define PX_SIGNUP_TEXT_ACCOUNT_COULD_NOT_EMPTY "�˻�����Ϊ��"
#define PX_SIGNUP_TEXT_NICKNAME_COULD_NOT_EMPTY "�ǳƲ���Ϊ��"
#define PX_SIGNUP_TEXT_REGCODE_COULD_NOT_EMPTY "ע���벻��Ϊ��"
#define PX_SIGNUP_TEXT_PASSWORD_TOO_SHORT "���볤��̫��"
#define PX_SIGNUP_TEXT_PASSWORD_NOT_MATCH "�������벻һ��"
#define PX_SIGNUP_TEXT_SIGNUP "ע��"
#define PX_SIGNUP_TEXT_EXIT "�˳�"

#define PX_SIGNUP_TEXT_INVALID_ACCOUNT "��Ч���˻���Ϣ"
#define PX_SIGNUP_TEXT_REPEAT_ACCOUNT "�˻����ظ�"
#define PX_SIGNUP_TEXT_INVALID_NICKNAME "��Ч���ǳ�"
#define PX_SIGNUP_TEXT_INVALID_PASSWORD "��Ч������"
#define PX_SIGNUP_TEXT_INVALID_SIGNUP_CODE "��Ч��ע����"
#define PX_SIGNUP_TEXT_INVALID_REPEAT_IP "ע����ʵЧ����"
#define PX_SIGNUP_TEXT_CONNECTION_FAILED ((px_char *)L"��������ʧ��")
#define PX_SIGNUP_TEXT_REGISTRY_SUCCEEDED ((px_char *)L"ע��ɹ�")
#define PX_SIGNUP_TEXT_LOGIN "��¼"


typedef enum
{
    PX_SIGNUP_STATUS_STANDBY,
    PX_SIGNUP_STATUS_REGISTING_EXCHAGE,
    PX_SIGNUP_STATUS_REGISTING_REG,
    PX_SIGNUP_STATUS_REGISTING_DOWNLOAD,
    PX_SIGNUP_STATUS_SUCCEEDED,
    PX_SIGNUP_STATUS_FAILED,
}PX_SIGNUP_STATUS;



typedef struct  
{
    PX_Instance *pIns;
    px_char userName[32];
    px_word nickName[32];
    px_char password[32];
    px_char regcode[32];
    px_byte privatekey[32];
    px_byte publickey[32];
    px_byte sharekey[32];
    px_char *loginText;
    px_texture SignUpTexture,SigningTexture;
    PX_SIGNUP_STATUS status;
    PX_Object *signup_root;
    PX_Object *edit_user,*edit_nickname,*edit_pwd,*edit_pwdrepeat,*edit_regcode,*btn_login;
    PX_Object *btn_reg,*btn_exit;
    PX_Object *label_accountInfo,*label_nickInfo,*label_pwdInfo,*label_regcodeInfo;
    PX_Object *radius_signing;
    PX_UDP udp;
    PX_UDP_ADDR ServerAddr;
    px_dword elpased;
    px_int times;
}PX_SignUp;


px_bool PX_SignUpInitialize(PX_Instance *pIns,PX_SignUp *pR,const px_char SignupServerIpAddr[],px_ushort port);
px_void PX_SignUpPostEvent(PX_SignUp *pl,PX_Object_Event e);
px_void PX_SignUpUpdate(PX_SignUp *pl,px_dword elpased);
px_void PX_SignUpRender(PX_SignUp *pl,px_dword elpased);
#endif
