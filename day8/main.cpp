#include <iostream>
#include <inttypes.h>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <unordered_map>
#include <queue>
#include <unordered_set>

#define NUM_CONNECTIONS 1000

struct point {
    int64_t x;
    int64_t y;
    int64_t z;

    point(int64_t x, int64_t y, int64_t z) : x(x), y(y), z(z) {}
    bool operator==(const point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

std::vector<point> points;

int64_t distance(const point& a, const point& b) {
    return (a.x - b.x) * (a.x - b.x) +
           (a.y - b.y) * (a.y - b.y) +
           (a.z - b.z) * (a.z - b.z);
}

bool compareDistances(const std::pair<int, int>& a, const std::pair<int, int>& b) {
    int64_t distA = distance(points[a.first], points[a.second]);
    int64_t distB = distance(points[b.first], points[b.second]);
    return distA > distB; // > since it is a maxheap
}

class UnionFind {
    std::vector<int> Parent;
    std::vector<int> Size;
public:
    UnionFind(int n) {   
        Parent.resize(n);
        for(int i = 0; i < n; ++i) {
            Parent[i] = i;
        }
        Size.resize(n, 1);
    }

    int find(int p) {
        int root = Parent[p];
      
        if (Parent[root] != root) {
            return Parent[p] = find(root);
        }
      
        return root;
    }

    int unionBySize(int i, int j) {
        int irep = find(i);
        int jrep = find(j);

        if (irep == jrep)
            return Size[irep];

        int isize = Size[irep];
        int jsize = Size[jrep];

        if (isize < jsize) {
            Parent[irep] = jrep;
            Size[jrep] += Size[irep];
        }
        else {
            Parent[jrep] = irep;
            Size[irep] += Size[jrep];
        }

        return isize + jsize;
    }
};

int main () {
    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        int firstComma = line.find(',');
        int secondComma = line.find(',', firstComma + 1);

        int64_t x = std::stoll(line.substr(0, firstComma));
        int64_t y = std::stoll(line.substr(firstComma + 1, secondComma - firstComma - 1));
        int64_t z = std::stoll(line.substr(secondComma + 1));
        points.emplace_back(x, y, z);
    }

    UnionFind uf(points.size());

    std::vector<std::pair<int, int>> minHeap;
    minHeap.reserve(points.size() * (points.size() - 1) / 2); // we only need upper triangle since distance is symmetric
    
    for(size_t i = 0; i < points.size(); ++i) {
        for(size_t j = i + 1; j < points.size(); ++j) {
            minHeap.emplace_back(i, j);
        }
    }

    std::make_heap(minHeap.begin(), minHeap.end(), compareDistances);
    while(!minHeap.empty()) {
        std::pop_heap(minHeap.begin(), minHeap.end(), compareDistances);
        auto [p1, p2] = minHeap.back();
        minHeap.pop_back();        

        int size = uf.unionBySize(p1, p2);
        if (size == points.size()) {
            std::cout << points[p1].x * points[p2].x << std::endl;
            break;
        }
    }

    return 0;
}