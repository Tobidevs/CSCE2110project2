// technology.cpp -- phones, apps, social media, screen time, devices.
// The group-defined eighth category; seventh in the dispatch order.

#include "technology.h"

#include <regex>

#include "text_utils.h"

bool technologyMatches(const std::string& input) {
    static const std::regex keywordPattern(
        R"(\b(phone|computer|laptop|tablet|internet|wifi|apps?|social media|instagram|tiktok|twitter|facebook|snapchat|youtube|reddit|screen time|online|scroll\w*|texting|email|games?|gaming|technology|notifications?)\b)",
        std::regex::icase);
    return std::regex_search(input, keywordPattern);
}

std::string technologyRespond(const std::string& input) {
    std::smatch match;

    static const std::regex tooMuchTime(
        R"(\bi\s+spend\s+too\s+much\s+time\s+on\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, tooMuchTime)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What do you think you'd do with that time if OBJECT "
                                         "weren't there?",
                                         "OBJECT", object);
    }

    static const std::regex addicted(
        R"(\bi(?:'m|\s+am)\s+addicted\s+to\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, addicted)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What keeps pulling you back to OBJECT?", "OBJECT",
                                         object);
    }

    static const std::regex cantPutDown(
        R"(\bi\s+can'?t\s+put\s+(?:my|the)\s+phone\s+down\b)", std::regex::icase);
    if (std::regex_search(input, cantPutDown)) {
        return "What are you usually looking for when you pick it up?";
    }

    static const std::regex screenTime(
        R"(\bmy\s+screen\s+time\s+(?:is|was)\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, screenTime)) {
        std::string amount = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("How do you feel when you see that it's AMOUNT?",
                                         "AMOUNT", amount);
    }

    static const std::regex scrolling(
        R"(\bi\s+(?:keep\s+scrolling|was\s+scrolling|'ve\s+been\s+scrolling|have\s+been\s+scrolling)\b)",
        std::regex::icase);
    if (std::regex_search(input, scrolling)) {
        return "What's that usually like for you afterward?";
    }

    static const std::regex quitApp(
        R"(\bi\s+(?:deleted|quit|got\s+off|deactivated)\s+(.+))", std::regex::icase);
    if (std::regex_search(input, match, quitApp)) {
        std::string object = text_utils::reflect(match[1].str());
        return text_utils::fillTemplate("What made you decide to get rid of OBJECT?", "OBJECT",
                                         object);
    }

    static const std::regex playGames(
        R"(\bi\s+play\s+(?:video\s+)?games\b|\bi(?:'m|\s+am)\s+gaming\b)", std::regex::icase);
    if (std::regex_search(input, playGames)) {
        return "What do you get out of playing?";
    }

    // technology owns the device nouns, so it has to answer "I have a new
    // phone" itself rather than leaving it to generic_verbs
    static const std::regex newDevice(
        R"(\bi\s+(?:have|got|bought)\s+(?:a\s+)?new\s+(phone|laptop|computer|tablet)\b)",
        std::regex::icase);
    if (std::regex_search(input, match, newDevice)) {
        std::string device = match[1].str();
        return text_utils::fillTemplate("How are you finding the new DEVICE?", "DEVICE", device);
    }

    static const std::regex deviceBroke(
        R"(\bmy\s+(phone|laptop|computer|tablet)\s+(?:broke|died|stopped\s+working|crashed)\b)",
        std::regex::icase);
    if (std::regex_search(input, match, deviceBroke)) {
        std::string device = match[1].str();
        return text_utils::fillTemplate("How much does having your DEVICE out of action affect "
                                         "your day?",
                                         "DEVICE", device);
    }

    return "Technology takes up a lot of room in daily life. Tell me more "
           "about that.";
}
