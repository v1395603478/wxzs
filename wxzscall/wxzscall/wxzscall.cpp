// wxzscall.cpp : 定义 DLL 应用程序的导出函数。

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include <stdlib.h >
#include "my_fun.h"



VOID Get_Grxx(HWND hModule)
{
	DWORD WeChatWin = Get_WeChatWin();
	CHAR wxid[0x100] = { 0 };
	sprintf_s(wxid,"%s", WeChatWin + 0x1397264);
	SetDlgItemText(hModule,ID_WXIDE, UTF8ToUnicode(wxid));

	CHAR name[0x100] = { 0 };
	sprintf_s(name, "%s", (char*)(WeChatWin + 0x13972DC));
	SetDlgItemText(hModule, ID_NAMEE,UTF8ToUnicode(name));

	CHAR sjhm[0x100] = { 0 };
	sprintf_s(sjhm, "%s", WeChatWin + 0x1397310);
	SetDlgItemText(hModule, ID_SJHME, UTF8ToUnicode(sjhm));
}

