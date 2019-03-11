#pragma once
#include "mod.h"
class overlay :
	public norm_dll::mod
{
public:
	overlay(std::shared_ptr<norm_dll::debug_socket> dbg_sock);
	virtual ~overlay();
	void draw_scene(void*);
	void register_hooks();
};

