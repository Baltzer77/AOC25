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

struct point_hash {
    std::size_t operator()(const point& p) const noexcept {
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);
        std::size_t h3 = std::hash<int>{}(p.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

int64_t distance(const point& a, const point& b) {
    return std::pow(a.x - b.x, 2) +
           std::pow(a.y - b.y, 2) +
           std::pow(a.z - b.z, 2);
}

bool compareDistances(const std::pair<point*, point*>& a, const std::pair<point*, point*>& b) {
    int64_t distA = distance(*a.first, *a.second);
    int64_t distB = distance(*b.first, *b.second);
    return distA > distB; // > since it is a maxheap
}

class UnionFind {
    std::unordered_map<point, point*, point_hash> Parent;
    std::unordered_map<point, int, point_hash> Size;
public:
    UnionFind(int n) {   
        Parent.reserve(n);
        Size.reserve(n);
    }

    // WTFFFF
    std::vector<int> getKLargestSizes(int k) {
        std::unordered_set<point, point_hash> uniqueParents;
        std::vector<int> filteredSizes;
        for (const auto& p : Parent) {
            point* root = find(p.second);
            if (uniqueParents.find(*root) == uniqueParents.end()) {
                uniqueParents.insert(*root);
                filteredSizes.push_back(Size[*root]);

                std::cout << "Component rooted at (" << root->x << "," << root->y << "," << root->z << ") has size " << Size[*root] << std::endl;
            }
        }
        std::sort(filteredSizes.begin(), filteredSizes.end(), std::greater<int>());
        if (filteredSizes.size() > k) {
            filteredSizes.resize(k);
        }
        return filteredSizes;
    }

    point* find(point* p) {

        if (Parent.find(*p) == Parent.end()) {
            Parent[*p] = p;
            Size[*p] = 1;
            return Parent[*p];
        }

        point* root = Parent[*p];
      
        if (Parent[*root] != root) {
            return Parent[*p] = find(root);
        }
      
        return root;
    }

    int unionBySize(point* i, point* j) {
        point* irep = find(i);
        point* jrep = find(j);

        if (irep == jrep)
            return Size[*irep];

        int isize = Size[*irep];
        int jsize = Size[*jrep];

        if (isize < jsize) {
            Parent[*irep] = jrep;
            Size[*jrep] += Size[*irep];
        }
        else {
            Parent[*jrep] = irep;
            Size[*irep] += Size[*jrep];
        }

        return isize + jsize;
    }
};


int main () {
    std::string line;
    std::vector<point> points;
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

    std::priority_queue<std::pair<point*, point*>, std::vector<std::pair<point*, point*>>, decltype(&compareDistances)> minHeap(compareDistances);


    for(size_t i = 0; i < points.size(); ++i) {
        for(size_t j = i + 1; j < points.size(); ++j) {
            minHeap.emplace(&points[i], &points[j]);
        }
    }

    while(!minHeap.empty()) {
        auto [p1, p2] = minHeap.top();
        minHeap.pop();
        int size = uf.unionBySize(p1, p2);

        if (size == points.size()) {
            std::cout << p1->x * p2->x << std::endl;
            break;
        }
    }

    // std::vector<int> largestSizes = uf.getKLargestSizes(3);

    // int64_t result = 1;
    // for(int size : largestSizes) {
    //     std::cout << "Size: " << size << std::endl;
    //     result *= size;
    // }
    // std::cout << result << std::endl;

    return 0;
}