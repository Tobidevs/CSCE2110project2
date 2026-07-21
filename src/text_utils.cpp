#include "text_utils.h"

#include <algorithm>
#include <cctype>
#include <regex>
#include <sstream>
#include <unordered_map>

namespace text_utils {

std::string toLower(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
                    [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string normalize(const std::string& input) {
    size_t start = input.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = input.find_last_not_of(" \t\r\n.!?");
    std::string trimmed = input.substr(start, end - start + 1);
    return toLower(trimmed);
}

std::string reflect(const std::string& input) {
    static const std::unordered_map<std::string, std::string> swapMap = {
        {"i", "you"},       {"me", "you"},       {"my", "your"},
        {"mine", "yours"},  {"myself", "yourself"},
        {"am", "are"},      {"was", "were"},
        {"you", "I"},       {"your", "my"},       {"yours", "mine"},
        {"yourself", "myself"},
    };

    std::istringstream stream(input);
    std::string word;
    std::string result;
    bool first = true;
    while (stream >> word) {
        if (!first) {
            result += " ";
        }
        first = false;

        auto it = swapMap.find(toLower(word));
        result += (it != swapMap.end()) ? it->second : word;
    }
    return result;
}

std::string fillTemplate(const std::string& tmpl, const std::string& placeholder,
                          const std::string& value) {
    // regex_replace treats $ in the replacement as a group reference, so a
    // captured dollar amount like "$50" would come back mangled
    std::string escaped;
    for (char c : value) {
        if (c == '$') {
            escaped += "$$";
        } else {
            escaped += c;
        }
    }
    return std::regex_replace(tmpl, std::regex(placeholder), escaped);
}

}
