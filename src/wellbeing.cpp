#include "wellbeing.h"

#include <regex>

#include "text_utils.h"

bool wellbeingMatches(const std::string& input) {
    static const std::regex keywordPattern(
        R"(\b(exercis\w*|workout|work out|working out|gym|jog\w*|yoga|sleep\w*|slept|insomnia|tired|exhausted|nap|vitamins?|diet|nutrition|healthy|health|stress\w*|relax\w*|meditat\w*|doctor|sick)\b)",
        std::regex::icase);
    return std::regex_search(input, keywordPattern);
}

std::string wellbeingRespond(const std::string& input) {
    std::smatch match;

    static const std::regex bareTired(
        R"(^\s*i(?:'m|\s+am)\s+(?:so\s+|really\s+)?(?:tired|exhausted)\.?\s*$)",
        std::regex::icase);
    if (std::regex_match(input, bareTired)) {
        return "Being that worn out takes a toll. What's been draining you?";
    }

    static const std::regex cantSleep(
        R"(\bi\s+can'?t\s+sleep\b|\bi\s+have\s+trouble\s+sleeping\b|\bi(?:'m|\s+am)\s+not\s+sleeping\b)",
        std::regex::icase);
    if (std::regex_search(input, cantSleep)) {
        return "Sleep trouble makes everything else harder. What's usually "
               "on your mind when you're lying awake?";
    }

    static const std::regex sleepHours(
        R"(\bi\s+(?:only\s+)?(?:sleep|get|slept)\s+(?:about\s+|around\s+)?(\d+)\s+hours?)",
        std::regex::icase);
    if (std::regex_search(input, match, sleepHours)) {
        std::string hours = match[1].str();
        return text_utils::fillTemplate("Does HOURS hours feel like enough for you?", "HOURS",
                                         hours);
    }

    static const std::regex stressed(
        R"(\bi(?:'m|\s+am)\s+stressed\s+(?:out\s+)?about\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, stressed)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What part of OBJECT weighs on you most?", "OBJECT",
                                         object);
    }

    static const std::regex wantToExercise(
        R"(\bi\s+(?:want|need|should|have)\s+to\s+(?:exercise|work\s+out|go\s+to\s+the\s+gym|relax)\b)",
        std::regex::icase);
    if (std::regex_search(input, wantToExercise)) {
        return "What's been getting in the way of that?";
    }

    static const std::regex exerciseHabit(
        R"(\bi\s+(?:exercise|work\s+out|go\s+to\s+the\s+gym|jog|run|do\s+yoga|meditate)\b)",
        std::regex::icase);
    if (std::regex_search(input, exerciseHabit)) {
        return "How do you feel afterward when you do?";
    }

    static const std::regex takeSomething(
        R"(\bi\s+(?:take|started\s+taking)\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, takeSomething)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What made you start taking OBJECT?", "OBJECT", object);
    }

    static const std::regex feelSick(
        R"(\bi(?:'m|\s+am)\s+sick\b|\bi\s+feel\s+sick\b|\bi\s+saw\s+(?:the|a)\s+doctor\b)",
        std::regex::icase);
    if (std::regex_search(input, feelSick)) {
        return "How long have you been feeling that way?";
    }

    return "Taking care of yourself matters. Tell me more about how you've "
           "been feeling.";
}
