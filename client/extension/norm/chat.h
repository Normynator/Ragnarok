#ifndef chat_h
#define chat_h
#include "stdafx.h"
#include "client_ver.h"
#include "debug_socket.h"
#include <Windows.h>

/* Memory addresses */
// Chat history function
// chat_hist_func = 0x0;

// TalkTypeTableVector
// ttt_vector = 0x0;
// ttt_vector_end = 0x0;

// Function which has pointer to the /command text.
// chat_input_func

void chat_detour(std::shared_ptr<norm_dll::debug_socket> dbg_sock);

#endif /* chat_h */