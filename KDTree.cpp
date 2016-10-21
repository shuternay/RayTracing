#include "KDTree.h"

void KDTree::buildTree(const std::vector<Primitive *> &primitives) {
    root = new KDNode(primitives, 0, BoundingBox(
            Point(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                  -std::numeric_limits<double>::infinity()),
            Point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
                  std::numeric_limits<double>::infinity())));
}

bool KDTree::findRayIntersection(const Point &rayStart, const Vector &rayDirection, Intersection &nearestIntersection) {
    return root->findRayIntersection(rayStart, rayDirection, nearestIntersection);
}

#ifdef COUNT_STATS
int KDNode::_buildCalls = 0;
int KDNode::_findCalls = 0;
#endif

KDNode::KDNode(const std::vector<Primitive *> &primitives, int iter, const BoundingBox &nodeBox) {
#ifdef COUNT_STATS
    ++_buildCalls;
#endif
    boundingBox.boundPrimitives(primitives);
    // intersection of bounding boxes
    boundingBox.minCorner = BoundingBox::uniteMaxCorners(boundingBox.minCorner, nodeBox.minCorner);
    boundingBox.maxCorner = BoundingBox::uniteMinCorners(boundingBox.maxCorner, nodeBox.maxCorner);

    if (iter > maxRecursionDepth || primitives.size() < maxObjectsInList) {
        plane = NONE;
        left = nullptr;
        right = nullptr;
        nodePrimitives = primitives;
        return;
    }

    plane = static_cast<Plane>(iter % 3);

    Point middlePoint = (boundingBox.minCorner + boundingBox.maxCorner) * 0.5;
    BoundingBox leftNodeBoundingBox = boundingBox, rightNodeBoundingBox = boundingBox;

    Vector planeNormal;
    if (plane == XY) {
        planeNormal = Vector(0, 0, -1);
        leftNodeBoundingBox.maxCorner.setZ(middlePoint.getZ());
        rightNodeBoundingBox.minCorner.setZ(middlePoint.getZ());
    }
    else if (plane == XZ) {
        planeNormal = Vector(0, -1, 0);
        leftNodeBoundingBox.maxCorner.setY(middlePoint.getY());
        rightNodeBoundingBox.minCorner.setY(middlePoint.getY());
    }
    else if (plane == YZ) {
        planeNormal = Vector(-1, 0, 0);
        leftNodeBoundingBox.maxCorner.setX(middlePoint.getX());
        rightNodeBoundingBox.minCorner.setX(middlePoint.getX());
    }

    std::vector<Primitive *> leftNodePrimitives, rightNodePrimitives;
    dividePrimitives(middlePoint, planeNormal, primitives, leftNodePrimitives, rightNodePrimitives);

    left = new KDNode(leftNodePrimitives, iter + 1, leftNodeBoundingBox);
    right = new KDNode(rightNodePrimitives, iter + 1, rightNodeBoundingBox);
}

void KDNode::dividePrimitives(const Point &middlePoint, const Vector &planeNormal,
                              const std::vector<Primitive *> &primitives,
                              std::vector<Primitive *> &leftNodePrimitives,
                              std::vector<Primitive *> &rightNodePrimitives) {
    for (Primitive *primitive : primitives) {
        BoundingBox primitiveBoundingBox = primitive->getBoundingBox();
        if (Point::doubleGreaterOrEqual((primitiveBoundingBox.minCorner - middlePoint) % planeNormal, 0)) {
            leftNodePrimitives.push_back(primitive);
        }
        if (Point::doubleLessOrEqual((primitiveBoundingBox.maxCorner - middlePoint) % planeNormal, 0)) {
            rightNodePrimitives.push_back(primitive);
        }
    }
}

bool KDNode::findRayIntersection(const Point &rayStart, const Vector &rayDirection, Intersection &nearestIntersection) {
#ifdef COUNT_STATS
    ++_findCalls;
#endif
    if (!boundingBox.intersectsWithRay(rayStart, rayDirection)) { // FIXME
        return false;
    }

    nearestIntersection.distance = std::numeric_limits<double>::infinity();

    if (plane == NONE) {
        for (Primitive *primitive : nodePrimitives) {
            Intersection intersection;

            if (primitive->intersectWithRay(rayStart, rayDirection, intersection) &&
                intersection < nearestIntersection) {
                nearestIntersection = intersection;
            }
        }
        return nearestIntersection.distance < std::numeric_limits<double>::infinity();
    }
    else {
        if ((plane == XY && rayDirection.getZ() > 0) ||
            (plane == XZ && rayDirection.getY() > 0) ||
            (plane == YZ && rayDirection.getX() > 0)) {
            if (!left->findRayIntersection(rayStart, rayDirection, nearestIntersection)) {
                return right->findRayIntersection(rayStart, rayDirection, nearestIntersection);
            }
            else {
                Intersection intersection = nearestIntersection;
                right->findRayIntersection(rayStart, rayDirection, intersection);
                if (intersection.distance < nearestIntersection.distance)
                    nearestIntersection = intersection;
                return nearestIntersection.distance < std::numeric_limits<double>::infinity();
            }
        }
        else {
            if (!right->findRayIntersection(rayStart, rayDirection, nearestIntersection)) {
                return left->findRayIntersection(rayStart, rayDirection, nearestIntersection);
            }
            else {
                Intersection intersection = nearestIntersection;
                left->findRayIntersection(rayStart, rayDirection, intersection);
                if (intersection.distance < nearestIntersection.distance)
                    nearestIntersection = intersection;
                return nearestIntersection.distance < std::numeric_limits<double>::infinity();
            }
        }
    }
}





