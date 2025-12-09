#include <iostream>
#include <string>
#include <vector>
#include <inttypes.h>
#include <unordered_set>
#include <algorithm>

struct Point {
    int64_t x;
    int64_t y;
    bool operator== (const Point& other) const {
        return x == other.x && y == other.y;
    }
    Point(int64_t x, int64_t y) : x(x), y(y) {}
};

int64_t area(const Point& a, const Point& b) {
    return (std::abs(a.x - b.x) + 1) * (std::abs(a.y - b.y) + 1);
}
struct Rect {
    Point topLeft;
    Point bottomLeft;
    Point topRight;
    Point bottomRight;
    int64_t size;
    Rect(Point tl, Point bl, Point tr, Point br, int64_t s) : topLeft(tl), bottomLeft(bl), topRight(tr), bottomRight(br), size(s) {}
};

struct Edge {
    Point start;
    Point end;
    Edge(Point s, Point e) : start(s), end(e) {}
};

std::string pointToString(const Point& p) {
    return "(" + std::to_string(p.x) + "," + std::to_string(p.y) + ")";
}

std::string edgeToString(const Edge& e) {
    return pointToString(e.start) + " -> " + pointToString(e.end);
}

std::string rectToString(const Rect& r) {
    return pointToString(r.bottomLeft) + " to " + pointToString(r.topRight) + " size: " + std::to_string(r.size);
}

// Helper function to determine orientation of ordered triplet (p, q, r)
// Returns: 0 if collinear, positive if clockwise, negative if counterclockwise

// for future reference, this calculates the area of the parallelogram formed by pq and pr. 
int64_t crossProduct(const Point& p, const Point& q, const Point& r) {
    return (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - q.x);
}

// Check if point q lies on segment pr (assuming they're collinear)
bool onEdge(const Edge& edge, const Point& point) {
    const Point& p = edge.start;
    const Point& r = edge.end;
    return point.x <= std::max(p.x, r.x) && point.x >= std::min(p.x, r.x) &&
           point.y <= std::max(p.y, r.y) && point.y >= std::min(p.y, r.y);
}

bool segmentsProperlyIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2) {
    int64_t d1 = crossProduct(p2, q2, p1); 
    int64_t d2 = crossProduct(p2, q2, q1);
    int64_t d3 = crossProduct(p1, q1, p2);
    int64_t d4 = crossProduct(p1, q1, q2);
    
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0))
      &&((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
        return true;
    }
    
    return false;
}

bool edgeCutsRect(const Edge& e, const Rect& rect) {
    int64_t minX = rect.bottomLeft.x;
    int64_t maxX = rect.topRight.x;
    int64_t minY = rect.bottomLeft.y;
    int64_t maxY = rect.topRight.y;
    
    auto isStrictlyInside = [&](const Point& p) {
        return p.x > minX && p.x < maxX && p.y > minY && p.y < maxY;
    };
    
    if (isStrictlyInside(e.start) || isStrictlyInside(e.end)) {
        return true;
    }
    
    // Define the four edges of the rectangle
    Edge top(Point(minX, maxY), Point(maxX, maxY));
    Edge bottom(Point(minX, minY), Point(maxX, minY));
    Edge left(Point(minX, minY), Point(minX, maxY));
    Edge right(Point(maxX, minY), Point(maxX, maxY));
    
    std::vector<Edge> rectEdges = {top, bottom, left, right};
    
    // Check if the edge properly intersects any rectangle edge
    for (const Edge& rectEdge : rectEdges) {
        if (segmentsProperlyIntersect(e.start, e.end, rectEdge.start, rectEdge.end)) {
            return true;
        }
    }
    
    return false;
}

bool isPointInside(const Point& point, const std::vector<Edge>& edges) {
    int n = edges.size();    
    int intersections = 0;
    
    for (int i = 0; i < n; i++) {
        Point p1 = edges[i].start;
        Point p2 = edges[i].end;
        
        if (onEdge(edges[i], point)) {
            return true; // On boundary counts as inside
        }
        
        if ((p1.y <= point.y && p2.y <= point.y) || (p1.y > point.y && p2.y > point.y)) {
            continue;
        }

        if(p2.x > point.x) {
            intersections++;
        }
    }
    
    return (intersections % 2) == 1;
}

int main() {
    std::string line;
    std::vector<Point> points;
    Point root(INT64_MAX, INT64_MAX);
    Point prev = root;
    std::vector<Edge> edges; 

    while(std::getline(std::cin, line)) {
        if(line.empty()) {
            break;
        }

        int commaPos = line.find(',');
        int64_t x = std::stoll(line.substr(0, commaPos));
        int64_t y = std::stoll(line.substr(commaPos + 1));

        Point newPoint(x,y);
        
        if(prev.x == INT64_MAX) {
            root = newPoint;
        }
        else {
            edges.emplace_back(prev, newPoint);
        }
    
        prev = newPoint;
        points.emplace_back(x,y);
    } 
    edges.emplace_back(prev, root);

    // std::cout << "Last edge added between " << edgeToString(Edge{prev, root}) << std::endl;
    int n = points.size();
    std::vector<Rect> areas;
    areas.reserve(n * (n-1) / 2);
    for(int i = 0; i < n; i++) {
        for(int j = i+1; j < n; j++) {
            int64_t minX = std::min(points[i].x, points[j].x);
            int64_t maxX = std::max(points[i].x, points[j].x);
            int64_t minY = std::min(points[i].y, points[j].y);
            int64_t maxY = std::max(points[i].y, points[j].y);

            areas.emplace_back(Point{minX, minY}, Point{minX, maxY}, Point{maxX, minY}, Point{maxX, maxY}, (maxX - minX + 1) * (maxY - minY + 1));
        }     
    }  
    std::sort(areas.begin(), areas.end(), [](const Rect& a, const Rect& b) {
        return a.size > b.size; // descending order
    });

    // areas = std::vector<Rect>{Rect{Point(2,7), Point(9,5), 24}}; // for testing
    for(const Rect& rect : areas) {
        bool valid = true;

        for(const Edge& e : edges) {
            if(edgeCutsRect(e, rect)) {
                valid = false;
                break;
            }
        }
        if(!valid) {
            continue;
        }

        std::vector<Point> rectCorners = {rect.topLeft, rect.topRight, rect.bottomLeft, rect.bottomRight};
        for(const Point& corner : rectCorners) {
            if(!isPointInside(corner, edges)) {
                valid = false;
                break;
            }
        }
        if(!valid) {
            continue;
        }

        std::cout << rect.size << std::endl;
        break;
    }
    return 0;
}