// category.h -- the record each topic component is registered as: a name plus
// its matches/respond function pointers.

#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>

struct Category {
    std::string name;
    bool (*matches)(const std::string& input);
    std::string (*respond)(const std::string& input);
};

#endif
