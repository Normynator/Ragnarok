#pragma once
#include "hook_dx.h"

#define CLASSNAME "IDirect3DDevice7"
class CProxyIDirect3DDevice7 : public IDirect3DDevice7 {
private:
	IDirect3DDevice7*			m_Instance;
	IDirectDrawSurface7*	TargetSurface;
	std::shared_ptr<norm_dll::norm> c_state;
	//BOOL					m_firstonce;
	//BOOL					m_frameonce;


public:

	CProxyIDirect3DDevice7(IDirect3DDevice7* ptr, IDirectDrawSurface7* psf, std::shared_ptr<norm_dll::norm> c_state) : m_Instance(ptr), TargetSurface(psf), c_state(c_state) {}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID p1, LPVOID * p2) PROXY2(QueryInterface)
	STDMETHOD_(ULONG, AddRef) (THIS)		PROXY0(AddRef)
	STDMETHOD_(ULONG, Release) (THIS)//	PROXY_RELEASE
	{
		Proxy_Release();
		ULONG Count = m_Instance->Release();
		if (Count == 0)
			delete this;
		return Count;
	}
	void Proxy_Release(void);

	/*** IDirect3DDevice7 methods ***/
	STDMETHOD(GetCaps)(THIS_ LPD3DDEVICEDESC7 p1) PROXY1(GetCaps)
	STDMETHOD(EnumTextureFormats)(THIS_ LPD3DENUMPIXELFORMATSCALLBACK p1, LPVOID p2) PROXY2(EnumTextureFormats)

	//STDMETHOD(BeginScene)(THIS) PROXY0(BeginScene)
	STDMETHOD(BeginScene)(THIS) { return Proxy_BeginScene(); };

	//STDMETHOD(EndScene)(THIS) PROXY0(EndScene)
	STDMETHOD(EndScene)(THIS) { return Proxy_EndScene(); };

	STDMETHOD(GetDirect3D)(THIS_ LPDIRECT3D7* p1) PROXY1(GetDirect3D)
	STDMETHOD(SetRenderTarget)(THIS_ LPDIRECTDRAWSURFACE7 p1, DWORD p2) PROXY2(SetRenderTarget)
	STDMETHOD(GetRenderTarget)(THIS_ LPDIRECTDRAWSURFACE7 *p1) PROXY1(GetRenderTarget)
	STDMETHOD(Clear)(THIS_ DWORD p1, LPD3DRECT p2, DWORD p3, D3DCOLOR p4, D3DVALUE p5, DWORD p6) PROXY6(Clear)
	STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE p1, LPD3DMATRIX p2) PROXY2(SetTransform)
	STDMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE p1, LPD3DMATRIX p2) PROXY2(GetTransform)
	STDMETHOD(SetViewport)(THIS_ LPD3DVIEWPORT7 p1) PROXY1(SetViewport)

	STDMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE p1, LPD3DMATRIX p2) PROXY2(MultiplyTransform)
	STDMETHOD(GetViewport)(THIS_ LPD3DVIEWPORT7 p1) PROXY1(GetViewport)
	STDMETHOD(SetMaterial)(THIS_ LPD3DMATERIAL7 p1) PROXY1(SetMaterial)
	STDMETHOD(GetMaterial)(THIS_ LPD3DMATERIAL7 p1) PROXY1(GetMaterial)
	STDMETHOD(SetLight)(THIS_ DWORD p1, LPD3DLIGHT7 p2) PROXY2(SetLight)
	STDMETHOD(GetLight)(THIS_ DWORD p1, LPD3DLIGHT7 p2) PROXY2(GetLight)

	STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE p1,DWORD p2) PROXY2(SetRenderState)
	//STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE p1, DWORD p2) { return Proxy_SetRenderState(p1, p2); }

	STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE p1, LPDWORD p2) PROXY2(GetRenderState)
	STDMETHOD(BeginStateBlock)(THIS) PROXY0(BeginStateBlock)
	STDMETHOD(EndStateBlock)(THIS_ LPDWORD p1) PROXY1(EndStateBlock)
	STDMETHOD(PreLoad)(THIS_ LPDIRECTDRAWSURFACE7 p1) PROXY1(PreLoad)
	STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE p1, DWORD p2, LPVOID p3, DWORD p4, DWORD p5) PROXY5(DrawPrimitive)
		//
	STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE p1, DWORD p2, LPVOID p3, DWORD p4, LPWORD p5, DWORD p6, DWORD p7) PROXY7(DrawIndexedPrimitive)
	STDMETHOD(SetClipStatus)(THIS_ LPD3DCLIPSTATUS p1) PROXY1(SetClipStatus)
	STDMETHOD(GetClipStatus)(THIS_ LPD3DCLIPSTATUS p1) PROXY1(GetClipStatus)
	STDMETHOD(DrawPrimitiveStrided)(THIS_ D3DPRIMITIVETYPE p1, DWORD p2, LPD3DDRAWPRIMITIVESTRIDEDDATA p3, DWORD p4, DWORD p5) PROXY5(DrawPrimitiveStrided)
	STDMETHOD(DrawIndexedPrimitiveStrided)(THIS_ D3DPRIMITIVETYPE p1, DWORD p2, LPD3DDRAWPRIMITIVESTRIDEDDATA p3, DWORD p4, LPWORD p5, DWORD p6, DWORD p7) PROXY7(DrawIndexedPrimitiveStrided)
	STDMETHOD(DrawPrimitiveVB)(THIS_ D3DPRIMITIVETYPE p1, LPDIRECT3DVERTEXBUFFER7 p2, DWORD p3, DWORD p4, DWORD p5) PROXY5(DrawPrimitiveVB)
	STDMETHOD(DrawIndexedPrimitiveVB)(THIS_ D3DPRIMITIVETYPE p1, LPDIRECT3DVERTEXBUFFER7 p2, DWORD p3, DWORD p4, LPWORD p5, DWORD p6, DWORD p7) PROXY7(DrawIndexedPrimitiveVB)
	STDMETHOD(ComputeSphereVisibility)(THIS_ LPD3DVECTOR p1, LPD3DVALUE p2, DWORD p3, DWORD p4, LPDWORD p5) PROXY5(ComputeSphereVisibility)
	STDMETHOD(GetTexture)(THIS_ DWORD p1, LPDIRECTDRAWSURFACE7 *p2) PROXY2(GetTexture)
	STDMETHOD(SetTexture)(THIS_ DWORD p1, LPDIRECTDRAWSURFACE7 p2) PROXY2(SetTexture)
	STDMETHOD(GetTextureStageState)(THIS_ DWORD p1, D3DTEXTURESTAGESTATETYPE p2, LPDWORD p3) PROXY3(GetTextureStageState)
	STDMETHOD(SetTextureStageState)(THIS_ DWORD p1, D3DTEXTURESTAGESTATETYPE p2, DWORD p3) PROXY3(SetTextureStageState)
	STDMETHOD(ValidateDevice)(THIS_ LPDWORD p1) PROXY1(ValidateDevice)
	STDMETHOD(ApplyStateBlock)(THIS_ DWORD p1) PROXY1(ApplyStateBlock)
	STDMETHOD(CaptureStateBlock)(THIS_ DWORD p1) PROXY1(CaptureStateBlock)
	STDMETHOD(DeleteStateBlock)(THIS_ DWORD p1) PROXY1(DeleteStateBlock)
	STDMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE p1, LPDWORD p2) PROXY2(CreateStateBlock)
	STDMETHOD(Load)(THIS_ LPDIRECTDRAWSURFACE7 p1, LPPOINT p2, LPDIRECTDRAWSURFACE7 p3, LPRECT p4, DWORD p5) PROXY5(Load)
	STDMETHOD(LightEnable)(THIS_ DWORD p1, BOOL p2) PROXY2(LightEnable)
	STDMETHOD(GetLightEnable)(THIS_ DWORD p1, BOOL* p2) PROXY2(GetLightEnable)
	STDMETHOD(SetClipPlane)(THIS_ DWORD p1, D3DVALUE* p2) PROXY2(SetClipPlane)
	STDMETHOD(GetClipPlane)(THIS_ DWORD p1, D3DVALUE* p2) PROXY2(GetClipPlane)
	STDMETHOD(GetInfo)(THIS_ DWORD p1, LPVOID p2, DWORD p3) PROXY3(GetInfo)


	//
	// Proxy Functions
	//HRESULT Proxy_SetRenderState(THIS_ D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState);
	HRESULT Proxy_BeginScene(void);
	HRESULT Proxy_EndScene(void);

	void add_beginSecene_callback(std::shared_ptr<norm_dll::mod> cb);
	void add_endSecene_callback(std::shared_ptr<norm_dll::mod> cb);
};
#undef CLASSNAME