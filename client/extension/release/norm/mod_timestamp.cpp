#include "stdafx.h"
#include "mod_timestamp.h"
#include <time.h>

#include "hook_chat.h"
#include "hook_session.h"

timestamp::timestamp(norm_dll::norm* c_state) : mod(c_state)
{
}

timestamp::~timestamp()
{
}

void timestamp::send_msg(void** this_obj, int* a1, void** a2, void** a3, int* a4, int* a5)
{
	if (*a1 != 1)
		return;

	if (this->enabled) {
		time_t t = time(NULL);
		struct tm buf_tm;
		char ts[10] = "";
		int err = localtime_s(&buf_tm, &t);
		strftime(ts, 10, "%H:%M:%S", &buf_tm);
		sprintf_s(this->msg_buf, "[%s] %s", ts, (char*)*a2);
#if CLIENT_VER == 20150000
		*a2 = (int)&this->msg_buf;
#elif CLIENT_VER == 20180620
		*a2 = this->msg_buf;
#endif	
	}
}

int timestamp::get_talk_type(void **this_obj, void **src, int *a1, int *a2, int* retval)
{
	// debug: c_state->dbg_sock->do_send((char*)*src);
	if (strcmp((char*)*src, "/timestamp") == 0) {
		char buf[64];
		this->enabled ^= 1;
		if (this->enabled)
			sprintf_s(buf, "Timestamp is now enabled.");
		else
			sprintf_s(buf, "Timestamp is now disabled.");
		this->print_to_chat(buf);
		*retval = -1;
		return 1;
	}
	return 0;
}

/*void timestamp::register_hooks()
{
	int res = register_SendMsg_hook(shared_from_this());
	res += register_GetTalkType_hook(shared_from_this());
	this->validate(res, 2, __FILE__);
}*/
