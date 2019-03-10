#pragma once
#include <memory>
#include "debug_socket.h"
#include "state.h"
namespace norm_dll {

	class norm {
	public:
		norm();
		virtual ~norm();

		void start();

	private:
		std::shared_ptr<debug_socket> dbg_sock;
		std::shared_ptr<state> c_state;
	};
}