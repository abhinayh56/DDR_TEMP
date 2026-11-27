
#pragma once

// Plugin interface shared between application and DLLs.

#define PLUGIN_API_VERSION 1

#ifdef _WIN32
    #define PLUGIN_CALL __cdecl
    #define PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
    #define PLUGIN_CALL
    #define PLUGIN_EXPORT extern "C" __attribute__((visibility("default")))
#endif

struct PluginAPI
{
    unsigned int version;
    const char* name;

    // Plugin lifecycle
    void* (PLUGIN_CALL *create)();
    int   (PLUGIN_CALL *initialize)(void* instance);
    void  (PLUGIN_CALL *execute)(void* instance);
    void  (PLUGIN_CALL *shutdown)(void* instance);
    void  (PLUGIN_CALL *destroy)(void* instance);
};

// Every plugin must export this function.
using GetPluginAPIFunc = const PluginAPI* (PLUGIN_CALL *)();
