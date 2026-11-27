
#include "PluginManager.h"

#include <iostream>

#ifndef _WIN32
    #include <dlfcn.h>
#endif

PluginManager::~PluginManager()
{
    unloadAll();
}

bool PluginManager::loadPlugin(const std::string& path)
{
    LibraryHandle library = nullptr;

#ifdef _WIN32

    library = LoadLibraryA(path.c_str());

    if (!library)
    {
        std::cerr << "Failed to load DLL\n";
        return false;
    }

    auto getAPI = reinterpret_cast<GetPluginAPIFunc>(
        GetProcAddress(library, "GetPluginAPI"));

#else

    library = dlopen(path.c_str(), RTLD_NOW);

    if (!library)
    {
        std::cerr << dlerror() << '\n';
        return false;
    }

    auto getAPI = reinterpret_cast<GetPluginAPIFunc>(
        dlsym(library, "GetPluginAPI"));

#endif

    if (!getAPI)
    {
        std::cerr << "Plugin API not found\n";

#ifdef _WIN32
        FreeLibrary(library);
#else
        dlclose(library);
#endif
        return false;
    }

    const PluginAPI* api = getAPI();

    if (!api ||
        api->version != PLUGIN_API_VERSION ||
        !api->name ||
        !api->create ||
        !api->initialize ||
        !api->execute ||
        !api->shutdown ||
        !api->destroy)
    {
        std::cerr << "Invalid plugin interface\n";

#ifdef _WIN32
        FreeLibrary(library);
#else
        dlclose(library);
#endif
        return false;
    }

    // Prevent loading the same plugin twice.
    for (const auto& plugin : plugins)
    {
        if (plugin.path == path)
        {
            std::cout << "Plugin already loaded\n";

#ifdef _WIN32
            FreeLibrary(library);
#else
            dlclose(library);
#endif
            return false;
        }
    }

    void* instance = api->create();

    if (!instance)
    {
        std::cerr << "Plugin instance creation failed\n";

#ifdef _WIN32
        FreeLibrary(library);
#else
        dlclose(library);
#endif
        return false;
    }

    if (api->initialize(instance) != 0)
    {
        api->destroy(instance);

#ifdef _WIN32
        FreeLibrary(library);
#else
        dlclose(library);
#endif
        return false;
    }

    plugins.push_back({library, api, instance, path});

    std::cout << "Loaded plugin: "
              << api->name << '\n';

    return true;
}

bool PluginManager::executePlugin(size_t index)
{
    if (index >= plugins.size())
        return false;

    auto& plugin = plugins[index];

    plugin.api->execute(plugin.instance);

    return true;
}

bool PluginManager::unloadPlugin(size_t index)
{
    if (index >= plugins.size())
        return false;

    auto& plugin = plugins[index];

    plugin.api->shutdown(plugin.instance);
    plugin.api->destroy(plugin.instance);

#ifdef _WIN32
    FreeLibrary(plugin.library);
#else
    dlclose(plugin.library);
#endif

    std::cout << "Unloaded plugin: "
              << plugin.api->name << '\n';

    plugins.erase(plugins.begin() + index);

    return true;
}

void PluginManager::listPlugins() const
{
    if (plugins.empty())
    {
        std::cout << "No plugins loaded\n";
        return;
    }

    std::cout << "\nLoaded plugins:\n";

    for (size_t i = 0; i < plugins.size(); i++)
    {
        std::cout << i + 1 << ". "
                  << plugins[i].api->name << '\n';
    }
}

size_t PluginManager::getPluginCount() const
{
    return plugins.size();
}

void PluginManager::unloadAll()
{
    while (!plugins.empty())
    {
        unloadPlugin(plugins.size() - 1);
    }
}
