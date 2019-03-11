#include "stdafx.h"
#include "mod_overlay.h"
#include "hook_renderer.h"

overlay::overlay(std::shared_ptr<norm_dll::debug_socket> dbg_sock_) : mod(dbg_sock_)
{
}

overlay::~overlay()
{
}

void overlay::draw_scene(void* this_obj)
{
	//enable this to debug: dbg_sock->do_send("draw_scene of overlay was called!");
}

void overlay::register_hooks() {
	register_DrawScene_hook(shared_from_this());
}
