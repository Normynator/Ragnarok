#include "stdafx.h"
#include "mod_overlay.h"

#include "hook_renderer.h"
#include "hook_session.h"

overlay::overlay(norm_dll::norm* c_state) : mod(c_state)
{
}

overlay::~overlay()
{
}

void overlay::draw_scene(void* this_obj)
{
	//enable this to debug: c_state->dbg_sock->do_send("draw_scene of overlay was called!");
	if (c_state->m_width == -1 || c_state->m_height == -1) {
		c_state->m_width = *(ULONG*)(*(DWORD*)(c_state->g_renderer) + 0x24);
		c_state->m_height = *(ULONG*)(*(DWORD*)(c_state->g_renderer) + 0x28);
	}

	DWORD tosf = get_TextOutScreen_Addr();
	if (this->display_ping && !this->display_grid) {
		char buf[32];
		sprintf_s(buf, "Ping: %ld ms", *(ULONG*)(c_state->g_session_m_averagePingTime));
		//((CRenderer__TextOutScreen)TextOutScreen_func)(this_obj, c_state->m_width - (c_state->m_width / 10) + 1, c_state->m_height - (c_state->m_height / 10) + 1, buf, 0x0000FF);
		((CRenderer__TextOutScreen)tosf)(this_obj, c_state->m_width - (c_state->m_width / 10), c_state->m_height - (c_state->m_height / 10), buf, 0x000000);
	}

	if (this->display_grid) {
		((CRenderer__TextOutScreen)tosf)(this_obj, 10, 10, (char*)"0%/0%", 0x0000FF);
		((CRenderer__TextOutScreen)tosf)(this_obj, 10, c_state->m_height / 2, (char*)"0%/50%", 0x0000FF);
		((CRenderer__TextOutScreen)tosf)(this_obj, 10, c_state->m_height - 40, (char*)"0%/100%", 0x0000FF);
		((CRenderer__TextOutScreen)tosf)(this_obj, c_state->m_width / 2, 10, (char*)"50%/0%", 0x0000FF);
		((CRenderer__TextOutScreen)tosf)(this_obj, c_state->m_width / 2, c_state->m_height / 2, (char*)"50%/50%", 0x0000FF);
		((CRenderer__TextOutScreen)tosf)(this_obj, c_state->m_width / 2, c_state->m_height - 40, (char*)"50%/100%", 0x0000FF);
		((CRenderer__TextOutScreen)tosf)(this_obj, c_state->m_width - 70, 10, (char*)"100%/0%", 0x0000FF);
		((CRenderer__TextOutScreen)tosf)(this_obj, c_state->m_width - 70, c_state->m_height / 2, (char*)"100%/50%", 0x0000FF);
		((CRenderer__TextOutScreen)tosf)(this_obj, c_state->m_width - 70, c_state->m_height - 40, (char*)"100%/100%", 0x0000FF);
	}
	
}

int overlay::get_talk_type(void **this_obj, void **src, int *a1, int *a2, int* retval)
{
	if (strcmp((char*)*src, "/pingo") == 0) {
		this->display_ping ^= 1;
		char buf[64];
		if (this->display_ping)
			sprintf_s(buf, "Ping is now displayed.");
		else
			sprintf_s(buf, "Ping is no longer displayed.");
		this->print_to_chat(buf);
		*retval = -1;
		return 1;
	}

	if (strcmp((char*)*src, "/grid") == 0) {
		this->display_grid ^= 1;
		char buf[64];
		if (this->display_grid)
			sprintf_s(buf, "Grid is now displayed.");
		else
			sprintf_s(buf, "Grid is no longer displayed.");
		this->print_to_chat(buf);
		*retval = -1;
		return 1;
	}

	return 0;
}

void overlay::register_hooks() {
	int res = 0;
	res += register_DrawScene_hook(shared_from_this());
	res += register_GetTalkType_hook(shared_from_this());
	this->validate(res, 2, __FILE__);
}
