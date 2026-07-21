// entertainment.cpp -- concerts, movies, music, reading, going out.
// Sixth in the dispatch order. Video games belong to technology instead.

#include "entertainment.h"

#include <regex>

#include "text_utils.h"

bool entertainmentMatches(const std::string& input) {
    // video games are left out on purpose: technology owns those
    static const std::regex keywordPattern(
        R"(\b(concert|movies?|film|show|tv|music|band|song|album|danc\w*|restaurant|bar|club|part(?:y|ies)|festival|theater|theatre|museum|book|reading|listen\w*|watch\w*|hobby|hobbies|going out)\b)",
        std::regex::icase);
    return std::regex_search(input, keywordPattern);
}

std::string entertainmentRespond(const std::string& input) {
    std::smatch match;

    static const std::regex favorite(
        R"(\bmy\s+favou?rite\s+(movie|song|band|show|book|restaurant|album)\s+is\s+(.+))",
        std::regex::icase);
    if (std::regex_search(input, match, favorite)) {
        std::string kind = match[1].str();
        std::string title = text_utils::reflect(match[2].str());
        std::string reply = text_utils::fillTemplate(
            "What is it about TITLE that makes it your favorite KIND?", "KIND", kind);
        return text_utils::fillTemplate(reply, "TITLE", title);
    }

    static const std::regex wentTo(
        R"(\bi\s+went\s+to\s+(?:an?\s+|the\s+)?(concert|movie|restaurant|bar|club|party|show|museum|festival)\b)",
        std::regex::icase);
    if (std::regex_search(input, match, wentTo)) {
        std::string place = match[1].str();
        return text_utils::fillTemplate("What was the PLACE like?", "PLACE", place);
    }

    static const std::regex wantToSee(
        R"(\bi\s+want\s+to\s+(?:see|watch)\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, wantToSee)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What appeals to you about OBJECT?", "OBJECT", object);
    }

    static const std::regex watched(R"(\bi\s+(?:watched|saw)\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, watched)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What did you think of OBJECT?", "OBJECT", object);
    }

    static const std::regex listening(
        R"(\bi\s+(?:listen\s+to|'ve\s+been\s+listening\s+to|have\s+been\s+listening\s+to)\s+(.+))",
        std::regex::icase);
    if (std::regex_search(input, match, listening)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What draws you to OBJECT?", "OBJECT", object);
    }

    static const std::regex reading(
        R"(\bi(?:'m|\s+am)\s+reading\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, reading)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What made you pick up OBJECT?", "OBJECT", object);
    }

    static const std::regex dancing(
        R"(\bi\s+(?:like|enjoy)\s+dancing\b|\bi\s+dance\b)", std::regex::icase);
    if (std::regex_search(input, dancing)) {
        return "What do you enjoy most about it?";
    }

    static const std::regex goingOut(
        R"(\bi(?:'m|\s+am)\s+going\s+(?:out\b|to\s+(?:an?\s+|the\s+)?(?:concert|movie|party|show|restaurant|bar)\b))",
        std::regex::icase);
    if (std::regex_search(input, goingOut)) {
        return "That sounds like something to look forward to. Who are you "
               "going with?";
    }

    return "It's good to have things you enjoy. Tell me more about it.";
}
