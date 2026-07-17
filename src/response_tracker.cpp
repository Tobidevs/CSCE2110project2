#include "response_tracker.h"

bool ResponseTracker::isRepeat(const std::string& normalizedInput) const {
    auto it = history_.find(normalizedInput);
    return it != history_.end() && it->second > 0;
}

void ResponseTracker::record(const std::string& normalizedInput) {
    ++history_[normalizedInput];
}
