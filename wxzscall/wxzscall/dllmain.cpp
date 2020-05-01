// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "wxzscall.h"
#include "postmsg.h"
#include "my_fun.h"

INT_PTR CALLBACK Dlgproc(HWND Arg1, UINT Arg2, WPARAM Arg3, LPARAM Arg4);
DWORD ProcessNameFind_Pid(LPCSTR ProcessName);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		DialogBox(hModule, MAKEINTRESOURCE(ID_WXZS), NULL, &Dlgproc);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

INT_PTR CALLBACK Dlgproc(HWND Arg1, UINT msg, WPARAM Arg3, LPARAM Arg4)
{
	WCHAR msgwxid[0x100] = { 0 };
	WCHAR postmsg[0x1000] = { 0 };
	//启动窗口前事件
	if (msg == WM_INITDIALOG)
	{
		//MessageBox(NULL,"aaa","11",0);
		//Get_Grxx(Arg1);
	}
	//关闭窗口事件
	if (msg == WM_CLOSE)
	{
		EndDialog(Arg1, 0);
	}
	//按钮事件，IN_DLL注册，DLL按钮
	
	if(msg == WM_COMMAND)
	{
		switch (Arg3)
		{
			//读取微信个人信息
		case ID_OK:
			Get_Grxx(Arg1);
			break;
			//发送微信消息
		case ID_MSGPOST:
			GetDlgItemText(Arg1, ID_MSGWXIDE, msgwxid, (int)msgwxid);
			GetDlgItemText(Arg1, ID_POSTMSGE, postmsg, (int)postmsg);
			Post_Msg(msgwxid,postmsg);
			//MessageBox(NULL, msgwxid, L"提示", 0);
			break;
		case ID_NOTOK:
			break;
		}
	}

	return FALSE;
}

