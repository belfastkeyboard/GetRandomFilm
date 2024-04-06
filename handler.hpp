#ifndef     HANDLER
#define     HANDLER 1

#include    <vector>
#include    <fstream>
#include    <iostream>
#include    <cctype>
#include    <filesystem>
#include    "film.hpp"
#include    "json.hpp"
#include    "rng.hpp"
#include    "status.h"

using json = nlohmann::json;

class Handler
{

private:

    std::string file_name = "list.json";

    json get_json_object();

    std::vector<Film> get_films();
    std::vector<Film> get_films(std::string genre);
    void make_lower(std::string& str);
    int update_list(Film film);

    void sanitise(std::string& text);

    void print_film(Film film);
    std::string get_genre();

public:

    void set_file(std::string fn);

    int get_film();
    int get_film_by_genre();
    int add_film();
    int remove_film();
    int find_film();

    int make_file();
    int make_file(std::string param);
    int copy_file(std::string param);
    int delete_file();

    int get_help();

    std::string get_param(std::string arg);
};

#endif