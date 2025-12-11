#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct MemoNode {
    bool visitedDac;
    bool visitedFft;
    int64_t count;
    int64_t firstPotentialCount;
    int64_t secondPotentialCount;
};

MemoNode countPaths(std::string current, const std::unordered_map<std::string, std::vector<std::string>>& graph, std::unordered_map<std::string, MemoNode>& memo = *(new std::unordered_map<std::string, MemoNode>())) {
    if(current == "out") {

        return MemoNode{false, false, 0, 0, 1};
    }

    if(memo.find(current) != memo.end()) {
        return memo[current];
    }

    if(graph.find(current) == graph.end()) {
        return MemoNode{false, false, 0, 0, 0}; // this node has no neighbors
    }
    std::vector<std::string> neighbors = graph.at(current);


    MemoNode node{false, false, 0, 0, 0};
    for(const std::string& neighbor : neighbors) {
        MemoNode newNode = countPaths(neighbor, graph, memo);
        node.count += newNode.count;
        node.firstPotentialCount += newNode.firstPotentialCount;
        node.secondPotentialCount += newNode.secondPotentialCount;

        bool isFirst = !(newNode.visitedDac || newNode.visitedFft) && (current == "dac" || current == "fft");
        if(isFirst) {
            node.firstPotentialCount += newNode.secondPotentialCount;
        }
        else {
            bool isSecond = (newNode.visitedDac != newNode.visitedFft) && (current == "dac" || current == "fft");
            if(isSecond) {
                node.count += newNode.firstPotentialCount;
            }
        }

        node.visitedDac = node.visitedDac || newNode.visitedDac || (current == "dac");
        node.visitedFft = node.visitedFft || newNode.visitedFft || (current == "fft");
    }

    return memo[current] = node;    
}

int main () {
    std::string line;

    std::unordered_map<std::string, std::vector<std::string>> graph;

    while (std::getline(std::cin, line)) {
        if(line.empty()) {
            break;
        }

        size_t colonPos = line.find(':');
        std::string key = line.substr(0, colonPos);
        std::vector<std::string> values;
        size_t pos = colonPos + 2; 
        while (pos != std::string::npos) {
            size_t spacePos = line.find(' ', pos);
            std::string value;
            if (spacePos != std::string::npos) {
                value = line.substr(pos, spacePos - pos);
                pos = spacePos + 1;
            }
            else {
                value = line.substr(pos);
                pos = std::string::npos;
            }
            values.push_back(value);
        }
        graph[key] = values;
    }
    
    
    int64_t result = countPaths("svr", graph).count;
    std::cout << result << std::endl;
    return 0;
}