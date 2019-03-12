#pragma once
#include "mod.h"
class overlay :
	public norm_dll::mod
{
public:
	overlay(norm_dll::norm* c_state);
	virtual ~overlay();
	void draw_scene(void*);
	void register_hooks();

private:
	bool display_ping = false;
	bool display_grid = false;
};

