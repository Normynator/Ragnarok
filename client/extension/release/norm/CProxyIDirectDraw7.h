#pragma once
#include "hook_dx.h"
#include "CProxyIDirect3D7.h"

#define CLASSNAME "IDirectDraw7"
class CProxyIDirectDraw7 : public IDirectDraw7 {
private:
	IDirectDraw7*			m_Instance;
	DWORD					CooperativeLevel;
	DWORD					PrimarySurfaceFlag;
	IDirectDrawSurface7*	TargetSurface;
	std::shared_ptr<norm_dll::norm> c_state;
	CProxyIDirect3D7* pd3d;
public:
	static CProxyIDirectDraw7*    lpthis;

	CProxyIDirectDraw7(IDirectDraw7* ptr, std::shared_ptr<norm_dll::norm> c_state) : c_state(c_state), m_Instance(ptr), CooperativeLevel(0), PrimarySurfaceFlag(0), TargetSurface(NULL) {}
	~CProxyIDirectDraw7();

	static CProxyIDirectDraw7* getLPProxyIDirectDraw7(void) { return lpthis; };
	void   setThis(CProxyIDirectDraw7* _lpthis) { lpthis = _lpthis; };
	CProxyIDirect3D7* getPD3D() { return pd3d; };

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * ppvObj)
		{
			return Proxy_QueryInterface(riid, ppvObj);
		}
		STDMETHOD_(ULONG, AddRef) (THIS)		PROXY0(AddRef)
		STDMETHOD_(ULONG, Release) (THIS)	PROXY0(Release)
		/*{
			return Proxy_Release();
		}*/

		/*** IDirectDraw methods ***/
		STDMETHOD(Compact)(THIS)			PROXY0(Compact)
		STDMETHOD(CreateClipper)(THIS_ DWORD p1, LPDIRECTDRAWCLIPPER FAR* p2, IUnknown FAR * p3)					PROXY3(CreateClipper)
		STDMETHOD(CreatePalette)(THIS_ DWORD p1, LPPALETTEENTRY p2, LPDIRECTDRAWPALETTE FAR* p3, IUnknown FAR * p4)	PROXY4(CreatePalette)
		STDMETHOD(CreateSurface)(THIS_  LPDDSURFACEDESC2 p1, LPDIRECTDRAWSURFACE7 FAR * p2, IUnknown FAR * p3)	    PROXY3(CreateSurface)
		/*{
			return Proxy_CreateSurface(p1, p2, p3);
		}*/
		STDMETHOD(DuplicateSurface)(THIS_ LPDIRECTDRAWSURFACE7 p1, LPDIRECTDRAWSURFACE7 FAR * p2)					PROXY2(DuplicateSurface)
		STDMETHOD(EnumDisplayModes)(THIS_ DWORD p1, LPDDSURFACEDESC2 p2, LPVOID p3, LPDDENUMMODESCALLBACK2 p4)		PROXY4(EnumDisplayModes)
		STDMETHOD(EnumSurfaces)(THIS_ DWORD p1, LPDDSURFACEDESC2 p2, LPVOID p3, LPDDENUMSURFACESCALLBACK7 p4) 		PROXY4(EnumSurfaces)
		STDMETHOD(FlipToGDISurface)(THIS)								PROXY0(FlipToGDISurface)
		STDMETHOD(GetCaps)(THIS_ LPDDCAPS p1, LPDDCAPS p2)				PROXY2(GetCaps)
		STDMETHOD(GetDisplayMode)(THIS_ LPDDSURFACEDESC2 p1)			PROXY1(GetDisplayMode)
		/*{
			return Proxy_GetDisplayMode(p1);
		}*/
		STDMETHOD(GetFourCCCodes)(THIS_  LPDWORD p1, LPDWORD p2)		PROXY2(GetFourCCCodes)
		STDMETHOD(GetGDISurface)(THIS_ LPDIRECTDRAWSURFACE7 FAR * p1)	PROXY1(GetGDISurface)
		STDMETHOD(GetMonitorFrequency)(THIS_ LPDWORD p1)				PROXY1(GetMonitorFrequency)
		STDMETHOD(GetScanLine)(THIS_ LPDWORD p1)						PROXY1(GetScanLine)
		STDMETHOD(GetVerticalBlankStatus)(THIS_ LPBOOL p1)				PROXY1(GetVerticalBlankStatus)
		STDMETHOD(Initialize)(THIS_ GUID FAR * p1)						PROXY1(Initialize)
		STDMETHOD(RestoreDisplayMode)(THIS)								PROXY0(RestoreDisplayMode)
		STDMETHOD(SetCooperativeLevel)(THIS_ HWND p1, DWORD p2)			PROXY2(SetCooperativeLevel)
		/*{
			return Proxy_SetCooperativeLevel(p1, p2);
		}*/
		STDMETHOD(SetDisplayMode)(THIS_ DWORD p1, DWORD p2, DWORD p3, DWORD p4, DWORD p5)	PROXY5(SetDisplayMode)
		/*{
			return Proxy_SetDisplayMode(p1, p2, p3, p4, p5);
		}*/
		STDMETHOD(WaitForVerticalBlank)(THIS_ DWORD p1, HANDLE p2)							PROXY2(WaitForVerticalBlank)
		/*{
			return Proxy_WaitForVerticalBlank(p1, p2);
		}*/

		/*** Added in the v2 interface ***/
		STDMETHOD(GetAvailableVidMem)(THIS_ LPDDSCAPS2 p1, LPDWORD p2, LPDWORD p3)	PROXY3(GetAvailableVidMem)
		/*** Added in the V4 Interface ***/
		STDMETHOD(GetSurfaceFromDC) (THIS_ HDC p1, LPDIRECTDRAWSURFACE7 * p2)		PROXY2(GetSurfaceFromDC)
		STDMETHOD(RestoreAllSurfaces)(THIS)											PROXY0(RestoreAllSurfaces)
		/*{
			return Proxy_RestoreAllSurfaces();
		}*/
		STDMETHOD(TestCooperativeLevel)(THIS)										PROXY0(TestCooperativeLevel)
		STDMETHOD(GetDeviceIdentifier)(THIS_ LPDDDEVICEIDENTIFIER2 p1, DWORD p2) 	PROXY2(GetDeviceIdentifier)
		/*** Added in the V7 Interface ***/
		STDMETHOD(StartModeTest)(THIS_ LPSIZE p1, DWORD p2, DWORD p3)				PROXY3(StartModeTest)
		STDMETHOD(EvaluateMode)(THIS_ DWORD p1, DWORD * p2)							PROXY2(EvaluateMode)


		//
		// Proxy Functions
		//
		//ULONG   Proxy_Release(void);
		//HRESULT Proxy_CreateSurface(LPDDSURFACEDESC2 p1, LPDIRECTDRAWSURFACE7 FAR * p2, IUnknown FAR * p3);
		//HRESULT Proxy_GetDisplayMode(LPDDSURFACEDESC2 p1);
		//HRESULT Proxy_SetCooperativeLevel(HWND p1, DWORD p2);
		//HRESULT Proxy_SetDisplayMode(DWORD p1, DWORD p2, DWORD p3, DWORD p4, DWORD p5);
		//HRESULT Proxy_WaitForVerticalBlank(DWORD dwFlags, HANDLE hEvent);
		HRESULT Proxy_QueryInterface(THIS_ REFIID riid, LPVOID FAR * ppvObj);
		//HRESULT Proxy_RestoreAllSurfaces(void);
};
#undef CLASSNAME