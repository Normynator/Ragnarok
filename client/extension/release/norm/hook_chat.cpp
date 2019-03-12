#include "stdafx.h"

#include "hook_chat.h"
#include "detours.h"

static std::shared_ptr<norm_dll::norm> c_state;
static std::vector<std::shared_ptr<norm_dll::mod>> SendMsg_callbacks;

bool SendMsg_detoured = false;

#if CLIENT_VER == 20180620
#define SENDMSG
DWORD UIWindowMgr_SendMsg_func = 0x0071ED80;
int __fastcall UIWindowMgr_SendMsg_hook(void* this_obj, DWORD EDX, int a1, void* a2, void* a3, int a4, int a5)

#elif CLIENT_VER == 20150000
#define SENDMSG
DWORD UIWindowMgr_SendMsg_func = 0x005F4CA0;
typedef int(__thiscall *UIWindowMgr_SendMsg)(void*, int, int, int, int, int);

int __fastcall UIWindowMgr_SendMsg_hook(void* this_obj, DWORD EDX, int a1, int a2, int a3, int a4, int a5)
#else
// fallback to have valid function
int __fastcall UIWindowMgr_SendMsg_hook(void* this_obj, DWORD EDX, int a1, int a2, int a3, int a4, int a5)
#endif
{
	UIWindowMgr_SendMsg original_sendmsg = (UIWindowMgr_SendMsg)UIWindowMgr_SendMsg_func;
/*
#if CLIENT_VER == 20150000
		if (welcome) {
			sprintf_s(buf, "Welcome to the Beta Client. Please report any bugs you find!");
			original_sendmsg(this_obj, a1, (int)&buf, a3, a4, a5);
			sprintf_s(buf, "- Timestamp is enabled by default. Use /timestamp to disable/enable.");
			original_sendmsg(this_obj, a1, (int)&buf, a3, a4, a5);
			welcome = 0;
		}
#endif

			case DIT:
			{
				/*typedef void*(__thiscall *makewindow)(void*, int);
				typedef int(__thiscall *UIFrameWnd__DrawItemText)(void*, int, int, int, int, int, int);
				void* g_windowMgr = (void*)0x00e6ec88;
				DWORD ret = *(DWORD*)((makewindow)0x711d20)(g_windowMgr, 12);
				((UIFrameWnd__DrawItemText)(*(DWORD*)(ret + 0x88)))((void*)ret, 0, 0x19, 0, 0, 0, 0);
				
			}
*/
	if (c_state->g_window_mgr == 0 && a1 == 1)
		c_state->g_window_mgr = (DWORD)this_obj;
	for (auto callback : SendMsg_callbacks)
		callback->send_msg(&this_obj, &a1, &a2, &a3, &a4, &a5);

	return original_sendmsg(this_obj, a1, a2, a3, a4, a5);
}

int register_SendMsg_hook(std::shared_ptr<norm_dll::mod> mod_ptr) {
#ifdef SENDMSG
	if (!SendMsg_detoured)
		return 0;
	SendMsg_callbacks.push_back(mod_ptr);
	return 1;
#else
	return 0;
#endif
}

DWORD get_SendMsg_addr() {
#if CLIENT_VER == 20180620
	return 0x0071ED80;
#endif
	return 0;
}

int chat_detour(std::shared_ptr<norm_dll::norm> state_) {
	LONG err = 0;
	int hook_count = 0;
	char info_buf[256];
	c_state = state_;

#ifdef SENDMSG
	err = DetourAttach(&(LPVOID&)UIWindowMgr_SendMsg_func, &UIWindowMgr_SendMsg_hook);
	CHECK(info_buf, err);
	if (err == NO_ERROR) {
		SendMsg_detoured = true;
		hook_count++;
	} else 
		c_state->dbg_sock->do_send(info_buf);
#endif

	sprintf_s(info_buf, "Chat hooks available: %d", hook_count);
	c_state->dbg_sock->do_send(info_buf);

	return hook_count;
}