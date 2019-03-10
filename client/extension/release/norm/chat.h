#pragma once
#include "stdafx.h"
#include "client_ver.h"
#include "debug_socket.h"
#include "state.h"
#include <Windows.h>

void chat_detour(std::shared_ptr<norm_dll::debug_socket> dbg_sock, std::shared_ptr<norm_dll::state> c_state);

enum e_command {
	NON = -1,
	TIMESTAMP = 0,
	PINGO = 1,
	GRID = 2,
	DIT = 3
};
