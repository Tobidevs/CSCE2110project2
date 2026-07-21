#include "concern.h"

#include <regex>

#include "text_utils.h"

namespace {

const char* kCrisisResponse =
    "That sounds like a lot of pain to carry. If you're thinking about "
    "hurting yourself, please reach out to the 988 Suicide & Crisis "
    "Lifeline (call or text 988) -- you don't have to go through this "
    "alone. What's making things feel this way?";

}

bool concernMatches(const std::string& input) {
    static const std::regex keywordPattern(
        R"(\b(suicide\w*|kill\w*|harm|hurt|hopeless|worthless|hate\w*|end my life|end it all|want to die|no reason to live|can't go on|cant go on)\b)",
        std::regex::icase);
    return std::regex_search(input, keywordPattern);
}

std::string concernRespond(const std::string& input) {
    std::smatch match;

    // whole-line self-harm/suicide statement
    static const std::regex selfHarmBare(
        R"(^\s*(?:i\s+)?(?:want\s+to\s+die|want\s+to\s+kill\s+myself|am\s+going\s+to\s+kill\s+myself|can'?t\s+go\s+on|cant\s+go\s+on|have\s+no\s+reason\s+to\s+live)\.?\s*$)",
        std::regex::icase);
    if (std::regex_match(input, selfHarmBare)) {
        return kCrisisResponse;
    }

    // same, mid-sentence
    static const std::regex selfHarmSearch(
        R"(\b(suicide\w*|kill\s+myself|end\s+my\s+life|end\s+it\s+all|harm\s+myself|hurt\s+myself|self[- ]harm|want\s+to\s+die|no\s+reason\s+to\s+live)\b)",
        std::regex::icase);
    if (std::regex_search(input, selfHarmSearch)) {
        return kCrisisResponse;
    }

    static const std::regex hateMyself(R"(\bi\s+hate\s+myself\b)", std::regex::icase);
    if (std::regex_search(input, hateMyself)) {
        return "It sounds like you're being really hard on yourself. What's "
               "making you feel that way about yourself?";
    }

    static const std::regex feelLow(R"(\bi\s+feel\s+(hopeless|worthless)\b)", std::regex::icase);
    if (std::regex_search(input, match, feelLow)) {
        std::string feeling = match[1].str();
        return text_utils::fillTemplate(
            "Feeling FEELING is heavy to carry. What's been contributing to "
            "that feeling?",
            "FEELING", feeling);
    }

    // hate directed outward, not at self
    static const std::regex hateObject(R"(\bi\s+hate\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, hateObject)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate(
            "What is it about OBJECT that troubles you so much?", "OBJECT", object);
    }

    static const std::regex killObject(R"(\bi\s+want\s+to\s+kill\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, killObject)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate(
            "That's a strong feeling to have about OBJECT. What's been "
            "happening between you?",
            "OBJECT", object);
    }

    return "That sounds difficult to sit with. Can you tell me more about "
           "what's going on?";
}
