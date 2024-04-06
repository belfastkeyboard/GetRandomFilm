#include "arguments.hpp"

int Arguments::maximum_params_allowed()
{
    if (command == "NULL")
    {
        warn("Command not defined!", NULL);
        return -1;
    }

    if (command != "COPYFILE")
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

bool Arguments::correct_number_of_params()
{
    if (command == "COPYFILE")
    {
        if (params.size() > 2)
        {
            warn("%d is too many parameters. %s requires 2 parameters.", params.size(), command.c_str());
            return false;
        }
        else if (params.size() < 2)
        {
            warn("%d is too few parameters. %s requires 2 parameters.", params.size(), command.c_str());
            return false;
        }
        else
        {
            return true;
        }
    }
    else if (command == "MAKEFILE" || command == "DELFILE")
    {
        if (params.size() > 1)
        {
            warn("%d is too many parameters. %s requires 1 parameter.", params.size(), command.c_str());
            return false;
        }
        else if (params.size() < 1)
        {
            warn("%d is too few parameters. %s requires 1 parameter.", params.size(), command.c_str());
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        if (params.size() > 1)
        {
            warn("%d is too many parameters. %s accepts 1 parameter maximum.", params.size(), command.c_str());
            return false;
        }
        else
        {
            return true;
        }
    }
}

bool Arguments::is_command(const char *to_check)
{
    if (is_param(to_check))
        return false;
    for (auto& c : std::string(to_check))
    {
        if (!std::isupper(c))
            return false;
    }
    return true;
}

bool Arguments::is_param(const char *to_check)
{
    return (std::string(to_check).substr(0, 1) == "-");
}

bool Arguments::is_command_found()
{
    return (command != "NULL");
}

int Arguments::get_arguments(int argc, char *argv[])
{
    if (argc > 5)
    {
        warn("%d is too many arguments!", argc - 1);
        problem_found = true;
        return EXIT_FAILURE;
    }

    for (size_t i = 1; i < argc; i++)
    {
        if (is_command(argv[i]))
        {
            if (is_command_found())
            {
                warn("Too many commands! Only 1 command accepted.", NULL);
                problem_found = true;
                break;
            }
            else
            {
                command = argv[i];
                continue;
            }
        }
        else if (is_param(argv[i]))
        {
            params.push_back(argv[i]);
        }
        else
        {
            warn("%s is invalid!", argv[i]);
            problem_found = true;
            break;
        }
    }

    if (problem_found)
        return EXIT_FAILURE;

    problem_found = !correct_number_of_params();

    if (problem_found)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

std::string Arguments::get_command()
{
    if (problem_found)
    {
        return "ERR";
    }
    else
    {
        return command;
    }
}

std::string Arguments::get_file_name(int x)
{
    return params.at(x);
}

int Arguments::get_params_number()
{
    if (problem_found)
    {
        return -1;
    }
    else
    {
        return params.size();
    }
}
