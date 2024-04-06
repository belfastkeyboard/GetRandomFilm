#ifndef     FILM
#define     FILM 1

#include <string>

struct Film
{
    std::string title, year, director, genre;
    Film(std::string t, std::string y, std::string d, std::string g) : title(t), year(y), director(d), genre(g) {}
};

#endif