#include "stdafx.h"
#include "norm.h"
#include "mod.h"

#include "hook_chat.h"


void norm_dll::mod::no_override()
{
		c_state->dbg_sock->do_send("Missing function override.");
}

void norm_dll::mod::print_to_chat(char * msg)
{
	DWORD smsg_addr = get_SendMsg_addr();
	DWORD color = 0x00FFFF;
	if (smsg_addr)
		((UIWindowMgr_SendMsg)smsg_addr)((void*)c_state->g_window_mgr, UIM_PUSHINTOCHATHISTORY, msg, &color, 0, 0);
	else
		c_state->dbg_sock->do_send("print_to_chat failed.");
}

void norm_dll::mod::validate(int result, int expected, const char* file)
{
	if (result != expected) {
		char buf[256];
		sprintf_s(buf, "Registering hook(s) failed! %s", file);
		c_state->dbg_sock->do_send(buf);
		this->active = 0;
	}
	else
		this->active = 1;
}
