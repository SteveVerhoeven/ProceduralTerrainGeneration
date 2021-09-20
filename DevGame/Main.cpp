// >----------------------------------< //
// > Copyright 2021 - Vanaheim Game   < //
// > Author: Steve Verhoeven		  < //
// >----------------------------------< //
#include "pch.h"

// Visual Leak Detector
#if defined(DEBUG) | defined(_DEBUG)
#include <vld.h>
#endif

// Game include
#include "DevGame.h"

// Application Entry Point - References:
// - https://docs.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
// - https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	DevGame devGame{"Development Game - Extending engine"};
	devGame.Initialize(hInstance);
	devGame.PostInitialize();
	return devGame.GameLoop();
}


