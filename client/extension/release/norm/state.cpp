#include "stdafx.h"
#include "state.h"
#include "client_ver.h"
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

namespace norm_dll {
state::state(std::shared_ptr<debug_socket> dbg_sock_)
{
	dbg_sock = dbg_sock_;

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

	//register
}

state::~state()
{
}

void state::notify_stat_server()
{
	if (already_notified)
		return;
	BOOL  bResults = FALSE;
	HINTERNET hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"norm.dll statics/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, L"127.0.0.1",
			5000, 0);

	// Create an HTTP Request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET",
			L"/beta-client",
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			0);

	// Send a Request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0, WINHTTP_NO_REQUEST_DATA, 0,
			0, 0);

	// PLACE ADDITIONAL CODE HERE.

	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n", GetLastError());

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	already_notified = true;
}
}
