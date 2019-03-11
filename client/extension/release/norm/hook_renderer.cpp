#include "stdafx.h"
#include "hook_renderer.h"
#include "detours.h"
#include <vector>

static std::shared_ptr<norm_dll::norm> c_state;
static std::vector<std::shared_ptr<norm_dll::mod>> DrawScene_callbacks;

#ifdef TEXTOUTSCREEN
#if CLIENT_VER == 20180620
DWORD DrawScene_func = 0x0043FA20;
DWORD TextOutScreen_func = 0x00443D00;
typedef int(__thiscall *CRenderer__TextOutScreen)(void*, int, int, LPCSTR, COLORREF);
typedef bool(__thiscall *CRenderer__DrawScene)(void*);
//typedef void*(__thiscall *makewindow)(void*, int);
//typedef int(__thiscall *UIFrameWnd__DrawItemText)(void*, int, int, int, int, int, int);
bool __fastcall DrawScene_hook(void* this_obj)
{

	bool res = ((CRenderer__DrawScene)DrawScene_func)(this_obj);

	for (std::shared_ptr<norm_dll::mod> callback : DrawScene_callbacks) {
		callback->draw_scene(this_obj);
	}

	//c_state->notify_stat_server();

	if (c_state->m_width != -1 && c_state->m_height != -1) {
		if (c_state->display_ping == 1 && c_state->display_grid == 0) {
			char buf[32];
			sprintf_s(buf, "Ping: %ld ms", *(ULONG*)(c_state->g_session_m_averagePingTime));
			//((CRenderer__TextOutScreen)TextOutScreen_func)(this_obj, c_state->m_width - (c_state->m_width / 10) + 1, c_state->m_height - (c_state->m_height / 10) + 1, buf, 0x0000FF);
			((CRenderer__TextOutScreen)TextOutScreen_func)(this_obj, c_state->m_width - (c_state->m_width / 10), c_state->m_height - (c_state->m_height / 10), buf, 0x000000);
		}

		if (c_state->display_grid == 1) {
			((CRenderer__TextOutScreen)TextOutScreen_func)(this_obj, 10, 10, (char*)"0%/0%", 0x0000FF);
			((CRenderer__TextOutScreen)TextOutScreen_func)(this_obj, 10, c_state->m_height / 2, (char*)"0%/50%", 0x0000FF);
			((CRenderer__TextOutScreen)TextOutScreen_func)(this_obj, 10, c_state->m_height - 40, (char*)"0%/100%", 0x0000FF);
			((CRenderer__TextOutScreen)TextOutScreen_func)(this_obj, c_state->m_width / 2, 10, (char*)"50%/0%", 0x0000FF);
			((CRenderer__TextOutScreen)TextOutScreen_func)(this_obj, c_state->m_width / 2, c_state->m_height / 2, (char*)"50%/50%", 0x0000FF);
			((CRenderer__TextOutScreen)TextOutScreen_func)(this_obj, c_state->m_width / 2, c_state->m_height - 40, (char*)"50%/100%", 0x0000FF);
			((CRenderer__TextOutScreen)TextOutScreen_func)(this_obj, c_state->m_width - 70, 10, (char*)"100%/0%", 0x0000FF);
			((CRenderer__TextOutScreen)TextOutScreen_func)(this_obj, c_state->m_width - 70, c_state->m_height / 2, (char*)"100%/50%", 0x0000FF);
			((CRenderer__TextOutScreen)TextOutScreen_func)(this_obj, c_state->m_width - 70, c_state->m_height - 40, (char*)"100%/100%", 0x0000FF);
		}
	}

	return res;
}
#endif
#endif

int register_DrawScene_hook(std::shared_ptr<norm_dll::mod> mod_ptr) {
	DrawScene_callbacks.push_back(mod_ptr);
	return 0;
}

int renderer_detour(std::shared_ptr<norm_dll::norm> c_state_) {
	int err = 0;
	int hook_count = 0;
	char info_buf[256];
	c_state = c_state_;

#ifdef TEXTOUTSCREEN
	err = DetourAttach(&(LPVOID&)DrawScene_func, &DrawScene_hook);
	CHECK(info_buf, err);
	c_state->dbg_sock->do_send(info_buf);
	hook_count++;
#endif

	sprintf_s(info_buf, "Render mods loaded: %d", hook_count);
	c_state->dbg_sock->do_send(info_buf);
	return hook_count;
}