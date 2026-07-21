// conversation.cpp -- the input loop and the only file that sequences things:
// normalize, check for "bye", check for a repeat, then walk the category
// registry until one matches, falling back if none does.

#include "conversation.h"

#include <iostream>
#include <string>
#include <vector>

#include "category.h"
#include "category_registry.h"
#include "response_tracker.h"
#include "text_utils.h"

namespace conversation {

namespace {

const char* kExitCommand = "bye";
const char* kGreeting = "Hello. I'm here to listen. What's on your mind today?";
const char* kFarewell = "Goodbye. Take care of yourself.";
const char* kRepeatPrompt =
    "You've mentioned that before -- could you tell me something different?";
const char* kFallbackResponse = "Tell me more about that.";

}

int run() {
    std::cout << kGreeting << std::endl;

    ResponseTracker tracker;
    std::vector<Category> categories = buildCategoryRegistry();

    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line)) {
        std::string normalized = text_utils::normalize(line);

        if (normalized == kExitCommand) {
            std::cout << kFarewell << std::endl;
            break;
        }

        if (normalized.empty()) {
            std::cout << kFallbackResponse << std::endl;
            continue;
        }

        if (tracker.isRepeat(normalized)) {
            std::cout << kRepeatPrompt << std::endl;
            tracker.record(normalized);
            continue;
        }

        bool matched = false;
        for (const Category& category : categories) {
            if (category.matches(line)) {
                std::cout << category.respond(line) << std::endl;
                matched = true;
                break;
            }
        }

        if (!matched) {
            std::cout << kFallbackResponse << std::endl;
        }

        tracker.record(normalized);
    }

    return 0;
}

}
