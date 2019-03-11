#pragma once
#include "stdafx.h"
#include "client_ver.h"
#include "debug_socket.h"
#include "norm.h"
#include <Windows.h>

/* chat: contains all hooks related to the ingame chat system */

/* hooks all available functions. */
int chat_detour(std::shared_ptr<norm_dll::norm> c_state);

/* used for custom /commands. 
 * will be revoked when GetTalkType hook is implemented.
 */
enum e_command {
	NON = -1,
	TIMESTAMP = 0,
	PINGO = 1,
	GRID = 2,
	DIT = 3
};
