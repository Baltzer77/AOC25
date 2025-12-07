#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <inttypes.h>

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2 + 1;  
    }
};

std::unordered_map<std::pair<int, int>, int64_t, pair_hash> memo;

int64_t dfs(std::vector<std::string>& graph, int colIndex, int rowIndex) {
    if (rowIndex < 0 || rowIndex >= graph.size() ||
       colIndex < 0 || colIndex >= graph[rowIndex].size()) {
        return 1;
    }

    if (memo[{colIndex, rowIndex}]) {
        return memo[{colIndex, rowIndex}];
    }

    if (graph[rowIndex][colIndex] == '^') {
        int64_t val = dfs(graph, colIndex - 1, rowIndex) + dfs(graph, colIndex + 1, rowIndex);
        memo[{colIndex, rowIndex}] = val;
        return val;
    }

    int64_t val = dfs(graph, colIndex, rowIndex + 1);
    memo[{colIndex, rowIndex}] = val;

    return val;
}

int main () {
    std::string line;
    std::vector<std::string> graph;
    while (std::getline(std::cin, line)) {
        if(line.empty()) {
            break;
        }
        graph.push_back(line);
    }

    int startCol = graph[0].find('S');
    std::cout << dfs(graph, startCol, 0) << std::endl;
    return 0; 
}