
#include "PluginManager.h"

#include <iostream>
#include <filesystem>

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
        std::cerr << "Failed to load: " << path << '\n';
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
        std::cerr << "GetPluginAPI not found\n";

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

    plugins.push_back({library, api, instance});

    std::cout << "Loaded: " << api->name << '\n';

    return true;
}

int PluginManager::loadAll(const std::string& directory)
{
    int count = 0;

    for (const auto& entry :
         std::filesystem::directory_iterator(directory))
    {
#ifdef _WIN32
        if (entry.path().extension() == ".dll")
#else
        if (entry.path().extension() == ".so")
#endif
        {
            if (loadPlugin(entry.path().string()))
                count++;
        }
    }

    return count;
}

void PluginManager::executeAll()
{
    for (auto& plugin : plugins)
    {
        plugin.api->execute(plugin.instance);
    }
}

void PluginManager::unloadAll()
{
    for (auto& plugin : plugins)
    {
        plugin.api->shutdown(plugin.instance);
        plugin.api->destroy(plugin.instance);

#ifdef _WIN32
        FreeLibrary(plugin.library);
#else
        dlclose(plugin.library);
#endif
    }

    plugins.clear();
}

size_t PluginManager::getPluginCount() const
{
    return plugins.size();
}
