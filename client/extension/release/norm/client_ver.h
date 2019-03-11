#pragma once
#include "stdafx.h"
#include <stdio.h>

#define CLIENT_VER 20180620 /* needs to fit the client date of the exe. */
#define DEBUG	/* requires a debug-server listening on localhost:1337 */

/* simple macro for some debug output (used with the debug server) */
#define CHECK(buf, errno_) (errno_ == 0) ? sprintf_s(buf, "Success at Line %s:%d", __FILE__ ,__LINE__) : \
	sprintf_s(buf, "Error %d at %s:%d", errno_, __FILE__ ,__LINE__)

/* disable mods by undefining */
#define COMENC	/* requires src edit to enable "encryption" on server side. */
#define CHAT_INPUT
#define UIWindowMgr__SendMsg
#define TEXTOUTSCREEN
