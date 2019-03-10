#include "stdafx.h"
#include "socket_hook.h"
#include "detours.h"

/* debug */
static std::shared_ptr<norm_dll::debug_socket> dbg_sock;
static std::shared_ptr<norm_dll::state> c_state;

#if CLIENT_VER == 20180620
#ifdef COMENC
int (WINAPI *pSend)(SOCKET s, const char* buf, int len, int flags) = send;
int (WINAPI *pRecv)(SOCKET s, char* buf, int len, int flags) = recv;

int WINAPI send_hook(SOCKET s, const char* buf, int len, int flags)
{
	if (s != *dbg_sock->get_sock()) {
		char* tmp_buf = (char*)calloc(len, sizeof(char));
		for (int i = 0; i < len; i++)
			tmp_buf[i] = buf[i] ^ 1234;
		return pSend(s, tmp_buf, len, flags);
	}
	return pSend(s, buf, len, flags);
}

int WINAPI recv_hook(SOCKET s, char* buf, int len, int flags) {
	int ret_len = pRecv(s, buf, len, flags);
	for (int i = 0; i < ret_len; i++)
		buf[i] ^= 1234;
	return ret_len;
}
#endif /* COMENC */
#endif /* 20180620 */

void socket_detour(std::shared_ptr<norm_dll::debug_socket> dbg_sock_, std::shared_ptr<norm_dll::state> state_) {
	int err = 0;
	int hook_count = 0;
	char info_buf[256];
	dbg_sock = dbg_sock_;
	c_state = state_;

#ifdef COMENC
	err = DetourAttach(&(LPVOID&)pSend, &send_hook);
	CHECK(info_buf, err);
	dbg_sock->do_send(info_buf);
	err = DetourAttach(&(LPVOID&)pRecv, &recv_hook);
	CHECK(info_buf, err);
	dbg_sock->do_send(info_buf);
	hook_count++;
#endif

	sprintf_s(info_buf, "Socket mods loaded: %d", hook_count);
	dbg_sock->do_send(info_buf);
}
