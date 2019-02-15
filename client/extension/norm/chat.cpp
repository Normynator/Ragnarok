#include "stdafx.h"
#include "chat.h"
#include "detours.h"

/* debug */
std::shared_ptr<norm_dll::debug_socket> dbg_sock;

/* Counter for chat_hist_hook calls */
int count_chh = 0;
int count_cih = 0;

/* Some bool constants to run code only once. */
bool ttt_vector_check = true;

#if CLIENT_VER == 20180620

#define CHAT_INPUT
DWORD chat_input_func = 0x00A0CF40;
typedef int(_stdcall *chat_input)(void*, int, int);

// __thiscall to __fastcall workaround with EDX.
signed int __fastcall chat_input_hook(void *this_obj, DWORD EDX, int a1, int a2) //wrong header
{
	chat_input orignal_chat_input = (chat_input)chat_input_func;
	return orignal_chat_input(this_obj, a1, a2);
}

#endif

#if CLIENT_VER == 20150000

#define CHAT_HIST
DWORD chat_hist_func = 0x004C1880;
DWORD ttt_vector = 0x00E1357C;
DWORD ttt_vector_end = 0x00E13580;
typedef void(__thiscall *chat_hist)(HDC hdc_obj);

void check_ttt_vector() {
	//char* first_entry = (char*)*((DWORD*)*((DWORD*)ttt_vector));
	DWORD first_entry = *((DWORD*)ttt_vector);
	DWORD tmp = (DWORD)*((DWORD*)ttt_vector_end) - (DWORD)*((DWORD*)ttt_vector);
	DWORD ttt_size = tmp / 8;
	/*
		TYPE: std::vector_std::pair_char_const___enum_TALKTYPE__std::allocator_std::pair_char_const___enum_TALKTYPE_____

		std::vector<std::pair<const char, int>> test_ttt = (std::vector<std::pair<const char, int>>)(ttt_vector);
		std::pair<const char, int> tmp = test_ttt.front();
	*/
	char buf[256];
	sprintf_s(buf, "ttt_vector size: %lu", ttt_size);
	dbg_sock->do_send(buf);

	for (int i = 0; i < ttt_size; i++) {
		sprintf_s(buf, "0x%lx | %d. ttt_vector entry: Command: %s, \t\tTalktype: %d,", (first_entry + i * 8), (i + 1), (char*)*(DWORD*)(first_entry + i * 8), (uint32_t)*(DWORD*)(first_entry + i * 8 + 4));
		dbg_sock->do_send(buf);
	}
}

void __fastcall chat_hist_hook(HDC hdc_obj)
{
	/*
		We should be ingame now.
		We can try and check the vector of CSession::InitTalkTypeTable
	*/
	if (ttt_vector_check) {
		ttt_vector_check = false;
		check_ttt_vector();
	}

	/* Doing actual chat related things. */
	chat_hist original_chat_hist = (chat_hist)chat_hist_func;
	char buf[256];

	// call counter
	count_chh++;
	sprintf_s(buf, "chat_hist_hook called. %dth time!", count_chh);
	dbg_sock->do_send(buf);

	DWORD chat_addr = (DWORD)hdc_obj + 0x70;
	DWORD chat_addr_end = (DWORD)hdc_obj + 0x74;
	DWORD first_entry = *((DWORD*)(chat_addr));
	DWORD tmp = (DWORD)*((DWORD*)chat_addr_end) - (DWORD)*((DWORD*)chat_addr);
	DWORD chat_size = tmp / 24;

	sprintf_s(buf, "chat_hist size: %lu", chat_size);
	dbg_sock->do_send(buf);

	for (int i = 0; i < chat_size; i++) {
		char* msg;
		int msg_len = (int)*(DWORD*)(first_entry + i * 24 + 0x10);
		// seems to be length related.
		int unk_val = (int)*(DWORD*)(first_entry + i * 24 + 0x14);
		// The client is storing message with len < 0x10 NOT as a char pointer.
		if (msg_len < 0x10) {
			msg = (char*)(first_entry + i * 24);
		}
		else {
			msg = (char*)*(DWORD*)(first_entry + i * 24);
		}
		sprintf_s(buf, "0x%lx | unk_val: %d | length: %d | %s", (first_entry + i * 24), unk_val, msg_len, msg);
		dbg_sock->do_send(buf);
	}

	/* Call original function */
	original_chat_hist(hdc_obj);
}

#define CHAT_INPUT
DWORD chat_input_func = 0x00925100;
typedef  signed int(__thiscall *chat_input)(void*, char*, int, char*);

// __thiscall to __fastcall workaround with EDX.
signed int __fastcall chat_input_hook(void *this_obj, DWORD EDX, char *a2, int a3, char *a4)
{
	chat_input orignal_chat_input = (chat_input)chat_input_func;
	char buf[256];

	// call counter
	count_cih++;
	sprintf_s(buf, "chat_input_hook called. %dth time!", count_cih);
	dbg_sock->do_send(buf);

	if (a2 && strcmp(a2, "/test") == 0) {
		dbg_sock->do_send("/test command was executed!");
	}

	return orignal_chat_input(this_obj, a2, a3, a4);
}
#endif

void chat_detour(std::shared_ptr<norm_dll::debug_socket> dbg_sock_) {
	int err = 0;
	char info_buf[256];
	dbg_sock = dbg_sock_;

#ifdef CHAT_INPUT
	err = DetourAttach(&(LPVOID&)chat_input_func, &chat_input_hook);
	CHECK(info_buf, err);
	dbg_sock->do_send(info_buf);
#endif

#ifdef CHAT_HIST
	err = DetourAttach(&(LPVOID&)chat_hist_func, &chat_hist_hook);
	CHECK(info_buf, err);
	dbg_sock->do_send(info_buf);
#endif
}
