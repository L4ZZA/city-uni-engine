#pragma once
// all common engine definitions

#ifdef ENGINE_PLATFORM_WIN

#if ENGINE_DYNAMIC
    #ifdef ENGINE_BUILD_DLL
        #define ENGINE_API __declspec(dllexport)
    #else
        #define ENGINE_API __declspec(dllimport)
    #endif
#else
    #define ENGINE_API
#endif

#else
    #error engine only supports Windows!
#endif

// 1 shifted by x places
// 1 << 0 = `0000 0001`
// 1 << 1 = `0000 0010`
// 1 << 2 = `0000 0100`
#define BIT(x) 1 << x

#if ENGINE_DEBUG
    #define ENGINE_ENABLE_ASSERTS
#endif

#ifdef ENGINE_ENABLE_ASSERTS
    #define ENGINE_ASSERT(x, ...) { if(!(x)) {LOG_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
    #define CORE_ASSERT(x, ...) { if(!(x)) {LOG_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
    #define ENGINE_ASSERT(x, ...)
    #define CORE_ASSERT(x, ...)
#endif

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#include <memory>
namespace engine
{
    /// \brief safe and scoped pointer to an object. [Like unique_ptr]
    template<typename T>
    using scope = std::unique_ptr<T>;

    /// \brief ref counted pointer to an object. [Usually used for resources]
    template<typename T>
    using ref = std::shared_ptr<T>;
}
