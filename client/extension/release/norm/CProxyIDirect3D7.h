#pragma once
#include "hook_dx.h"
#include "CProxyIDirect3DDevice7.h"

#define CLASSNAME "IDirect3D7"
class CProxyIDirect3D7 : public IDirect3D7 {
private:
	IDirect3D7*			m_Instance;
	std::shared_ptr<norm_dll::norm> c_state;
	CProxyIDirect3DDevice7* pd3dd;
public:
	CProxyIDirect3D7(IDirect3D7* ptr, std::shared_ptr<norm_dll::norm> c_state) : c_state(c_state), m_Instance(ptr) {}
	CProxyIDirect3DDevice7* getPD3DD() { return pd3dd; };

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID p1, LPVOID * p2) PROXY2(QueryInterface)
	STDMETHOD_(ULONG, AddRef) (THIS)		PROXY0(AddRef)
	STDMETHOD_(ULONG, Release) (THIS)	PROXY_RELEASE

	/*** IDirect3D7 methods ***/
	STDMETHOD(EnumDevices)(THIS_ LPD3DENUMDEVICESCALLBACK7 p1, LPVOID p2) PROXY2(EnumDevices)
	//STDMETHOD(CreateDevice)(THIS_ REFCLSID p1,LPDIRECTDRAWSURFACE7 p2,LPDIRECT3DDEVICE7* p3) PROXY3(CreateDevice)
	STDMETHOD(CreateDevice)(THIS_ REFCLSID p1, LPDIRECTDRAWSURFACE7 p2, LPDIRECT3DDEVICE7* p3)
	{
		return Proxy_CreateDevice(p1, p2, p3);
	}
	STDMETHOD(CreateVertexBuffer)(THIS_ LPD3DVERTEXBUFFERDESC p1, LPDIRECT3DVERTEXBUFFER7* p2, DWORD p3) PROXY3(CreateVertexBuffer)
	STDMETHOD(EnumZBufferFormats)(THIS_ REFCLSID p1, LPD3DENUMPIXELFORMATSCALLBACK p2, LPVOID p3) PROXY3(EnumZBufferFormats)
	STDMETHOD(EvictManagedTextures)(THIS) PROXY0(EvictManagedTextures)


	HRESULT Proxy_CreateDevice(REFCLSID rclsid, LPDIRECTDRAWSURFACE7 lpDDS, LPDIRECT3DDEVICE7 *  lplpD3DDevice);
};
#undef CLASSNAME