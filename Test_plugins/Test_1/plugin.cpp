// plugin.cpp
// Windows: g++ -shared -fPIC plugin.cpp -o plugin.dll
// Linux  : g++ -shared -fPIC plugin.cpp -o libplugin.so

#include <iostream>

#ifdef _WIN32
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API __attribute__((visibility("default")))
#endif

extern "C"
{
    PLUGIN_API void initialize()
    {
        std::cout << "Plugin initialized!" << std::endl;
    }

    PLUGIN_API void execute()
    {
        std::cout << "Plugin executing!" << std::endl;
    }

    PLUGIN_API void terminate()
    {
        std::cout << "Plugin terminated!" << std::endl;
    }
}