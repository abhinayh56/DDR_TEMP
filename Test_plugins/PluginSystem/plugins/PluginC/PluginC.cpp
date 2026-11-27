// g++ -std=c++17 -shared -I. plugins/PluginC/PluginC.cpp -o bin/plugins/PluginC.dll

#include <iostream>
#include "../../PluginInterface.h"

struct PluginC
{
    double value = 10.5;
};

static void* PLUGIN_CALL create()
{
    return new PluginC();
}

static int PLUGIN_CALL initialize(void* instance)
{
    std::cout << "Plugin C initialized\n";
    return 0;
}

static void PLUGIN_CALL execute(void* instance)
{
    auto* plugin = static_cast<PluginC*>(instance);

    plugin->value *= 2.0;

    std::cout << "Plugin C executing: "
              << plugin->value << std::endl;
}

static void PLUGIN_CALL shutdown(void* instance)
{
    std::cout << "Plugin C shutdown\n";
}

static void PLUGIN_CALL destroy(void* instance)
{
    delete static_cast<PluginC*>(instance);
}

static const PluginAPI api =
{
    PLUGIN_API_VERSION,
    "Plugin C",
    create,
    initialize,
    execute,
    shutdown,
    destroy
};

PLUGIN_EXPORT const PluginAPI* PLUGIN_CALL GetPluginAPI()
{
    return &api;
}
