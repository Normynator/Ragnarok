#include "stdafx.h"
#include "mod_overlay.h"
#include "hook_renderer.h"
#include "hook_chat.h"

overlay::overlay(norm_dll::norm* c_state) : mod(c_state)
{
}

overlay::~overlay()
{
}

void overlay::draw_scene(void* this_obj)
{
	//enable this to debug: c_state->dbg_sock->do_send("draw_scene of overlay was called!");
	DWORD tosf = get_TextOutScreen_Addr();
	if (c_state->m_width != -1 && c_state->m_height != -1) {
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
}

void overlay::register_hooks() {
	register_DrawScene_hook(shared_from_this());
	register_SendMsg_hook(shared_from_this());
}
