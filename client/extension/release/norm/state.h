#pragma once
#include "debug_socket.h"

/* This class contains all settings which are required for the DLL
 * to function correctly.
 * Client constants as g_session and g_renderer are set in the constructor.
 */
namespace norm_dll {
class state
{
public:
	/* Constatns from the client*/
	DWORD g_session;
	DWORD g_session_m_averagePingTime;
	DWORD g_renderer;

	/* Screen height and width*/
	ULONG m_width = -1; 
	ULONG m_height = -1;

	/* Settings for renderer.
	 * Draw if 1 else not.
	 */
	int display_ping = 0;
	int display_grid = 0;

	state(std::shared_ptr<debug_socket>dbg_sock);
	~state();

	std::shared_ptr<debug_socket>dbg_sock;

	void notify_stat_server();
	bool already_notified = false;
};
}

