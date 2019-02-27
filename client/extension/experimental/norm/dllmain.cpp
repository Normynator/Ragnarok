// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "stdafx.h"

#include <memory>
#include "norm.h"

std::unique_ptr<norm_dll::norm> norm_ptr;

BOOL __declspec(dllexport) WINAPI  DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	// store the address of sum() in testprogram.exe here.
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		norm_ptr = std::make_unique<norm_dll::norm>();
		norm_ptr->start();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}
	return TRUE;
}
