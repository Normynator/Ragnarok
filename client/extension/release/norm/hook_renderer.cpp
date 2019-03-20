#include "stdafx.h"
#include "hook_renderer.h"
#include "detours.h"
#include <vector>

static std::shared_ptr<norm_dll::norm> c_state;
//static std::vector<std::shared_ptr<norm_dll::mod>> DrawScene_callbacks;

bool DrawScene_detoured = false;

#if CLIENT_VER == 20180620
#define DRAWSCENE
DWORD DrawScene_Addr = 0x0043FA20;
//typedef void*(__thiscall *makewindow)(void*, int);
//typedef int(__thiscall *UIFrameWnd__DrawItemText)(void*, int, int, int, int, int, int);
bool __fastcall DrawScene_hook(void* this_obj)
{
	//DWORD target = DrawScene_Addr;
	bool res = ((CRenderer__DrawScene)DrawScene_Addr)(this_obj);
	//for (std::shared_ptr<norm_dll::mod> callback : DrawScene_callbacks) {
	for (auto callback : c_state->mods)
		callback->draw_scene(this_obj);
	//}
	return res;
}
#endif

/*DWORD get_DrawScene_Addr()
{
	return DrawScene_Addr;
}*/

DWORD get_TextOutScreen_Addr()
{
	return 0x00443D00;
}

/*int register_DrawScene_hook(std::shared_ptr<norm_dll::mod> mod_ptr) {
#ifdef DRAWSCENE
	if (!DrawScene_detoured)
		return 0;
	//DrawScene_callbacks.push_back(mod_ptr);
	return 1;
#else
	return 0;
#endif
}*/

int renderer_detour(std::shared_ptr<norm_dll::norm> c_state_) {
	LONG err = 0;
	int hook_count = 0;
	char info_buf[256];
	c_state = c_state_;

#ifdef DRAWSCENE
	err = DetourAttach(&(LPVOID&)DrawScene_Addr, &DrawScene_hook);
	CHECK(info_buf, err);
	if (err == NO_ERROR) {
		DrawScene_detoured = true;
		hook_count++;
	} else
		c_state->dbg_sock->do_send(info_buf);
#endif

	sprintf_s(info_buf, "Renderer hooks available: %d", hook_count);
	c_state->dbg_sock->do_send(info_buf);
	return hook_count;
}