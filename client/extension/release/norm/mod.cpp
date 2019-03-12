#include "stdafx.h"
#include "norm.h"
#include "mod.h"

void norm_dll::mod::no_overwrite()
{
		c_state->dbg_sock->do_send("Missing function overwrite.");
}
