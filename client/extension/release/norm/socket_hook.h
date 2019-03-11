#pragma once
#include "stdafx.h"
#include "client_ver.h"
#include "debug_socket.h"
#include "state.h"
#include <Windows.h>

int socket_detour(std::shared_ptr<norm_dll::state> c_state);

