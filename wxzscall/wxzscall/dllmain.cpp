// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "wxzscall.h"

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
	//启动窗口前事件
	if (msg == WM_INITDIALOG)
	{
		//MessageBox(NULL,"aaa","11",0);
		//Get_Grxx(Arg1);
	}
	//关闭窗口事件
	else if (msg == WM_CLOSE)
	{
		EndDialog(Arg1, 0);
	}
	//按钮事件，IN_DLL注册，DLL按钮
	else if (Arg3 == ID_OK)
	{
		Get_Grxx(Arg1);
		//MessageBox(NULL, "22", "11", 0);
		//In_Dll_Fun();
	}
	//按钮事件，UN_DLL卸载，卸载DLL按钮
	else if (Arg3 == ID_NOTOK)
	{

	}


	return FALSE;
}

