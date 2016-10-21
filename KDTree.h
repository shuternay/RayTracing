#ifndef RAYTRACING_KDTREE_H
#define RAYTRACING_KDTREE_H


#include "Vector.h"
#include "Primitives.h"

enum Plane {
    XY, XZ, YZ, NONE
};

class KDNode {
    const static int maxRecursionDepth = 15;
    const static int maxObjectsInList = 2;

    enum Plane plane;

    BoundingBox boundingBox;
    KDNode *left;
    KDNode *right;

    std::vector<Primitive *> nodePrimitives;

#ifdef COUNT_STATS
    static int _buildCalls, _findCalls;
#endif

    static void dividePrimitives(const Point &middlePoint, const Vector &planeNormal,
                                 const std::vector<Primitive *> &primitives,
                                 std::vector<Primitive *> &leftNodePrimitives,
                                 std::vector<Primitive *> &rightNodePrimitives);

public:
    KDNode(const std::vector<Primitive *> &primitives, int iter, const BoundingBox &nodeBox);

    ~KDNode() {
        // doesn't delete primitives
        delete left;
        delete right;
    }

    bool findRayIntersection(const Point &rayStart, const Vector &rayDirection, Intersection &nearestIntersection);

#ifdef COUNT_STATS
    static int _getBuildCalls() { return _buildCalls; }
    static int _getFindCalls() { return _findCalls; }
    static void _resetStats() {
        _buildCalls = 0;
        _findCalls = 0;
    }
#endif
};

class KDTree {
    KDNode *root;

public:
    KDTree() : root(nullptr) { }

    ~KDTree() {
        delete root;
    }

    void buildTree(const std::vector<Primitive *> &primitives);

    bool findRayIntersection(const Point &rayStart, const Vector &rayDirection, Intersection &nearestIntersection);
};

#endif //RAYTRACING_KDTREE_H
