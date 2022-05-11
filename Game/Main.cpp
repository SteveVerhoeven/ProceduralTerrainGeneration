// >----------------------------------< //
// > Copyright 2022 - Vanaheim Game   < //
// > Author: Steve Verhoeven		  < //
// >----------------------------------< //
#include "GamePCH.h"

// Visual Leak Detector
#if defined(DEBUG) | defined(_DEBUG)
#include <vld.h>
#endif

// Game include
#include "VGame.h"

// Application Entry Point - References:
// - https://docs.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
// - https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	VGame vGame{ "Vanaheim Project Game - Making own game" };
	vGame.Initialize(hInstance);
	vGame.PostInitialize();
	return vGame.GameLoop();
}