#include "handler.hpp"

json Handler::get_json_object()
{
    std::ifstream file(file_name);

    if (!file.is_open())
    {
        warn("Failed to open '%s'!", file_name.c_str());
        return json();
    }

    json j_obj;
    file >> j_obj;
    file.close();

    return j_obj;
}

std::vector<Film> Handler::get_films()
{
    json j_obj = get_json_object();

    if (j_obj.dump() == "null")
    {
        return std::vector<Film>();
    }

    std::vector<Film> f_vec;

    for (auto it = j_obj.begin(); it != j_obj.end(); it++)
    {
        std::string d = it.key();
        json j_films = it.value();

        for (const json& d_film : j_films)
        {
            std::string t, y, g = "error";

            if (d_film.find("title") != d_film.end())
                t = d_film["title"];
            if (d_film.find("year")  != d_film.end())
                y = d_film["year"];
            if (d_film.find("genre") != d_film.end())
                g = d_film["genre"];

            if (t == "error" || y == "error" || g == "error")
            {
                warn("Error creating film object! Title: %s, Year: %s, Genre: %s.", t.c_str(), y.c_str(), g.c_str());
                return std::vector<Film>();
            }

            f_vec.push_back(Film(t, y, d, g));
        }
    }
    return f_vec;
}

std::vector<Film> Handler::get_films(std::string genre)
{
    json j_obj = get_json_object();

    if (j_obj.dump() == "null")
    {
        return std::vector<Film>();
    }

    std::vector<Film> f_vec;

    std::vector<std::string> v = { "films_seen", "films_to_see" }; // remove this?

    for (auto it = j_obj.begin(); it != j_obj.end(); it++)
    {
        std::string d = it.key();
        json j_films = it.value();

        for (const json& d_film : j_films)
        {
            std::string t, y, g = "error";

            if (d_film.find("title") != d_film.end())
                t = d_film["title"];
            if (d_film.find("year")  != d_film.end())
                y = d_film["year"];
            if (d_film.find("genre") != d_film.end())
                g = d_film["genre"];

            if (t == "error" || y == "error" || g == "error")
            {
                warn("Error creating film object! Title: %s, Year: %s, Genre: %s.", t.c_str(), y.c_str(), g.c_str());
                return std::vector<Film>();
            }

            if (g != genre)
                continue;

            f_vec.push_back(Film(t, y, d, g));
        }
    }

    return f_vec;
}

void Handler::set_file(std::string fn)
{
    file_name = fn;
}

int Handler::get_film()
{
    std::vector<Film> films = get_films();

    if (films.empty())
    {
        warn("No films found in '%s'", file_name.c_str());
        return EXIT_FAILURE;
    }

    std::shuffle(films.begin(), films.end(), random::gen());
    Film film = films.front();

    print_film(film);

    return EXIT_SUCCESS;
}

int Handler::get_film_by_genre()
{
    std::string genre = get_genre();
    std::vector<Film> films = get_films(genre);

    if (films.empty())
    {
        warn("No films with genre '%s' found!", genre.c_str());
        return EXIT_FAILURE;
    }

    std::shuffle(films.begin(), films.end(), random::gen());
    Film film = films.front();

    print_film(film);

    return EXIT_SUCCESS;
}

int Handler::add_film()
{

    std::string director_name, film_title, film_year, film_genre;

    printf("\nDirector name: ");
    std::getline(std::cin, director_name);
    sanitise(director_name);

    printf("\nTitle: ");
    std::getline(std::cin, film_title);
    sanitise(film_title);

    printf("\nYear: ");
    std::getline(std::cin, film_year);
    sanitise(film_year);
    if (film_year.length() > 4)
    {
        film_year = film_year.substr(0, 4);
    }
    film_year.insert(film_year.begin(), '(');
    film_year.push_back(')');

    printf("\nGenre: ");
    std::getline(std::cin, film_genre);
    make_lower(film_genre);
    sanitise(film_genre);

    Film new_film(film_title, film_year, director_name, film_genre);

    int result = update_list(new_film);

    if (result != EXIT_SUCCESS)
    {
        warn("update_list() returned with exit code %d", EXIT_FAILURE);
        return EXIT_FAILURE;
    }
    else
    {
        good("%s added!", film_title.c_str());
        return EXIT_SUCCESS;
    }
}

