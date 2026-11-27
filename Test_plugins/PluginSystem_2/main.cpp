// g++ -std=c++17 main.cpp PluginManager.cpp -o bin/main.exe


#include <iostream>
#include <string>
#include "PluginManager.h"

int main()
{
    PluginManager manager;

    int choice = 0;

    while (true)
    {
        std::cout << "\n================================\n";
        std::cout << "       PLUGIN APPLICATION\n";
        std::cout << "================================\n";

        std::cout << "Loaded plugins: "
                  << manager.getPluginCount() << "\n\n";

        std::cout << "1. Load plugin\n";
        std::cout << "2. Execute plugin\n";
        std::cout << "3. Unload plugin\n";
        std::cout << "4. List loaded plugins\n";
        std::cout << "5. Exit\n";

        std::cout << "\nSelect option: ";

        if (!(std::cin >> choice))
        {
            if (std::cin.eof())
                break;

            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input\n";
            continue;
        }

        switch (choice)
        {
            case 1:
            {
                std::string path;

                std::cout << "Enter plugin DLL path: ";
                std::cin >> std::ws;
                std::getline(std::cin, path);

                manager.loadPlugin(path);

                break;
            }

            case 2:
            {
                manager.listPlugins();

                if (manager.getPluginCount() == 0)
                    break;

                size_t index;

                std::cout << "Select plugin number: ";
                std::cin >> index;

                if (index == 0 ||
                    !manager.executePlugin(index - 1))
                {
                    std::cout << "Invalid plugin selection\n";
                }

                break;
            }

            case 3:
            {
                manager.listPlugins();

                if (manager.getPluginCount() == 0)
                    break;

                size_t index;

                std::cout << "Select plugin number: ";
                std::cin >> index;

                if (index == 0 ||
                    !manager.unloadPlugin(index - 1))
                {
                    std::cout << "Invalid plugin selection\n";
                }

                break;
            }

            case 4:
            {
                manager.listPlugins();
                break;
            }

            case 5:
            {
                manager.unloadAll();

                std::cout << "Application terminated\n";
                return 0;
            }

            default:
            {
                std::cout << "Invalid option\n";
                break;
            }
        }
    }

    return 0;
}
