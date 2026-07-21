#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

#include <string>

namespace text_utils {

std::string toLower(const std::string& input);

// strips leading/trailing whitespace and a trailing . ! ?, then lowercases
std::string normalize(const std::string& input);

// swaps pronouns so "I have my keys" becomes "you have your keys"
std::string reflect(const std::string& input);

// replaces every occurrence of `placeholder` in `tmpl` with `value`
std::string fillTemplate(const std::string& tmpl, const std::string& placeholder,
                          const std::string& value);

}

#endif
