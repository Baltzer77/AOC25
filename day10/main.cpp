#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <inttypes.h>
#include <algorithm>

struct vector_hash {
    std::size_t operator()(const std::vector<int>& v) const {
        std::size_t seed = v.size();
        for (auto& i : v) {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};


int64_t countButtonClicks(int depth, std::vector<int> current, const std::vector<std::vector<int>>& buttons, std::unordered_map<std::vector<int>, int, vector_hash>& depthMap, std::unordered_map<std::vector<int>, int64_t, vector_hash>& memo) {
    if (std::all_of(current.begin(), current.end(), [](int v) { return v == 0; })) {
        // std::cout << "Reached target state!" << std::endl;
        // for(const auto& step : path) {
        //     std::cout << "Button pressed: " << step << std::endl;
        // }
        // std::cout << std::endl; 
        std::cout << "Depth: " << depth << std::endl;

        return 0;
    }
    if (memo.find(current) != memo.end()) {
        return memo[current];
    }

    int64_t minClicks = INT64_MAX;
    for (int i = 0; i < buttons.size(); ++i) {
        std::vector<int> next = current;
        bool valid = true;
        for (int index : buttons[i]) {
            if (--next[index] < 0) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            continue;
        }

        if (depthMap.find(next) != depthMap.end() && depthMap[next] <= depth) {
            continue;
        }
        depthMap[next] = depth;
        
        int64_t clicks = countButtonClicks(depth + 1, next, buttons, depthMap, memo) + 1;

        if(clicks < minClicks && clicks > 0) {
            minClicks = clicks;
        }
    }
    if(minClicks == INT64_MAX) {
        return -1;
    }
    memo[current] = minClicks;
    return minClicks;
}

int main() {
    int64_t sum = 0;
    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }        
        
        std::vector<std::vector<int>> buttons;
        int squarePos = line.find(']');
        size_t pos = line.find('(', squarePos);
        
        while (pos != std::string::npos) {
            size_t endPos = line.find(')', pos);
            std::string buttonStr = line.substr(pos + 1, endPos - pos - 1);
            std::vector<int> button;
            size_t commaPos = 0;
            while (commaPos != std::string::npos) {
                size_t nextComma = buttonStr.find(',', commaPos);
                int index = std::stoi(buttonStr.substr(commaPos, nextComma - commaPos));
                button.push_back(index);
                if (nextComma == std::string::npos) {
                    break;
                }
                commaPos = nextComma + 1;
            }
            buttons.push_back(button);
            pos = line.find('(', endPos);
        }

        std::vector<int> target;
        pos = line.find('{', squarePos);
        size_t endPos = line.find('}', pos);

        while(pos != std::string::npos && pos < endPos) {
            size_t nextComma = line.find(',', pos + 1);
            nextComma = std::min(nextComma, endPos);

            int count = std::stoi(line.substr(pos + 1, nextComma - pos - 1));
            target.push_back(count);
            pos = nextComma;
        }
        
        std::vector<int> current(target.size(), 0);
        std::unordered_map<std::vector<int>, int64_t, vector_hash> memo;
        std::unordered_map<std::vector<int>, int, vector_hash> depthMap;
        
        std::cout << "Target state:";
        for (const auto& t : target) {
            std::cout << " " << t;
        }
        std::cout << std::endl;
        
        int64_t clicks = countButtonClicks(0, target, buttons, depthMap, memo);
        
        //int64_t clicks = memo[current];
        std::cout << "Clicks needed: " << clicks << std::endl;
        sum += clicks;
    }

    std::cout << sum << std::endl;
    return 0;

}