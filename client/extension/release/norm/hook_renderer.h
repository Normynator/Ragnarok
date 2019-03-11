#pragma once
#include "stdafx.h"
#include "client_ver.h"
#include "debug_socket.h"
#include "norm.h"
#include <Windows.h>
#include "mod.h"

/* contains all hooks related to drawing and rendering */

int register_DrawScene_hook(std::shared_ptr<norm_dll::mod> mod_ptr);

/* hooks all available functions. */
int renderer_detour(std::shared_ptr<norm_dll::norm> c_state);
