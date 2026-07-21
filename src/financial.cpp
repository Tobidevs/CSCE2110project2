// financial.cpp -- money: affording things, debt, jobs, saving, spending.
// Third in the dispatch order.

#include "financial.h"

#include <regex>

#include "text_utils.h"

bool financialMatches(const std::string& input) {
    static const std::regex keywordPattern(
        // "spend" is deliberately not matched bare: "I spend too much time on
        // my phone" belongs to technology, and "I spend money" hits "money"
        R"(\b(money|debt|owe\w*|loan\w*|save|saving\w*|savings|invest\w*|bankrupt\w*|broke|afford\w*|rent|bills?|paycheck|salary|income|budget\w*|spent|spending|expensive|cost\w*|credit|mortgage|job|laid off|fired)\b)",
        std::regex::icase);
    return std::regex_search(input, keywordPattern);
}

std::string financialRespond(const std::string& input) {
    std::smatch match;

    static const std::regex bareBroke(
        R"(^\s*i(?:'m|\s+am)\s+broke\.?\s*$)", std::regex::icase);
    if (std::regex_match(input, bareBroke)) {
        return "Money being that tight sounds stressful. What's putting the "
               "most pressure on you right now?";
    }

    static const std::regex cantAfford(
        R"(\bi\s+can'?t\s+afford\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, cantAfford)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate(
            "What would change for you if you could afford OBJECT?", "OBJECT", object);
    }

    static const std::regex lostJob(
        R"(\bi\s+(?:lost\s+my\s+job|got\s+laid\s+off|was\s+laid\s+off|got\s+fired)\b)",
        std::regex::icase);
    if (std::regex_search(input, lostJob)) {
        return "Losing a job affects a lot more than income. How have you "
               "been handling it?";
    }

    static const std::regex inDebt(
        R"(\bi\s+(?:am|'m)\s+in\s+debt\b|\bi\s+have\s+(?:a\s+lot\s+of\s+)?debt\b)",
        std::regex::icase);
    if (std::regex_search(input, inDebt)) {
        return "Carrying debt can weigh on you. How long has that been "
               "hanging over you?";
    }

    static const std::regex owe(R"(\bi\s+owe\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, owe)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("How did you come to owe OBJECT?", "OBJECT", object);
    }

    // two alternatives, so the goal lands in whichever group matched
    static const std::regex saving(
        R"(\bi(?:'m|\s+am)\s+saving\s+(?:up\s+)?for\s+(.+)|\bi\s+want\s+to\s+save\s+(?:up\s+)?for\s+(.+))",
        std::regex::icase);
    if (std::regex_search(input, match, saving)) {
        std::string goal = text_utils::reflect(match[1].matched ? match[1].str() : match[2].str());
        return text_utils::fillTemplate("What makes GOAL worth saving for?", "GOAL", goal);
    }

    static const std::regex invest(
        R"(\bi\s+(?:invested|want\s+to\s+invest|am\s+investing)\s+in\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, invest)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What drew you to investing in OBJECT?", "OBJECT", object);
    }

    static const std::regex spent(R"(\bi\s+spent\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, spent)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate(
            "How did you feel after spending OBJECT?", "OBJECT", object);
    }

    static const std::regex worriedAbout(
        R"(\bi(?:'m|\s+am)\s+worried\s+about\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, worriedAbout)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What's the worst case you imagine with OBJECT?",
                                         "OBJECT", object);
    }

    return "Money can be a heavy thing to think about. Tell me more about "
           "what's going on.";
}
