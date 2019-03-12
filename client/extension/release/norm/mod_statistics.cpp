#include "stdafx.h"
#include "mod_statistics.h"
#include "hook_renderer.h"
#include <winhttp.h>

statistics::statistics(norm_dll::norm* c_state) : mod(c_state)
{
}

statistics::~statistics()
{
}

void statistics::draw_scene(void* this_obj)
{
	//enable this to debug: dbg_sock->do_send("draw_scene of statistics was called!");
	//this->notify_stat_server();
}

void statistics::register_hooks() {
	register_DrawScene_hook(shared_from_this());
}

void statistics::notify_stat_server()
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