int Handler::update_list(Film film)
{
    json films = get_json_object();

    if (films.dump() == "null")
    {
        char input;
        printf("Create new file? Y / N: ");
        scanf("%c", &input);

        if (input == 'Y' || input == 'y')
        {
            make_file(file_name);
        }
        else if (input == 'N' || input == 'n')
        {
            info("File not created! Exiting...", NULL);
            return EXIT_FAILURE;
        }
        else
        {
            warn("Invalid input! Exiting...", NULL);
            return EXIT_FAILURE;
        }
    }

    json data_to_write = 
    {
        { "title", film.title },
        { "year",  film.year  },
        { "genre", film.genre }
    };

    std::string s_director = film.director;

    films[s_director].push_back(data_to_write);

    std::ofstream to_save(file_name);

    if (!to_save.is_open())
    {
        warn("Could not open %s", file_name.c_str());
        return EXIT_FAILURE;
    }

    to_save << std::setw(4) << films << std::endl;
    to_save.close();

    return EXIT_SUCCESS;
}

void Handler::sanitise(std::string& text)
{
    while (text.back() == ' ')
    {
        text.pop_back();
    }
}

void Handler::print_film(Film film)
{
    std::cout << std::endl;
    std::cout << "\t" << film.title;
    std::cout << "   " << film.year;
    std::cout << std::endl;
    std::cout << "\t" << film.director;
    std::cout << std::endl << std::endl;
}

void Handler::make_lower(std::string& str)
{
    for (auto& c : str)
    {
        c = tolower(c);
    }
}

int Handler::remove_film()
{
    std::string title;
    std::cout << "\nTitle: ";
    std::getline(std::cin, title);
    make_lower(title);
    sanitise(title);

    json j_obj = get_json_object();

    if ( j_obj.dump() == "null")
    {
        return EXIT_FAILURE;
    }

    std::vector<Film> films;

    bool film_found = false;

    for (auto it = j_obj.begin(); it != j_obj.end(); it++)
    {
        json director_list = it.value();
        for (const json& d_film : director_list)
        {
            std::string title_cpy = d_film["title"];
            make_lower(title_cpy);

            if (title == title_cpy && !film_found)
            {
                film_found = true;
                continue;
            }

            std::string d = it.key();

            std::string t, y, g = "error";

            if (d_film.find("title") != d_film.end())
                t = d_film["title"];
            if (d_film.find("year")  != d_film.end())
                y = d_film["year"];
            if (d_film.find("genre") != d_film.end())
                g = d_film["genre"];

            if (t == "error" || y == "error" || g == "error")
            {
                warn("Error creating film object! Title: %s, Year: %s, Genre: %s.", t.c_str(), y.c_str(), g.c_str());
                return EXIT_FAILURE;
            }

            Film film(t, y, d, g);

            films.push_back(film);
        }
    }

    if (!film_found)
    {
        warn("%s was not found!", title.c_str());
        return EXIT_FAILURE;
    }

    j_obj.clear();
    std::ofstream o_file(file_name);
    o_file << std::setw(4) << j_obj << std::endl;
    o_file.close();

    for (auto& film : films)
    {
        update_list(film);
    }

    good("%s removed from %s", title.c_str(), file_name.c_str());
    return EXIT_SUCCESS;
}

