#include "stdafx.h"
#include "CProxyIDirect3D7.h"
#include "CProxyIDirect3DDevice7.h"

HRESULT CProxyIDirect3D7::Proxy_CreateDevice(REFCLSID rclsid, LPDIRECTDRAWSURFACE7 lpDDS, LPDIRECT3DDEVICE7 *  lplpD3DDevice)
{
	this->c_state->dbg_sock->do_send("CProxyIDirect3D7::Proxy_CreateDevice");
	HRESULT temp_ret = m_Instance->CreateDevice(rclsid, lpDDS, lplpD3DDevice);

	if (temp_ret == D3D_OK) {
		void *ret_cProxy;
		ret_cProxy = new CProxyIDirect3DDevice7((IDirect3DDevice7*)*lplpD3DDevice, lpDDS, c_state);
		*lplpD3DDevice = (LPDIRECT3DDEVICE7)ret_cProxy;
		return D3D_OK;
	}
	return temp_ret;
}