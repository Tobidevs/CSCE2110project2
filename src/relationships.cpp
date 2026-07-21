#include "relationships.h"

#include <regex>

#include "text_utils.h"

bool relationshipsMatches(const std::string& input) {
    static const std::regex keywordPattern(
        R"(\b(love\w*|spouse|wife|husband|girlfriend|boyfriend|partner|marr\w*|divorc\w*|break\s*up|broke\s+up|dating)\b)",
        std::regex::icase);
    return std::regex_search(input, keywordPattern);
}

std::string relationshipsRespond(const std::string& input) {
    std::smatch match;

    // checked before the plain "love" pattern below
    static const std::regex dontLove(
        R"(\bi\s+(?:don't|do not)\s+love\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, dontLove)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate(
            "What's changed between you and OBJECT?", "OBJECT", object);
    }

    static const std::regex love(R"(\bi\s+love\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, love)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate(
            "What is it about OBJECT that you love?", "OBJECT", object);
    }

    static const std::regex myRelation(
        R"(\bmy\s+(girlfriend|boyfriend|spouse|wife|husband|partner)\s+(.+))",
        std::regex::icase);
    if (std::regex_search(input, match, myRelation)) {
        std::string relation = match[1].str();
        std::string predicate = text_utils::reflect(match[2].str());
        std::string reply = text_utils::fillTemplate(
            "Tell me more about why your RELATION PREDICATE.", "RELATION", relation);
        return text_utils::fillTemplate(reply, "PREDICATE", predicate);
    }

    static const std::regex missObject(R"(\bi\s+miss\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, missObject)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What do you miss most about OBJECT?", "OBJECT", object);
    }

    // capture group is optional: "we broke up" has no named object
    static const std::regex brokeUp(
        R"(\bwe\s+broke\s+up\b|\bi\s+broke\s+up\s+with\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, brokeUp)) {
        if (match[1].matched) {
            std::string object = text_utils::reflect(match[1].str());
            return text_utils::fillTemplate(
                "Breaking up with OBJECT sounds hard. How are you feeling about it?",
                "OBJECT", object);
        }
        return "Breakups are hard. How are you feeling about it?";
    }

    static const std::regex bareInLove(
        R"(^\s*i(?:'m|\s+am)\s+in\s+love\.?\s*$)", std::regex::icase);
    if (std::regex_match(input, bareInLove)) {
        return "That sounds like a big feeling. Who are you in love with?";
    }

    static const std::regex gettingMarried(
        R"(\bi(?:'m|\s+am)\s+getting\s+married\b|\bi\s+got\s+married\b)", std::regex::icase);
    if (std::regex_search(input, gettingMarried)) {
        return "How are you feeling about getting married?";
    }

    return "Tell me more about that relationship.";
}
