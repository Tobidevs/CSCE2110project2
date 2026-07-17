#include "generic_verbs.h"

#include <regex>

#include "text_utils.h"

namespace {

std::string fillTemplate(const std::string& tmpl, const std::string& placeholder,
                          const std::string& value) {
    return std::regex_replace(tmpl, std::regex(placeholder), value);
}

}

bool genericVerbsMatches(const std::string& input) {
    static const std::regex keywordPattern(
        R"(\b(have|has|had|purchas\w*|bought|buy\w*|travel\w*|know|knew|known|learn\w*)\b)",
        std::regex::icase);
    return std::regex_search(input, keywordPattern);
}

std::string genericVerbsRespond(const std::string& input) {
    std::smatch match;

    // Whole-line short answer (e.g. "I don't know" / "Don't know").
    static const std::regex bareDontKnow(
        R"(^\s*(?:i\s+)?(?:don't|dont|do not)\s+know\.?\s*$)", std::regex::icase);
    if (std::regex_match(input, bareDontKnow)) {
        return "That's alright -- take your time. Is there anything close to an answer?";
    }

    static const std::regex dontKnow(R"(\bI\s+(?:don't|do not)\s+know\b)", std::regex::icase);
    if (std::regex_search(input, dontKnow)) {
        return "Not knowing can be uncomfortable. What would help you find out?";
    }

    static const std::regex dontHave(
        R"(\bI\s+(?:don't|do not)\s+have\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, dontHave)) {
        std::string object = text_utils::reflect(match[1].str());
        return fillTemplate("What would it mean to you to have OBJECT?", "OBJECT", object);
    }

    static const std::regex have(R"(\bI\s+have\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, have)) {
        std::string object = text_utils::reflect(match[1].str());
        return fillTemplate("How long have you had OBJECT?", "OBJECT", object);
    }

    static const std::regex wantToPurchase(
        R"(\bI\s+want\s+to\s+(?:purchase|buy)\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, wantToPurchase)) {
        std::string object = text_utils::reflect(match[1].str());
        return fillTemplate("Why do you want to purchase OBJECT?", "OBJECT", object);
    }

    static const std::regex purchased(
        R"(\bI\s+(?:purchased|bought)\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, purchased)) {
        std::string object = text_utils::reflect(match[1].str());
        return fillTemplate("What made you decide to purchase OBJECT?", "OBJECT", object);
    }

    static const std::regex traveled(
        R"(\bI\s+(?:travel(?:ed|led|ling|ing)?|went)\s+to\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, traveled)) {
        std::string place = text_utils::reflect(match[1].str());
        return fillTemplate("What was your trip to PLACE like?", "PLACE", place);
    }

    static const std::regex learn(
        R"(\bI\s+(?:learned|am\s+learning|want\s+to\s+learn)\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, learn)) {
        std::string topic = text_utils::reflect(match[1].str());
        return fillTemplate("What draws you to learning about TOPIC?", "TOPIC", topic);
    }

    static const std::regex know(R"(\bI\s+know\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, know)) {
        std::string topic = text_utils::reflect(match[1].str());
        return fillTemplate("How do you know TOPIC?", "TOPIC", topic);
    }

    return "That's interesting -- tell me more about it.";
}
