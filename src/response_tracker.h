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
