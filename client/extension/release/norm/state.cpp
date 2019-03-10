#include "stdafx.h"
#include "state.h"
#include "client_ver.h"

namespace norm_dll {
state::state()
{
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


state::~state()
{
}
}
