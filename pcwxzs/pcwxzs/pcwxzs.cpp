// pcwxzs.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "pcwxzs.h"
#include <windows.h>
#include "resource.h"
#include  <TlHelp32.h>
#include <stdio.h>
#include <iostream>

//定义常量
#define WXPROCESS_NAME "WeChat.exe"
#define DLLNAME "wxzscall.dll"

//声明函数预编译
INT_PTR CALLBACK Dlgproc(HWND Arg1, UINT Arg2, WPARAM Arg3, LPARAM Arg4);
DWORD ProcessNameFind_Pid(LPCSTR ProcessName);
LPVOID In_Dll_Fun();
VOID Un_Dll_Fun(LPVOID IN_ADD);
LPVOID IN_ADD;
MODULEENTRY32 FindMoId(DWORD dwPid);

//主函数开始
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, &Dlgproc);
    return (int) 0;
}

//窗口回调函数
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
		LPVOID IN_ADDD = In_Dll_Fun();
	}
	//按钮事件，UN_DLL卸载，卸载DLL按钮
	else if (Arg3 == UN_DLL)
	{
		if (IN_ADD==0)
		{
			MessageBox(NULL, "获取注入dll地址失败", "提示", 0);
		}
		else
		{
			Un_Dll_Fun(IN_ADD);
		}
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
LPVOID In_Dll_Fun()
{
	//获取微信进程ID
	DWORD ProcessID = ProcessNameFind_Pid(WXPROCESS_NAME);
	if (ProcessID == 0)
	{
		MessageBox(NULL, "获取微信进程失败", "提示", 0);
		return 0;
	}
	//打开微信进程
	HANDLE OpenPid = OpenProcess(PROCESS_ALL_ACCESS,FALSE, ProcessID);
	if (OpenPid == NULL)
	{
		MessageBox(NULL, "打开微信进程失败", "提示", 0);
		return 0;
	}
	//DLL文件地址
	CHAR Dll_Str[0x100] = { "E://vs//wxhook//wxzs//wxzscall//Debug//wxzscall.dll" };

	//申请进程内存,返回基址
	LPVOID Dll_Add = VirtualAllocEx(OpenPid,NULL,strlen(Dll_Str),MEM_COMMIT,PAGE_READWRITE);
	if (Dll_Add==NULL)
	{
		MessageBox(NULL, "申请内存失败", "提示", 0);
		return 0;
	}
	//写入dll
	BOOL Is_True = WriteProcessMemory(OpenPid, Dll_Add, Dll_Str,strlen(Dll_Str),NULL);
	if (Is_True== FALSE)
	{
		MessageBox(NULL, "写入dll失败", "提示", 0);
		return 0;

	}
	//设置缓冲区
	CHAR temp[0x100] = { 0 };
	//打印十六进制地址
	sprintf_s(temp,"地址:%p",Dll_Add);
	OutputDebugString(temp);
	HMODULE K32 = GetModuleHandle("Kernel32.dll");
	LPVOID Load_Add=GetProcAddress(K32,"LoadLibraryA");
	HANDLE hRemoteThread = CreateRemoteThread(OpenPid, NULL, 0, (LPTHREAD_START_ROUTINE)Load_Add, Dll_Add, 0, NULL);
	if (hRemoteThread == NULL)
	{
		MessageBoxA(NULL, "远程线程注入失败", "错误", 0);
		return 0;
	}
	CloseHandle(hRemoteThread);
	CloseHandle(OpenPid);
	IN_ADD = Dll_Add;
	return  0;
}




//卸载dll函数
VOID Un_Dll_Fun(LPVOID IN_ADD)
{
	//获取微信进程ID
	DWORD ProcessID = ProcessNameFind_Pid(WXPROCESS_NAME);
	if (ProcessID == 0)
	{
		MessageBox(NULL, "获取微信进程失败", "提示", 0);
		return;
	}
	//打开微信进程
	HANDLE OpenPid = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
	if (OpenPid == NULL)
	{
		MessageBox(NULL, "打开微信进程失败", "提示", 0);
		return;
	}

	HMODULE K32 = GetModuleHandle("Kernel32.dll");
	LPVOID Load_Add = GetProcAddress(K32, "FreeLibrary");
	HANDLE hRemoteThread = CreateRemoteThread(OpenPid, NULL, 0, (LPTHREAD_START_ROUTINE)Load_Add, FindMoId(ProcessID).modBaseAddr, 0, NULL);
	if (hRemoteThread == NULL)
	{
		MessageBoxA(NULL, "远程线程卸载失败", "错误", 0);
		return;
	}
	CloseHandle(hRemoteThread);
	CloseHandle(OpenPid);
}


//获取模块基址
MODULEENTRY32 FindMoId(DWORD dwPid)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	MODULEENTRY32 ME32 = { 0 };
	ME32.dwSize = sizeof(MODULEENTRY32);
	BOOL isNext = Module32First(hSnap, &ME32);
	BOOL flag = FALSE;
	while (isNext)
	{
		if (strcmp(ME32.szModule, DLLNAME) == 0)
		{
			flag = TRUE;
			break;
		}
		isNext = Module32Next(hSnap, &ME32);
	}
	return ME32;
	if (flag == FALSE)
	{
		MessageBoxA(NULL, "找不到目标模块", "错误", 0);
		return ME32;
	}

}
