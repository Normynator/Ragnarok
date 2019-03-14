#pragma once
#include "norm.h"
#define INITGUID
#include <ddraw.h>
#include <d3d.h>

#define PROXY0(name) { return m_Instance->name();							}
#define PROXY1(name) { return m_Instance->name(p1);							}
#define PROXY2(name) { return m_Instance->name(p1, p2);						}
#define PROXY3(name) { return m_Instance->name(p1, p2, p3);					}
#define PROXY4(name) { return m_Instance->name(p1, p2, p3, p4);				}
#define PROXY5(name) { return m_Instance->name(p1, p2, p3, p4, p5);			}
#define PROXY6(name) { return m_Instance->name(p1, p2, p3, p4, p5 ,p6);		}
#define PROXY7(name) { return m_Instance->name(p1, p2, p3, p4, p5 ,p6 ,p7);	}
#define PROXY_RELEASE													\
	{																	\
		ULONG Count = m_Instance->Release();								\
		if(Count == 0)													\
			delete this;												\
		return Count;													\
}

int dx_detour(std::shared_ptr<norm_dll::norm> state_);