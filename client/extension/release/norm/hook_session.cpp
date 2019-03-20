#include "stdafx.h"
#include "hook_session.h"
#include "detours.h"


static std::shared_ptr<norm_dll::norm> c_state;
//static std::vector<std::shared_ptr<norm_dll::mod>> GetTalkType_callbacks;

bool GetTalkType_detoured = false;

#if CLIENT_VER == 20180620
#define GETTALKTYPE
DWORD GetTalkType_func = 0x00A0CF40;
typedef int(__thiscall *GetTalkType)(void*, void*, int, int);

// __thiscall to __fastcall workaround with EDX.
int __fastcall GetTalkType_hook(void *this_obj, DWORD EDX, void* a2, int a3, int a4)
{
#elif CLIENT_VER == 20150000
#define CHAT_INPUT
DWORD GetTalkType_func = 0x00925100;
typedef  signed int(__thiscall *GetTalkType)(void*, char*, int, char*);

// __thiscall to __fastcall workaround with EDX.
signed int __fastcall GetTalkType_hook(void *this_obj, DWORD EDX, char *a2, int a3, char *a4)
{
#endif
	GetTalkType original_GetTalkType = (GetTalkType)GetTalkType_func;

	int cret = 0;
	int retval = 0;
	//for (auto callback : GetTalkType_callbacks)
	for (auto callback : c_state->mods)
		cret += callback->get_talk_type(&this_obj, &a2, &a3, &a4, &retval);

	if (cret == 1)
		return retval;

	if (cret > 1)
		c_state->dbg_sock->do_send("Error: Multiple GetTalkType hooks want to return a value.");

	return original_GetTalkType(this_obj, a2, a3, a4);
}

/*int register_GetTalkType_hook(std::shared_ptr<norm_dll::mod> mod_ptr) {
#ifdef GETTALKTYPE
	if (!GetTalkType_detoured)
		return 0;
	//GetTalkType_callbacks.push_back(mod_ptr);
	return 1;
#else
	return 0;
#endif
}*/

int session_detour(std::shared_ptr<norm_dll::norm> state_) {
	LONG err = 0;
	int hook_count = 0;
	char info_buf[256];
	c_state = state_;

#ifdef GETTALKTYPE
	err = DetourAttach(&(LPVOID&)GetTalkType_func, &GetTalkType_hook);
	CHECK(info_buf, err);
	if (err == NO_ERROR) {
		GetTalkType_detoured = true;
		hook_count++;
	} else 
		c_state->dbg_sock->do_send(info_buf);
#endif

	sprintf_s(info_buf, "Session hooks available: %d", hook_count);
	c_state->dbg_sock->do_send(info_buf);

	return hook_count;
}
