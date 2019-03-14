#include "stdafx.h"
#include "CProxyIDirectDraw7.h"
#include "CProxyIDirect3D7.h"

CProxyIDirectDraw7* CProxyIDirectDraw7::lpthis;

HRESULT CProxyIDirectDraw7::Proxy_QueryInterface(THIS_ REFIID riid, LPVOID FAR * ppvObj)
{
	this->c_state->dbg_sock->do_send("Proxy_QueryInterface");
	if (IsEqualGUID(riid, IID_IDirect3D7)) {
		HRESULT temp_ret = m_Instance->QueryInterface(riid, ppvObj);
		if (temp_ret == S_OK) {
			void *ret_cProxy;
			//LPVOID FAR * ppvObj_proxy;
			ret_cProxy = new CProxyIDirect3D7((IDirect3D7*)*ppvObj, c_state);
			*ppvObj = ret_cProxy;

			return S_OK;
		}
		else {
			return temp_ret;
		}
	}

	return m_Instance->QueryInterface(riid, ppvObj);
}

CProxyIDirectDraw7::~CProxyIDirectDraw7()
{
}