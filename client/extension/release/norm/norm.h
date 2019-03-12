#pragma once
#include <memory>
#include <vector>
#include "debug_socket.h"
#include "mod.h"
namespace norm_dll {

	class norm : public std::enable_shared_from_this<norm> {
	public:
		/* Constatns from the client*/
		DWORD g_session;
		DWORD g_session_m_averagePingTime;
		DWORD g_renderer;
		DWORD g_window_mgr = 0;

		/* Screen height and width*/
		ULONG m_width = -1;
		ULONG m_height = -1;

		/* Settings for renderer.
		 * Draw if 1 else not.
		 */
		//int display_ping = 0;
		//int display_grid = 0;

		norm();
		virtual ~norm();

		void start();

		std::shared_ptr<debug_socket>dbg_sock;

		//void notify_stat_server();
		//bool already_notified = false;

	private:
		//std::shared_ptr<state> c_state;
		void install_mods();
		std::vector<std::shared_ptr<mod>> mods;
	};
}