// education.cpp -- school: classes, studying, exams, grades, majors.
// Fifth in the dispatch order.

#include "education.h"

#include <regex>

#include "text_utils.h"

bool educationMatches(const std::string& input) {
    static const std::regex keywordPattern(
        // "passed" excludes "passed away" so a death doesn't get answered as
        // if it were an exam result
        R"(\b(class|classes|course\w*|universit\w*|college|school|study\w*|studied|student|exam\w*|midterm|quiz|homework|assignment\w*|professor|teacher|lecture|degree|major\w*|semester|grades?|gpa|graduat\w*|failing|failed|passed(?!\s+away)|struggling|understand|dropped|tuition)\b)",
        std::regex::icase);
    return std::regex_search(input, keywordPattern);
}

std::string educationRespond(const std::string& input) {
    std::smatch match;

    static const std::regex bareFailing(
        R"(^\s*i(?:'m|\s+am)\s+failing\.?\s*$)", std::regex::icase);
    if (std::regex_match(input, bareFailing)) {
        return "That's a stressful place to be. Which part started slipping "
               "first?";
    }

    static const std::regex failed(R"(\bi\s+failed\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, failed)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What do you think went wrong with OBJECT?", "OBJECT",
                                         object);
    }

    static const std::regex passed(R"(\bi\s+passed\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, passed)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("How did it feel to pass OBJECT?", "OBJECT", object);
    }

    static const std::regex struggling(
        R"(\bi(?:'m|\s+am)\s+struggling\s+(?:with|in)\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, struggling)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What makes OBJECT harder than the rest?", "OBJECT",
                                         object);
    }

    static const std::regex dontUnderstand(
        R"(\bi\s+don'?t\s+understand\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, dontUnderstand)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("Where does OBJECT stop making sense to you?", "OBJECT",
                                         object);
    }

    // two alternatives, so the subject lands in whichever group matched
    static const std::regex majoring(
        R"(\bi(?:'m|\s+am)\s+majoring\s+in\s+(.+)|\bmy\s+major\s+is\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, majoring)) {
        std::string subject =
            text_utils::reflect(match[1].matched ? match[1].str() : match[2].str());
        return text_utils::fillTemplate("What led you to choose SUBJECT?", "SUBJECT", subject);
    }

    static const std::regex studying(
        R"(\bi(?:'m|\s+am)\s+studying\s+(?:for\s+)?(.+)|\bi\s+study\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, studying)) {
        std::string subject =
            text_utils::reflect(match[1].matched ? match[1].str() : match[2].str());
        return text_utils::fillTemplate("How is SUBJECT going so far?", "SUBJECT", subject);
    }

    static const std::regex haveExam(
        R"(\bi\s+have\s+(?:an?\s+|my\s+)?(?:exam|test|midterm|final|quiz)\b)", std::regex::icase);
    if (std::regex_search(input, haveExam)) {
        return "How prepared are you feeling for it?";
    }

    static const std::regex dropped(R"(\bi\s+dropped\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, dropped)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What made you decide to drop OBJECT?", "OBJECT", object);
    }

    return "School can take a lot out of you. Tell me more about how it's "
           "going.";
}
