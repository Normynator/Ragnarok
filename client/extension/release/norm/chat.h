#pragma once
#include "stdafx.h"
#include "client_ver.h"
#include "debug_socket.h"
#include <Windows.h>

void chat_detour(std::shared_ptr<norm_dll::debug_socket> dbg_sock);
