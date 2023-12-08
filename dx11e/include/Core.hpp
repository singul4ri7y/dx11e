#ifndef _DX11E_CORE_
#define _DX11E_CORE_

#pragma once

// Framework API Import/Export definition.
#if defined(DX11E_DLL) 

	#define DX11E_API __declspec(dllexport)
#else 

	#define DX11E_API __declspec(dllimport)
#endif

// Namespaces.
#define DX11E_START  namespace DX11E {
#define DX11E_END    }

// Window Exception

#define DX11E_WINDOW_EXCEPTION(note, hr) throw DX11E::WindowException(__LINE__, _CRT_WIDE(__FILE__), note, hr)

// Graphics Exception

#define DX11E_GFX_EXCEPTION(note, hr) throw DX11E::GraphicsException(__LINE__, _CRT_WIDE(__FILE__), note, hr)

#endif // !_DX11E_CORE_