#pragma once
namespace norm_dll {
class state
{
public:
	DWORD g_session;
	DWORD g_session_m_averagePingTime;
	DWORD g_renderer;
	ULONG m_width = -1;
	ULONG m_height = -1;
	int display_ping = 0;
	int display_grid = 0;

	state();
	~state();
};
}

