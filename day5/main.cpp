#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <inttypes.h>

bool intervalCompare(const std::pair<int64_t, int64_t>& a, const std::pair<int64_t, int64_t>& b) {
    return a.first < b.first;
}

int main () {

    std::vector<std::pair<int64_t, int64_t>> ranges;
    std::string line;

    while (std::getline(std::cin, line)) {
        if(line.empty()) break;
        size_t dash_pos = line.find('-');
        if (dash_pos != std::string::npos) {
            int64_t start = std::stoll(line.substr(0, dash_pos));
            int64_t end = std::stoll(line.substr(dash_pos + 1));
            ranges.emplace_back(start, end);
        }
    }
    std::sort(ranges.begin(), ranges.end(), intervalCompare);

    std::vector<std::pair<int64_t, int64_t>> mergedRanges;
    mergedRanges.reserve(ranges.size());

    for (int i = 0; i < ranges.size(); ++i) {
        int64_t start = ranges[i].first;
        int64_t end = ranges[i].second;

        if (!mergedRanges.empty() && mergedRanges.back().second >= start) {
            continue;
        }  

        for(int j = i + 1; j < ranges.size(); ++j) {
            if (ranges[j].first <= end) {
                end = std::max(end, ranges[j].second);
            } else {
                break;
            }
        }

        mergedRanges.emplace_back(start, end);
    }        

    int64_t total = 0;
    for(const auto& range : mergedRanges) {
        total += (range.second - range.first + 1);
    }

    std::cout << total << std::endl; 
}
