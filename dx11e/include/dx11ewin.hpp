#ifndef _DX12E_WIN_
#define _DX12E_WIN_

#pragma once

// Targeting Windows 10
#define _WIN32_WINNT_WINTHRESHOLD   0x0A00
#define _WIN32_WINNT_WIN10          0x0A00

#include <sdkddkver.h>

#ifndef FULL_WINTARD
#define WIN32_LEAN_AND_MEAN
#define NOSYSMETRICS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOCLIPBOARD
#define NOCTLMGR
#define NODRAWTEXT
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#endif

#define NOMINMAX

#include <Windows.h>

#include <Core.hpp>

#endif