// main.cpp
// Windows: g++ main.cpp -o main.exe
// Linux  : g++ main.cpp -o main -ldl

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

int main()
{
    std::cout << "Main application started" << std::endl;

#ifdef _WIN32

    // Load DLL
    HMODULE library = LoadLibraryA("plugin.dll");

    if (!library)
    {
        std::cerr << "Failed to load library" << std::endl;
        return -1;
    }

    // Get function pointers
    auto initialize = (void (*)())GetProcAddress(library, "initialize");
    auto execute = (void (*)())GetProcAddress(library, "execute");
    auto terminate = (void (*)())GetProcAddress(library, "terminate");

#else

    // Load shared library
    void* library = dlopen("./libplugin.so", RTLD_NOW);

    if (!library)
    {
        std::cerr << "Failed to load library: " << dlerror() << std::endl;
        return -1;
    }

    // Get function pointers
    auto initialize = (void (*)())dlsym(library, "initialize");
    auto execute = (void (*)())dlsym(library, "execute");
    auto terminate = (void (*)())dlsym(library, "terminate");

#endif

    // Execute plugin functions
    if (initialize && execute && terminate)
    {
        initialize();
        execute();
        terminate();
    }

#ifdef _WIN32
    FreeLibrary(library);
#else
    dlclose(library);
#endif

    std::cout << "Main application terminated" << std::endl;

    return 0;
}