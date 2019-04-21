#pragma once

// all common Pyro definitions

#ifdef PYRO_PLATFORM_WIN

#if PYRO_DYNAMIC
    #ifdef PYRO_BUILD_DLL
        #define PYRO_API __declspec(dllexport)
    #else
        #define PYRO_API __declspec(dllimport)
    #endif
#else
    #define PYRO_API
#endif

#else
    #error Pyro only supports Windows!
#endif

#if _DEBUG
    #define ENABLE_ASSERTS
#endif

#ifdef ENABLE_ASSERTS
    #define ASSERT(x, ...) { if(x) {PYRO_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
    #define CORE_ASSERT(x, ...) { if(x) {PYRO_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
    #define ASSERT(x, ...)
    #define CORE_ASSERT(x, ...)
#endif

// 1 shifted by x places
// 1 << 0 = `0000 0001`
// 1 << 1 = `0000 0010`
// 1 << 2 = `0000 0100`
#define BIT(x) 1 << x

#if PYRO_DEBUG
    #define PYRO_ENABLE_ASSERTS
#endif

#ifdef PYRO_ENABLE_ASSERTS
    #define PYRO_ASSERT(x, ...) { if(!x) {PYRO_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
    #define PYRO_CORE_ASSERT(x, ...) { if(!x) {PYRO_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
    #define PYRO_ASSERT(x, ...)
    #define PYRO_CORE_ASSERT(x, ...)
#endif

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

// macros to safely delete memory allocated on the heap
#define SAFE_RELEASE(p) if(p) { delete p; p = nullptr; }
#define SAFE_RELEASES(p) if(p) { delete[] p; p = nullptr; }

#include <cstdint>
// Custom types
typedef uint8_t   byte;
typedef uint8_t   uint8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint64_t  uint64;

typedef int8_t    sbyte;
typedef int8_t    int8;
typedef int16_t   int16;
typedef int32_t   int32;
typedef int64_t   int64;

#define S(x) #x
#define SS(x) S(x)

#define MAJOR_VERSION 0
#define MINOR_VERSION 1
#define REVISION 0
#define VERSION_STRING SS(MAJOR_VERSION.MINOR_VERSION.REVISION)
