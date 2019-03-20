#pragma once
#include "debug_socket.h"

namespace norm_dll {
class norm;
class mod : public std::enable_shared_from_this<mod>
{
public:
	mod(norm* c_state) : c_state(c_state) {}
	virtual ~mod() {}
	//virtual void register_hooks(void) = 0;
	virtual void draw_scene(void*) { /*no_override();*/ }
	virtual void send_msg(void**, int*, void**, void**, int*, int*) { /*no_override();*/ }
	virtual int get_talk_type(void**, void**, int*, int*, int*) { /*no_override();*/ return 0; }

protected:
	norm* c_state;
	void validate(int, int, const char*);
	void print_to_chat(char* msg);

private:
	//void no_override();
	int active = -1;
};
}

