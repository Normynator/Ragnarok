#pragma once
#include "stdafx.h"
#include "client_ver.h"
#include "debug_socket.h"
#include "state.h"
#include <Windows.h>

/* contains all hooks related to drawing and rendering */

/* hooks all available functions. */
int renderer_detour(std::shared_ptr<norm_dll::state> c_state);
