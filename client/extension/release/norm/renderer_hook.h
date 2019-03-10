#pragma once
#include "stdafx.h"
#include "client_ver.h"
#include "debug_socket.h"
#include "state.h"
#include <Windows.h>

/* contains all hooks related to drawing and rendering */

/* hooks all available functions. */
void renderer_detour(std::shared_ptr<norm_dll::debug_socket> dbg_sock, std::shared_ptr<norm_dll::state> c_state);
