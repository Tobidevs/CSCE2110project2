// response_tracker.h -- hash table of normalized input to how many times it
// has been said this session.

#ifndef RESPONSE_TRACKER_H
#define RESPONSE_TRACKER_H

#include <string>
#include <unordered_map>

class ResponseTracker {
public:
    bool isRepeat(const std::string& normalizedInput) const;
    void record(const std::string& normalizedInput);

private:
    std::unordered_map<std::string, int> history_;
};

#endif
