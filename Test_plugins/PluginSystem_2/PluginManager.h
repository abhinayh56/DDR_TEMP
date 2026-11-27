
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
        std::string path;
    };

    std::vector<LoadedPlugin> plugins;

public:

    PluginManager() = default;
    ~PluginManager();

    bool loadPlugin(const std::string& path);

    bool executePlugin(size_t index);

    bool unloadPlugin(size_t index);

    void listPlugins() const;

    size_t getPluginCount() const;

    void unloadAll();
};
