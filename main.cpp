#include <iostream>
#include <string>
#include <iomanip>
#include <map>
#include <windows.h>

#include "handler.hpp"
#include "arguments.hpp"

enum class Commands
{
    DEFAULT,
    GET,
    GENRE,
    WRITE,
    REMOVE,
    FIND,
    MAKEFILE,
    COPYFILE,
    DELFILE,
    HELP
};

static std::map<std::string, Commands> GetCommand = {
        { "GET", Commands::GET }, {"GENRE", Commands::GENRE }, {"WRITE", Commands::WRITE }, {"REMOVE", Commands::REMOVE }, {"FIND", Commands::FIND },
        {"MAKEFILE", Commands::MAKEFILE }, {"COPYFILE", Commands::COPYFILE }, {"DELFILE", Commands::DELFILE }, {"HELPCommands", Commands::HELP }
    };

int main(int argc, char* argv[])
{
    Handler handler;
    Arguments arguments;
    Commands COMMAND;

    SetConsoleOutputCP(CP_UTF8);

    if (arguments.get_arguments(argc, argv) == EXIT_FAILURE)
        return EXIT_FAILURE;

    int result = arguments.get_params_number();

    if (result == -1)
    {
        return EXIT_FAILURE;
    }
    else if (result > 0)
    {
        std::string fn = arguments.get_file_name(0);
        fn = handler.get_param(fn);
        handler.set_file(fn);
    }

    std::string command_text = arguments.get_command();

    auto it = GetCommand.find(command_text);
    COMMAND = (it != GetCommand.end()) ? it->second : Commands::DEFAULT;

    switch (COMMAND)
    {
        case Commands::GET:
            result = handler.get_film();
            break;
        case Commands::GENRE:
            result = handler.get_film_by_genre();
            break;
        case Commands::WRITE:
            result = handler.add_film();
            break;
        case Commands::REMOVE:
            result = handler.remove_film();
            break;
        case Commands::FIND:
            result = handler.find_film();
            break;
        case Commands::MAKEFILE:
            result = handler.make_file();
            break;
        case Commands::COPYFILE:
            result = handler.copy_file(handler.get_param(arguments.get_file_name(1)));
            break;
        case Commands::DELFILE:
            result = handler.delete_file();
            break;
        case Commands::HELP:
            result = handler.get_help();
            break;
        
        default:
            warn("Invalid command. Type HELP for help!", NULL);
            return EXIT_FAILURE;;
    }

    if (result == EXIT_FAILURE)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}