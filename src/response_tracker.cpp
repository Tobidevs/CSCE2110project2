// response_tracker.cpp -- counts how often each normalized line has been said
// so the program can spot a repeated answer and ask for a different one.

#include "response_tracker.h"

bool ResponseTracker::isRepeat(const std::string& normalizedInput) const {
    auto it = history_.find(normalizedInput);
    return it != history_.end() && it->second > 0;
}

void ResponseTracker::record(const std::string& normalizedInput) {
    ++history_[normalizedInput];
}
