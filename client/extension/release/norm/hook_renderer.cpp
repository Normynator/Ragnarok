#include "stdafx.h"
#include "hook_renderer.h"
#include "detours.h"
#include <vector>

static std::shared_ptr<norm_dll::norm> c_state;
static std::vector<std::shared_ptr<norm_dll::mod>> DrawScene_callbacks;

#ifdef TEXTOUTSCREEN
#if CLIENT_VER == 20180620
DWORD DrawScene_Addr = 0x0043FA20;
//typedef void*(__thiscall *makewindow)(void*, int);
//typedef int(__thiscall *UIFrameWnd__DrawItemText)(void*, int, int, int, int, int, int);
bool __fastcall DrawScene_hook(void* this_obj)
{
	DWORD target = get_DrawScene_Addr();
	bool res = ((CRenderer__DrawScene)target)(this_obj);
	for (std::shared_ptr<norm_dll::mod> callback : DrawScene_callbacks) {
		callback->draw_scene(this_obj);
	}
	return res;
}

DWORD get_DrawScene_Addr()
{
	return DrawScene_Addr;
}

DWORD get_TextOutScreen_Addr()
{
	return 0x00443D00;
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
	err = DetourAttach(&(LPVOID&)DrawScene_Addr, &DrawScene_hook);
	CHECK(info_buf, err);
	c_state->dbg_sock->do_send(info_buf);
	hook_count++;
#endif

	sprintf_s(info_buf, "Render mods loaded: %d", hook_count);
	c_state->dbg_sock->do_send(info_buf);
	return hook_count;
}