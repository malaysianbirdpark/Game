#pragma once
#ifdef PLATFORM_WINDOWS
    // target Windows 10 or later
    #define _WIN32_WINNT 0x0A00
    #include <sdkddkver.h>

    #define WIN32_LEAN_AND_MEAN
    #define NOGDICAPMASKS
    #define NOSYSMETRICS
    #define NOMENUS
    #define NOICONS
    #define NOSYSCOMMANDS
    #define NORASTEROPS
    #define OEMRESOURCE
    #define NOATOM
    #define NOCLIPBOARD
    //#define NOCOLOR
    //#define NOCTLMGR
    #define NODRAWTEXT
    #define NOKERNEL
    //#define NONLS
    #define NOMEMMGR
    #define NOMETAFILE
    #define NOMINMAX
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

    #define STRICT

    #include <Windows.h>

    #include <wrl.h>
    #include <dxgi.h>
    #include <DirectxMath.h>

    #include <d3d11.h>
    #include <dxgiformat.h>
#endif



