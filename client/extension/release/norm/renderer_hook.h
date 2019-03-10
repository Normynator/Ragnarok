#pragma once
#include "stdafx.h"
#include "client_ver.h"
#include "debug_socket.h"
#include "state.h"
#include <Windows.h>

void renderer_detour(std::shared_ptr<norm_dll::debug_socket> dbg_sock, std::shared_ptr<norm_dll::state> c_state);