int Handler::find_film()
{
    std::string film_title;

    printf("\nFind film: ");
    std::getline(std::cin, film_title);
    sanitise(film_title);

    json j_obj = get_json_object();

    if ( j_obj.dump() == "null")
    {
        return EXIT_FAILURE;
    }

    bool found = false;

    for (auto it = j_obj.begin(); it != j_obj.end(); it++)
    {
        if (found)
            break;

        json director_list = it.value();

        for (const json& d_film : director_list)
        {
            if (found)
                break;

            std::string title_to_compare;

            title_to_compare = d_film["title"];

            if (title_to_compare == film_title)
                found = true;
        }
    }

    if (!found)
    {
        warn("%s was not found!", film_title.c_str());
        return EXIT_FAILURE;
    }
    else
    {
        good("%s was found!", film_title.c_str());
        return EXIT_SUCCESS;
    }
}

int Handler::make_file()
{
    return make_file(file_name);
}

int Handler::make_file(std::string param)
{
    std::ifstream file_exists(param);

    if (file_exists.is_open())
    {
        warn("File '%s' already exists!", param.c_str());
        file_exists.close();
        return EXIT_FAILURE;
    }
 
    std::string empty_template = "{\n}";
    std::ofstream new_file(param);

    new_file << empty_template;

    new_file.close();

    good("File '%s' created!", param.c_str());
    return EXIT_SUCCESS;
}

int Handler::copy_file(std::string param)
{
    json to_copy = get_json_object();

    if (to_copy.dump() == "null")
    {
        return EXIT_FAILURE;
    }

    int result = make_file(param);

    if (result == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    std::ofstream to_stream(param);

    if (!to_stream.is_open())
    {
        warn("Cannot open newly created file '%s'!", param.c_str());
        return EXIT_FAILURE;
    }

    to_stream << std::setw(4) << to_copy;

    to_stream.close();

    return EXIT_SUCCESS;
}

int Handler::delete_file()
{
    if (!std::filesystem::exists(file_name))
    {
        warn("'%s' not found!", file_name.c_str());
        return EXIT_FAILURE;
    }

    std::filesystem::remove(file_name);

    if (std::filesystem::exists(file_name))
    {
        warn("Failed to delete '%s'!", file_name.c_str());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int Handler::get_help()
{
    puts("");
    info("Pass one command to the program to run.\n", NULL);;
    puts("Commands are:\n");
    puts("HELP     --   get help.\n");
    puts("    <-- FILM COMMANDS -->\n");
    puts("GET      --   get random film.");
    puts("GENRE    --   get random films by genre.");
    puts("WRITE    --   add film to list.");
    puts("REMOVE   --   remove film from list.");
    puts("FIND     --   find film in list.\n");
    puts("    <-- FILE COMMANDS -->\n");
    puts("MAKEFILE --   create new file.");
    puts("COPYFILE --   create duplicate file.");
    puts("DELFILE  --   delete file.\n");
    puts("    <-- PARAMETERS -->\n");
    puts("Write filename preceded by - to indicate file.");
    puts("e.g. -newfile will use file 'newfile.json'.\n");
    puts("All commands accept parameters. File commands require parameters.\n");
    puts("MAKEFILE --   1 param:  file name to create.");
    puts("COPYFILE --   2 params: file to copy; new file name.");
    puts("DELFILE  --   1 param:  file to delete.\n");

    return EXIT_SUCCESS;
}

std::string Handler::get_param(std::string arg)
{
    if (std::string(arg).substr(0, 1) != "-")
    {
        warn("'%s' is not a parameter!", arg);
        return "ERR";
    }
    else
    {
        arg = arg.substr(1);

        size_t pos = arg.find_last_of('.');
        bool file_type_included = (pos != std::string::npos);

        if (file_type_included)
        {
            if (pos + 5 == arg.length())
            {
                std::string filetype = arg.substr(pos);
                if (filetype != ".json")
                {
                    warn("File format %s is not accepted. Only .json files accepted!", filetype.c_str());
                    return "ERR";
                }
            }
        }
        else
        {
            arg.append(".json");
        }
       return arg;
    }
}

std::string Handler::get_genre()
{
    std::string genre;
    std::cout << "\nGenre: ";
    std::cin >> genre;
    make_lower(genre);
    return genre;
}
