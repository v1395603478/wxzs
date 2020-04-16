// pcwxzs.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "pcwxzs.h"
#include <windows.h>
#include "resource.h"
#include  <TlHelp32.h>
#include <stdio.h>

#define WXPROCESS_NAME "WeChat.exe"

INT_PTR CALLBACK Dlgproc(HWND Arg1, UINT Arg2, WPARAM Arg3, LPARAM Arg4);
DWORD ProcessNameFind_Pid(LPCSTR ProcessName);
VOID In_Dll_Fun();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, &Dlgproc);
    return (int) 0;
}

INT_PTR CALLBACK Dlgproc(HWND Arg1,UINT msg,WPARAM Arg3,LPARAM Arg4)
{
	//启动窗口前事件
	if (msg == WM_INITDIALOG)
	{
		//MessageBox(NULL,"aaa","11",0);
	}
	//关闭窗口事件
	else if (msg == WM_CLOSE)
	{
		EndDialog(Arg1,0);
	}
	//按钮事件，IN_DLL注册，DLL按钮
	else if (Arg3 == IN_DLL)
	{
		//MessageBox(NULL, "aaa", "11", 0);
		In_Dll_Fun();
	}
	//按钮事件，UN_DLL卸载，卸载DLL按钮
	else if (Arg3 == UN_DLL)
	{
	
	}


	return FALSE;
}

//通过进程名称获取进程ID
DWORD ProcessNameFind_Pid(LPCSTR ProcessName)
{
	HANDLE ProcessAll = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 Process = { 0 };
	Process.dwSize = sizeof(PROCESSENTRY32);
	//遍历进程，查找微信进程，获取进程ID
	do
	{
		if (strcmp(Process.szExeFile, ProcessName)==0)
		{
			return Process.th32ProcessID;
		}

	} while (Process32Next(ProcessAll,&Process));
	return 0;
}

//注入dll函数
VOID In_Dll_Fun() 
{
	//获取微信进程ID
	DWORD ProcessID = ProcessNameFind_Pid(WXPROCESS_NAME);
	if (ProcessID == 0)
	{
		MessageBox(NULL, "获取微信进程失败", "提示", 0);
		return;
	}
	//打开微信进程
	HANDLE OpenPid = OpenProcess(PROCESS_ALL_ACCESS,FALSE, ProcessID);
	if (OpenPid == NULL)
	{
		MessageBox(NULL, "打开微信进程失败", "提示", 0);
		return;
	}
	//DLL文件地址
	CHAR Dll_Str[0x100] = { "E://vs//wxhook//wxzs//wxzscall//Debug//wxzscall.dll" };

	//申请进程内存,返回基址
	LPVOID Dll_Add = VirtualAllocEx(OpenPid,NULL,strlen(Dll_Str),MEM_COMMIT,PAGE_READWRITE);
	if (Dll_Add==NULL)
	{
		MessageBox(NULL, "申请内存失败", "提示", 0);
		return;
	}
	//写入dll
	BOOL Is_True = WriteProcessMemory(OpenPid, Dll_Add, Dll_Str,strlen(Dll_Str),NULL);
	if (Is_True== FALSE)
	{
		MessageBox(NULL, "写入dll失败", "提示", 0);
		return;

	}
	//设置缓冲区
	CHAR temp[0x100] = { 0 };
	//打印十六进制地址
	sprintf_s(temp,"地址:%p",Dll_Add);
	OutputDebugString(temp);
	HMODULE K32 = GetModuleHandle("Kernel32.dll");
	LPVOID Load_Add=GetProcAddress(K32,"LoadLibraryA");
	CreateRemoteThread(OpenPid, NULL, 0, (LPTHREAD_START_ROUTINE)Load_Add, Dll_Add, 0, NULL);
}
//卸载dll函数
VOID Un_Dll_Fun()
{
//
}