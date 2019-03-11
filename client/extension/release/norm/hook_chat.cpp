#include "stdafx.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "hook_chat.h"
#include <time.h>
#include "detours.h"

static std::shared_ptr<norm_dll::norm> c_state;

/* global values */
int welcome = 1;
int catch_invalid_message = 0;
int last_command = -1;
int timestamp = 1;


#ifdef CHAT_INPUT
#if CLIENT_VER == 20180620
DWORD chat_input_func = 0x00A0CF40;
typedef int(__thiscall *chat_input)(void*, void*, int, int);

// __thiscall to __fastcall workaround with EDX.
int __fastcall chat_input_hook(void *this_obj, DWORD EDX, void* a2, int a3, int a4)
{
#elif CLIENT_VER == 20150000
DWORD chat_input_func = 0x00925100;
typedef  signed int(__thiscall *chat_input)(void*, char*, int, char*);

// __thiscall to __fastcall workaround with EDX.
signed int __fastcall chat_input_hook(void *this_obj, DWORD EDX, char *a2, int a3, char *a4)
{
#endif
	chat_input original_chat_input = (chat_input)chat_input_func;

	if (a2) {
		if (strcmp((char*)a2, "/timestamp") == 0) {
			last_command = TIMESTAMP;
			catch_invalid_message = 1;
		}
		else if (strcmp((char*)a2, "/pingo") == 0) {
			last_command = PINGO;
			catch_invalid_message = 1;
		}
		else if (strcmp((char*)a2, "/grid") == 0) {
			last_command = GRID;
			catch_invalid_message = 1;
		}
		else if (strcmp((char*)a2, "/drawitemtext") == 0) {
			last_command = DIT;
			catch_invalid_message = 1;
		}
	}

	return original_chat_input(this_obj, a2, a3, a4);
}
#endif

#ifdef UIWindowMgr__SendMsg
#if CLIENT_VER == 20180620
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
		c_state->dbg_sock->do_send("UIWindowMgr_SendMsg: first arg was 1");
		sprintf_s(buf, "UIWindowMgr_SendMsg: %s", (char*)a2);
		c_state->dbg_sock->do_send(buf);

		if (strcmp((char*)a2, "Invalid Command") == 0) {
			sprintf_s(buf, "UIWindowMgr_SendMsg: %d %s %d %d %d", a1, (char*)a2, a3, a4, a5);
			c_state->dbg_sock->do_send(buf);
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
		if (last_command != -1 && catch_invalid_message && (strcmp((char*)a2, "Invalid Command") == 0)) {
			switch(last_command) {
			case TIMESTAMP:
				if (timestamp) {
					timestamp = 0;
					sprintf_s(tmp_buf, "Timestamp is now disabled!");
				}
				else {
					timestamp = 1;
					sprintf_s(tmp_buf, "Timestamp is now enabled!");
				}
				break;
			case PINGO:
				{
					//init vars
					c_state->m_width = *(ULONG*)(*(DWORD*)(c_state->g_renderer) + 0x24);
					c_state->m_height = *(ULONG*)(*(DWORD*)(c_state->g_renderer) + 0x28);
					sprintf_s(tmp_buf, "Width: %d | Height: %d", c_state->m_width, c_state->m_height);
					c_state->dbg_sock->do_send("Pingo catched!");
					c_state->display_ping = 1;
				}
				break;
			case GRID:
				if (c_state->display_grid) {
					c_state->display_grid = 0;
					sprintf_s(tmp_buf, "Grid is now disabled.");
				}
				else {
					c_state->display_grid = 1;
					sprintf_s(tmp_buf, "Grid is now enabled.");
				}
				break;
			case DIT:
			{
				/*typedef void*(__thiscall *makewindow)(void*, int);
				typedef int(__thiscall *UIFrameWnd__DrawItemText)(void*, int, int, int, int, int, int);
				void* g_windowMgr = (void*)0x00e6ec88;
				DWORD ret = *(DWORD*)((makewindow)0x711d20)(g_windowMgr, 12);
				((UIFrameWnd__DrawItemText)(*(DWORD*)(ret + 0x88)))((void*)ret, 0, 0x19, 0, 0, 0, 0);
				*/
			}
			break;
			default:
				sprintf_s(tmp_buf, "Norm: Command error, please report this.");
				c_state->dbg_sock->do_send("Invalid switch case!");
			}
#if CLIENT_VER == 20150000
			a2 = (int)&tmp_buf;
#elif CLIENT_VER == 20180620
			a2 = tmp_buf;
#endif		
			last_command = -1;
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
#endif

int chat_detour(std::shared_ptr<norm_dll::norm> state_) {
	int err = 0;
	int hook_count = 0;
	char info_buf[256];
	c_state = state_;

#ifdef UIWindowMgr__SendMsg
	err = DetourAttach(&(LPVOID&)UIWindowMgr_SendMsg_func, &UIWindowMgr_SendMsg_hook);
	CHECK(info_buf, err);
	c_state->dbg_sock->do_send(info_buf);
	hook_count++;
#endif

#ifdef CHAT_INPUT
	err = DetourAttach(&(LPVOID&)chat_input_func, &chat_input_hook);
	CHECK(info_buf, err);
	c_state->dbg_sock->do_send(info_buf);
	hook_count++;
#endif

	sprintf_s(info_buf, "Chat mods loaded: %d", hook_count);
	c_state->dbg_sock->do_send(info_buf);

	return hook_count;
}