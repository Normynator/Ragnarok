#include "stdafx.h"
#include "chat.h"
#include <time.h>
#include "detours.h"

/* debug */
std::shared_ptr<norm_dll::debug_socket> dbg_sock;

/* global values */
int welcome = 1;
int catch_invalid_message = 0;
int timestamp = 1;

#if CLIENT_VER == 20180620
#define CHAT_INPUT
DWORD chat_input_func = 0x00A0CF40;
typedef int(__thiscall *chat_input)(void*, void*, int, int);

// __thiscall to __fastcall workaround with EDX.
int __fastcall chat_input_hook(void *this_obj, DWORD EDX, void* a2, int a3, int a4)
{
#elif CLIENT_VER == 20150000
#define CHAT_INPUT
DWORD chat_input_func = 0x00925100;
typedef  signed int(__thiscall *chat_input)(void*, char*, int, char*);

// __thiscall to __fastcall workaround with EDX.
signed int __fastcall chat_input_hook(void *this_obj, DWORD EDX, char *a2, int a3, char *a4)
{
#endif
	chat_input original_chat_input = (chat_input)chat_input_func;

	if (a2 && strcmp((char*)a2, "/timestamp") == 0) {
		catch_invalid_message = 1;
	}

	return original_chat_input(this_obj, a2, a3, a4);
}

#if CLIENT_VER == 20180620
#define UIWindowMgr__SendMsg
DWORD UIWindowMgr_SendMsg_func = 0x0071ED80;
typedef int(__thiscall *UIWindowMgr_SendMsg)(void*, int, void*, void*, int, int);

int __fastcall UIWindowMgr_SendMsg_hook(void* this_obj, DWORD EDX, int a1, void* a2, void* a3, int a4, int a5)

#elif CLIENT_VER == 20150000
#define UIWindowMgr__SendMsg
DWORD UIWindowMgr_SendMsg_func = 0x005F4CA0;
typedef int(__thiscall *UIWindowMgr_SendMsg)(void*, int, int, int, int, int);

int __fastcall UIWindowMgr_SendMsg_hook(void* this_obj, DWORD EDX, int a1, int a2, int a3, int a4, int a5)
#else
// fallback to have valid function
int __fastcall UIWindowMgr_SendMsg_hook(void* this_obj, DWORD EDX, int a1, int a2, int a3, int a4, int a5)
#endif
{
	UIWindowMgr_SendMsg original_sendmsg = (UIWindowMgr_SendMsg)UIWindowMgr_SendMsg_func;
	char buf[256];
	char tmp_buf[256];
	if (a1 == 1) {
#ifdef DEBUG
		// some testing -- begin
		dbg_sock->do_send("UIWindowMgr_SendMsg: first arg was 1");
		sprintf_s(buf, "UIWindowMgr_SendMsg: %s", (char*)a2);
		dbg_sock->do_send(buf);

		if (strcmp((char*)a2, "Invalid Command") == 0) {
			sprintf_s(buf, "UIWindowMgr_SendMsg: %d %s %d %d %d", a1, (char*)a2, a3, a4, a5);
			dbg_sock->do_send(buf);
		}
		// some testing -- end
#endif
#if CLIENT_VER == 20150000
		if (welcome) {
			sprintf_s(buf, "Welcome to the Beta Client. Please report any bugs you find!");
			original_sendmsg(this_obj, a1, (int)&buf, a3, a4, a5);
			sprintf_s(buf, "- Timestamp is enabled by default. Use /timestamp to disable/enable.");
			original_sendmsg(this_obj, a1, (int)&buf, a3, a4, a5);
			welcome = 0;
		}
#endif
		if (catch_invalid_message && (strcmp((char*)a2, "Invalid Command") == 0)) {
			if (timestamp) {
				timestamp = 0;
				sprintf_s(tmp_buf, "Timestamp is now disabled!");
			}
			else {
				timestamp = 1;
				sprintf_s(tmp_buf, "Timestamp is now enabled!");
			}
#if CLIENT_VER == 20150000
			a2 = (int)&tmp_buf;
#elif CLIENT_VER == 20180620
			a2 = tmp_buf;
#endif
			catch_invalid_message = 0; // this only works if the client is not multithreaded.
		}
		// change the original message to include timestamp
		if (timestamp) {
			time_t t = time(NULL);
			struct tm buf_tm;
			char timestamp[10] = "";
			/* err = */ localtime_s(&buf_tm, &t);
			strftime(timestamp, 10, "%H:%M:%S", &buf_tm);
			sprintf_s(buf, "[%s] %s", timestamp, (char*)a2);
#if CLIENT_VER == 20150000
			a2 = (int)&buf;
#elif CLIENT_VER == 20180620
			a2 = buf;
#endif	
		}
	}
	return original_sendmsg(this_obj, a1, a2, a3, a4, a5);
}

void chat_detour(std::shared_ptr<norm_dll::debug_socket> dbg_sock_) {
	int err = 0;
	int hook_count = 0;
	char info_buf[256];
	dbg_sock = dbg_sock_;

#ifdef UIWindowMgr__SendMsg
	err = DetourAttach(&(LPVOID&)UIWindowMgr_SendMsg_func, &UIWindowMgr_SendMsg_hook);
	CHECK(info_buf, err);
	dbg_sock->do_send(info_buf);
	hook_count++;
#endif

#ifdef CHAT_INPUT
	err = DetourAttach(&(LPVOID&)chat_input_func, &chat_input_hook);
	CHECK(info_buf, err);
	dbg_sock->do_send(info_buf);
	hook_count++;
#endif

	sprintf_s(info_buf, "Mods loaded: %d", hook_count);
	dbg_sock->do_send(info_buf);
}
