#pragma once
#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#define WECHATWIN L"WeChatWin.dll"

DWORD Get_WeChatWin();
wchar_t * UTF8ToUnicode(const char* str);

//�ṹ��
struct wxid_struct
{
	wchar_t * wxid;		//΢��ID
	int wxidLen;		//ID����
	int buffLen;		//����������
};

struct wxmsg_struct
{
	wchar_t * wxmsg;	//΢����Ϣ
	int msgLen;			//��Ϣ����
	int buffLen;		//����������
};