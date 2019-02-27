#include "stdafx.h"
#include "chat.h"
#include <time.h>
#include "detours.h"

/* debug */
std::shared_ptr<norm_dll::debug_socket> dbg_sock;

#if CLIENT_VER == 20180620
#define UIWindowMgr__SendMsg
DWORD UIWindowMgr_SendMsg_func = 0x0071ED80;
typedef int(__thiscall *UIWindowMgr_SendMsg)(void*, int, void*, void*, int, int);

int __fastcall UIWindowMgr_SendMsg_hook(void* this_obj, DWORD EDX, int a1, void* a2, void* a3, int a4, int a5)

#elif CLIENT_VER == 20150000
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
	if (a1 == 1) {
#if DEBUG
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

		// change the original message to include timestamp
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

	sprintf_s(info_buf, "Mods loaded: %d", hook_count);
	dbg_sock->do_send(info_buf);
}
