#pragma once

#ifdef PLATFORM_WINDOWS
    #ifdef DYNAMIC_LINK
        #ifdef BUILD_DLL
            #define DEVELOPING_API __declspec(dllexport)
        #else
            #define DEVELOPING_API __declspec(dllimport)
        #endif
    #else
            #define DEVELOPING_API 
    #endif
#else
    #error WINDOWS ONLY!
#endif

#ifdef DEBUG
    #define ENABLE_ASSERTS
#endif

#ifdef ENABLE_ASSERTS
	#define ASSERT(x, ...) { if(!(x)) { ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CORE_ASSERT(x, ...) { if(!(x)) { CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define ASSERT(x, ...)
	#define CORE_ASSERT(x, ...)
#endif
