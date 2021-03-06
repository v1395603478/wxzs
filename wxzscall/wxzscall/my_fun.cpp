#include "stdafx.h"
#include <windows.h>
#include <stdlib.h >
#define WECHATWIN L"WeChatWin.dll"


DWORD Get_WeChatWin()
{
	HMODULE WeChatWin = LoadLibrary(WECHATWIN);
	return (DWORD)WeChatWin;
}

//************************************************************
// 函数名称: UTF8ToUnicode
// 函数说明: 将UTF8编码转为Unicode(微信默认编码为UTF8)
// 作    者: GuiShou
// 时    间: 2019/7/7
// 参    数: str 需要转换的字符串
// 返 回 值: wchar_t 返回的字符串 
//************************************************************
wchar_t * UTF8ToUnicode(const char* str)
{
	int    textlen = 0;
	wchar_t * result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t *)malloc((textlen + 1) * sizeof(wchar_t));
	memset(result, 0, (textlen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
	return    result;
}

