#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include <stdlib.h>
#include "my_fun.h"
#define POSTWXMSG_CALL 0x2FA7F0

VOID Post_Msg(wchar_t * WxId, wchar_t * PostMsg)
{

	//��װwxid�ṹ��
	wxid_struct wxid = { 0 };
	wxid.wxid = WxId;
	wxid.wxidLen = wcslen(WxId);
	wxid.buffLen = wcslen(WxId) * 2;

	//��װ��������PostMsg�ṹ��
	wxmsg_struct msg = { 0 };
	msg.wxmsg = PostMsg;
	msg.msgLen = wcslen(PostMsg);
	msg.buffLen = wcslen(PostMsg) * 2;

	//ȡ����ַת��char����
	char * pwxid = (char *)&wxid.wxid;
	char * pmsg = (char *)&msg.wxmsg;
	//����0x81C��С������buff 
	char  buff[0x81C] = { 0 };

	//	pushad
	//	mov edx, [wxidg]
	//	push 0x1
	//	push 0x0
	//	mov ebx, [msgg]
	//	push ebx
	//	mov ecx, [buff]
	//	call[callg]
	//	add esp, 0xC
	//	popad

	//������Ϣcall��ַ
	DWORD call_add = Get_WeChatWin() + POSTWXMSG_CALL;
	_asm
	{
		pushad
		mov edx,pwxid
		push 0x1
		push 0x0
		mov ebx,pmsg
		push ebx
		lea ecx,buff
		call call_add
		add esp,0xC
		popad
	}


	return;

}