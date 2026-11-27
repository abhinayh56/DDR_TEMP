// g++ -std=c++17 main.cpp PluginManager.cpp -o bin/main.exe

#include <iostream>
#include "PluginManager.h"

int main()
{
    std::cout << "Main application started\n";

    PluginManager manager;

    // Load all DLLs from the plugins directory
    int count = manager.loadAll("bin/plugins");

    std::cout << "\nTotal plugins loaded: "
              << count << "\n";

    // Execute all plugins
    for (int i = 0; i < 3; i++)
    {
        std::cout << "\nExecution cycle: " << i + 1 << "\n";

        manager.executeAll();
    }

    // Unload all plugins
    manager.unloadAll();

    std::cout << "\nMain application terminated\n";

    return 0;
}
