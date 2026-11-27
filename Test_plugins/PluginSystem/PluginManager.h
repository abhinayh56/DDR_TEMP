
#pragma once

#include <string>
#include <vector>
#include "PluginInterface.h"

#ifdef _WIN32
    #include <windows.h>
    using LibraryHandle = HMODULE;
#else
    using LibraryHandle = void*;
#endif

class PluginManager
{
private:

    struct LoadedPlugin
    {
        LibraryHandle library = nullptr;
        const PluginAPI* api = nullptr;
        void* instance = nullptr;
    };

    std::vector<LoadedPlugin> plugins;

public:

    PluginManager() = default;
    ~PluginManager();

    bool loadPlugin(const std::string& path);

    int loadAll(const std::string& directory);

    void executeAll();

    void unloadAll();

    size_t getPluginCount() const;
};
