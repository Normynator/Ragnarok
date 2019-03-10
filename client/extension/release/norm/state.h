#pragma once
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

	state();
	~state();
};
}

