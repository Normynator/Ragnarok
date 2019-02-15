#ifndef client_ver_h
#define client_ver_h
#include "stdafx.h"
#include <stdio.h>

#define CLIENT_VER 20150000

#define CHECK(buf, errno_) (errno_ == 0) ? sprintf_s(buf, "Success at Line %s:%d", __FILE__ ,__LINE__) : \
	sprintf_s(buf, "Error %d at %s:%d", errno_, __FILE__ ,__LINE__)

#endif /* client_ver_h */
