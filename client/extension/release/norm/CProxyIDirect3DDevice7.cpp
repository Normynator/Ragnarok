#include "stdafx.h"
#include "CProxyIDirect3DDevice7.h"

void CProxyIDirect3DDevice7::Proxy_Release(void)
{
}

HRESULT CProxyIDirect3DDevice7::Proxy_BeginScene(void)
{
	HRESULT result;
	/*if (m_firstonce) {
		m_firstonce = false;
		if (g_pRoCodeBind)g_pRoCodeBind->Init(m_Instance);
	}*/
	//m_frameonce = TRUE;

	this->c_state->dbg_sock->do_send("CProxyIDirect3D7::Proxy_BeginScene()");
	for (auto it : this->c_state->mods)
		;

	result = m_Instance->BeginScene();

	return result;
}

HRESULT CProxyIDirect3DDevice7::Proxy_EndScene(void)
{
	this->c_state->dbg_sock->do_send("CProxyIDirect3D7::Proxy_EndScene()");
	//if (g_pRoCodeBind)
	//	g_pRoCodeBind->DrawSRHDebug(m_Instance);


	return m_Instance->EndScene();
}

void CProxyIDirect3DDevice7::add_beginSecene_callback(std::shared_ptr<norm_dll::mod> cb)
{
	//this->beginScene_callbacks.push_back(cb);
}

void CProxyIDirect3DDevice7::add_endSecene_callback(std::shared_ptr<norm_dll::mod> cb)
{
	//this->endScene_callbacks.push_back(cb);
}
