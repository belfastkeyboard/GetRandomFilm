#ifndef ARGUMENTS
#define ARGUMENTS 1

#include <vector>
#include <string>
#include <cctype>
#include "status.h"

class Arguments
{

private:

    std::string command = "NULL";
    std::vector<std::string> params;

    int maximum_params_allowed();
    bool correct_number_of_params();

    bool is_command(const char* to_check);
    bool is_param(const char* to_check);

    bool is_command_found();

    bool problem_found = false;

public:

    int get_arguments(int argc, char* argv[]);
    int get_params_number();
    
    std::string get_command();
    std::string get_file_name(int x);

};

#endif