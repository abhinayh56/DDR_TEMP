// g++ -std=c++17 -shared -I. plugins/PluginB/PluginB.cpp -o bin/plugins/PluginB.dll

#include <iostream>
#include "../../PluginInterface.h"

struct PluginB
{
    double value = 10.5;
};

static void* PLUGIN_CALL create()
{
    return new PluginB();
}

static int PLUGIN_CALL initialize(void* instance)
{
    std::cout << "Plugin B initialized\n";
    return 0;
}

static void PLUGIN_CALL execute(void* instance)
{
    auto* plugin = static_cast<PluginB*>(instance);

    plugin->value *= 2.0;

    std::cout << "Plugin B executing: "
              << plugin->value << std::endl;
}

static void PLUGIN_CALL shutdown(void* instance)
{
    std::cout << "Plugin B shutdown\n";
}

static void PLUGIN_CALL destroy(void* instance)
{
    delete static_cast<PluginB*>(instance);
}

static const PluginAPI api =
{
    PLUGIN_API_VERSION,
    "Plugin B",
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
