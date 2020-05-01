#pragma once
#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#define WECHATWIN L"WeChatWin.dll"

DWORD Get_WeChatWin();
wchar_t * UTF8ToUnicode(const char* str);

//结构体
struct wxid_struct
{
	wchar_t * wxid;		//微信ID
	int wxidLen;		//ID长度
	int buffLen;		//缓冲区长度
};

struct wxmsg_struct
{
	wchar_t * wxmsg;	//微信消息
	int msgLen;			//消息长度
	int buffLen;		//缓冲区长度
};