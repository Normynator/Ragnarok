#pragma once
#include "debug_socket.h"
namespace norm_dll {
class norm;
class mod : public std::enable_shared_from_this<mod>
{
public:
	mod(norm* c_state) : c_state(c_state) {}
	virtual ~mod() {}
	virtual void register_hooks(void) = 0;
	virtual void draw_scene(void*) {}

protected:
	norm* c_state;

private:
	void no_overwrite();
};
}

