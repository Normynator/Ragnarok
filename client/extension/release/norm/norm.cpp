#include "stdafx.h"
#include "detours.h"
#include "norm.h"
#include "chat.h"
#include "renderer_hook.h"
#include "socket_hook.h"
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

/* dll class */
namespace norm_dll {

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
	dbg_sock = std::make_shared<debug_socket>();
	c_state = std::make_shared<state>(dbg_sock);
	int err = dbg_sock->do_connect();
	if (err != 0) {
		MessageBox(0, (LPCWSTR)"Unable to connect to the debug socket!", (LPCWSTR)"norm.dll error!", MB_OK);
	}
	dbg_sock->do_send("Hello Debugger!");

	/* Hook functions. */
	err = DetourTransactionBegin();
	CHECK(info_buf, err);
	dbg_sock->do_send(info_buf);

	err = DetourUpdateThread(GetCurrentThread());
	CHECK(info_buf, err);
	dbg_sock->do_send(info_buf);

	// attach hooks and share ownership of dbg_sock.
	int total_mods = 0;
	total_mods += chat_detour(c_state);
	total_mods += socket_detour(c_state);
	total_mods += renderer_detour(c_state);

	err = DetourTransactionCommit();
	CHECK(info_buf, err);
	dbg_sock->do_send(info_buf);

	sprintf_s(info_buf, "Total loaded mods: %d", total_mods);
	dbg_sock->do_send(info_buf);
}
}