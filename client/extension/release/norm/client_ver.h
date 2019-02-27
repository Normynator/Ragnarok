#pragma once
#include "stdafx.h"
#include <stdio.h>

#define CLIENT_VER 20180620//20150000

#define DEBUG

#define CHECK(buf, errno_) (errno_ == 0) ? sprintf_s(buf, "Success at Line %s:%d", __FILE__ ,__LINE__) : \
	sprintf_s(buf, "Error %d at %s:%d", errno_, __FILE__ ,__LINE__)
