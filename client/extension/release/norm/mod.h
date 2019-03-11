#pragma once
#include "debug_socket.h"
namespace norm_dll {
class mod : public std::enable_shared_from_this<mod>
{
public:
	mod(std::shared_ptr<norm_dll::debug_socket> dbg_sock) : dbg_sock(dbg_sock) {}
	virtual ~mod() {}
	virtual void register_hooks(void) = 0;
	virtual void draw_scene(void*) {}

protected:
	std::shared_ptr<norm_dll::debug_socket> dbg_sock;

private:
	void no_overwrite() {
		dbg_sock->do_send("Missing function overwrite,");
	}
};
}

