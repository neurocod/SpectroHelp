//win.h by Kostya Kozachuck as neurocod
#pragma once

#ifdef WIN32

#undef LPCSTR
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN			 // Exclude rarely-used stuff from Windows headers
#define _WINDOWS
#include <windows.h>

#endif