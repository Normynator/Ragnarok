#pragma once
#include "stdafx.h"
#include "client_ver.h"
#include "debug_socket.h"
#include "norm.h"
#include <Windows.h>

/* chat: contains all hooks related to the ingame chat system */

//int register_SendMsg_hook(std::shared_ptr<norm_dll::mod> mod_ptr);

/* hooks all available functions. */
int chat_detour(std::shared_ptr<norm_dll::norm> c_state);

#if CLIENT_VER == 20180620
typedef int(__thiscall *UIWindowMgr_SendMsg)(void*, int, void*, void*, int, int);
#endif

DWORD get_SendMsg_addr();


/* Enum definitions from the client */
typedef enum UIM {
	UIM_CREATESTATUSWND = 0x0,
	UIM_PUSHINTOCHATHISTORY = 0x1,
	UIM_CHANGECHATWNDSTATUS = 0x2,
	UIM_OPENCHATWND = 0x3,
	UIM_LOADINGPERCENT = 0x4,
	UIM_PUSH_INTO_CHATROOM = 0x5,
	UIM_SAVE_CHAT_TO_FILE = 0x6,
	UIM_SAVE_CHAT_TO_FILE_FROM_CHATROOM = 0x7,
	UIM_IME_OPENCANDIDATE = 0x8,
	UIM_IME_CHANGECANDIDATE = 0x9,
	UIM_IME_CLOSECANDIDATE = 0xa,
	UIM_IME_STARTCOMPOSITION = 0xb,
	UIM_IME_COMPSTR = 0xc,
	UIM_IME_ENDCOMPOSITION = 0xd,
	UIM_MAKE_WHISPER_WINDOW = 0xe,
	UIM_CURSOR_CHANGE_ACTION = 0xf,
	UIM_CURSOR_CHANGE_MOTION = 0x10,
	UIM_REPORT_CHAT_TO_FILE = 0x11,
	UIM_REPORT_CHAT_TO_FILE_FROM_CHATROOM = 0x12,
	UIM_CLOSE_GRON_MESSENGER = 0x13,
	UIM_PUSHINTOCHATHISTORY2 = 0x14,
} UIM;

