#include "stdafx.h"
#include "detours.h"
#include "norm.hpp"
#include <Windows.h>
#include <vector>
#include "debug_socket.h"
#include <csignal> // Test to catch singals, to get some debug infos.

/* Globaly required variables */
norm_dll::debug_socket dbg_sock;

/* Counter for chat_hist_hook calls */
int count_chh = 0;

/* Some bool constants to run code only once. */
bool ttt_vector_check = true;

/* Memory addresses */
// Chat history function
DWORD chat_hist_func = 0x004C1880;
// TalkTypeTableVector
DWORD ttt_vector = 0x00E1357C;
DWORD ttt_vector_end = 0x00E13580;

/* typedefs for hooks*/
typedef void(__thiscall *chat_hist)(HDC hdc_obj);

/* hook related functions */
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
	dbg_sock.do_send(buf);

	for (int i = 0; i < ttt_size; i++) {
		sprintf_s(buf, "0x%lx | %d. ttt_vector entry: Command: %s, \t\tTalktype: %d,", (first_entry + i * 8), (i+1), (char*)*(DWORD*)(first_entry + i * 8), (uint32_t)*(DWORD*)(first_entry + i * 8 + 4));
		dbg_sock.do_send(buf);
	}
}

/* hooked functions */
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
	dbg_sock.do_send(buf);

	DWORD chat_addr = (DWORD)hdc_obj + 0x70;
	DWORD chat_addr_end = (DWORD)hdc_obj + 0x74;
	DWORD first_entry = *((DWORD*)(chat_addr));
	DWORD tmp = (DWORD)*((DWORD*)chat_addr_end) - (DWORD)*((DWORD*)chat_addr);
	DWORD chat_size = tmp / 24;

	sprintf_s(buf, "chat_hist size: %lu", chat_size);
	dbg_sock.do_send(buf);

	for (int i = 0; i < chat_size; i++) {
		int msg_len = (int)*(DWORD*)(first_entry + i * 24 + 0x10);
		char* msg;
		// The client is storing message with len < 0x10 NOT as a char pointer.
		if (msg_len < 0x10) {
			msg = (char*)(first_entry + i * 24);
		}
		else {
			msg = (char*)*(DWORD*)(first_entry + i * 24);
		}
		sprintf_s(buf, "0x%lx | length: %d | %s", (first_entry + i * 24), msg_len, msg);
		dbg_sock.do_send(buf);
	}

	/* Call original function */
	original_chat_hist(hdc_obj);
}

/* dll class */
namespace norm_dll {

#define CHECK(buf, errno_) (errno_ == 0) ? sprintf_s(buf, "Success at Line %s:%d", __FILE__ ,__LINE__) : \
	sprintf_s(buf, "Error %d at %s:%d", errno_, __FILE__ ,__LINE__)

norm::norm()
{
}

norm::~norm()
{
}

void norm::start()
{
	char info_buf[256];
	/* Connect to the debug socket */
	dbg_sock = debug_socket();
	int err = dbg_sock.do_connect();
	if (err != 0) {
		MessageBox(0, (LPCWSTR)"Unable to connect to the debug socket!", (LPCWSTR)"norm.dll error!", MB_OK);
	}
	dbg_sock.do_send("Hello Debugger!");

	/* Hook functions. */
	err = DetourTransactionBegin();
	CHECK(info_buf, err);
	dbg_sock.do_send(info_buf);

	err = DetourUpdateThread(GetCurrentThread());
	CHECK(info_buf, err);
	dbg_sock.do_send(info_buf);

	// this will hook the function
	err = DetourAttach(&(LPVOID&)chat_hist_func, &chat_hist_hook);
	CHECK(info_buf, err);
	dbg_sock.do_send(info_buf);

	err = DetourTransactionCommit();
	CHECK(info_buf, err);
	dbg_sock.do_send(info_buf);
}
}
