#include "stdafx.h"
#include "detours.h"
#include "norm.h"

#include "hook_chat.h"
#include "hook_renderer.h"
#include "hook_socket.h"

#include "mod_overlay.h"
#include "mod_statistics.h"

#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

// Installs a mod based on the classname.
#define INSTALL_MOD(modname)	auto m_##modname = std::make_shared<modname>(dbg_sock); \
								m_##modname->register_hooks(); \
								mods.push_back(m_##modname);

/* dll class */
namespace norm_dll {

norm::norm()
{
	/* settings */
#if CLIENT_VER == 20180620
	g_session = 0x010130C8;
	g_session_m_averagePingTime = g_session + 0x630;
	g_renderer = 0x00DA2068;
#endif
#if CLIENT_VER == 20150000
	g_session = 0x00E0EE28;
	g_session_m_averagePingTime = g_session + 0x634;
#endif
}

void norm::install_mods()
{
	// Disable a mod by commenting out the specific line.
	INSTALL_MOD(overlay);
	INSTALL_MOD(statistics);
}

void norm::start()
{
	char info_buf[256];
	/* Connect to the debug socket */
	dbg_sock = std::make_shared<debug_socket>();
	//c_state = std::make_shared<state>(dbg_sock);
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
	this->install_mods();

	int total_mods = 0;
	auto sptr = shared_from_this();
	total_mods += chat_detour(sptr);
	total_mods += socket_detour(sptr);
	total_mods += renderer_detour(sptr);

	err = DetourTransactionCommit();
	CHECK(info_buf, err);
	dbg_sock->do_send(info_buf);

	sprintf_s(info_buf, "Total loaded mods: %d", total_mods);
	dbg_sock->do_send(info_buf);
}


norm::~norm()
{
}
}