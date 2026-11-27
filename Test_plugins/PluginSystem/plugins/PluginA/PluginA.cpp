// g++ -std=c++17 -shared -I. plugins/PluginA/PluginA.cpp -o bin/plugins/PluginA.dll

#include <iostream>
#include "../../PluginInterface.h"

struct PluginA
{
    int counter = 0;
};

static void* PLUGIN_CALL create()
{
    return new PluginA();
}

static int PLUGIN_CALL initialize(void* instance)
{
    std::cout << "Plugin A initialized\n";
    return 0;
}

static void PLUGIN_CALL execute(void* instance)
{
    auto* plugin = static_cast<PluginA*>(instance);

    plugin->counter++;

    std::cout << "Plugin A executing: "
              << plugin->counter << std::endl;
}

static void PLUGIN_CALL shutdown(void* instance)
{
    std::cout << "Plugin A shutdown\n";
}

static void PLUGIN_CALL destroy(void* instance)
{
    delete static_cast<PluginA*>(instance);
}

static const PluginAPI api =
{
    PLUGIN_API_VERSION,
    "Plugin A",
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
