#ifndef norm_h
#define norm_h
#include "debug_socket.h"

namespace norm_dll {

class norm {
public:
	norm();
	virtual ~norm();

	void start();

private:
	std::shared_ptr<debug_socket> dbg_sock;
};
}
#endif /* norm_h */
