#include "Primitives.h"

Triangle::Triangle(const Point &A, const Point &B, const Point &C, const Vector &normal, const Material &material) :
        Primitive(material) {
    if ((B - A) * (C - A) % normal > 0) {
        this->A = A;
        this->B = B;
        this->C = C;
    }
    else {
        this->A = A;
        this->B = C;
        this->C = B;
    }
}

bool Triangle::intersectWithRay(Point rayStart, Vector rayDirection, Intersection &intersection) {

    Point rayPoint = rayStart + rayDirection;

    double V1 = mixedProduct(A - rayStart, B - rayStart, C - rayStart);
    double V2 = mixedProduct(A - rayPoint, B - rayPoint, C - rayPoint);

    if (Point::doubleEqual(0, V1 - V2) || Point::doubleLessOrEqual(V1 / (V1 - V2), 0)) {
        return false;
    }

    Point X = rayStart + rayDirection * (V1 / (V1 - V2));

    if (
            Point::doubleGreater(0, ((B - A) * (X - A)) % ((B - A) * (C - A))) ||
            Point::doubleGreater(0, ((C - B) * (X - B)) % ((C - B) * (A - B))) ||
            Point::doubleGreater(0, ((A - C) * (X - C)) % ((A - C) * (B - C)))) {
        return false;
    }

    if (V1 > 0) {
        intersection.color = Color(0x00, 0x00, 0x00);
        intersection.isFrontFace = false;
        intersection.surfaceNormal = ((B - A) * (C - A)).normalise(-1);
    }
    else {
        intersection.color = getMaterial().getColor();
        intersection.isFrontFace = true;
        intersection.surfaceNormal = ((B - A) * (C - A)).normalise(1);
    }
    intersection.point = X;
    intersection.distance = (X - rayStart).length();
    intersection.primitive = this;

    return true;
}

BoundingBox Triangle::getBoundingBox() const {
    Point minCorner = BoundingBox::uniteMinCorners(A, B);
    minCorner = BoundingBox::uniteMinCorners(C, minCorner);

    Point maxCorner = BoundingBox::uniteMaxCorners(A, B);
    maxCorner = BoundingBox::uniteMaxCorners(C, maxCorner);

    return BoundingBox(minCorner, maxCorner);
}


Polygon::Polygon(const std::vector<Point> points, const Vector &normal, const Material &material) : Primitive(
        material) {
    if ((points[1] - points[0]) * (points[2] - points[0]) % normal > 0) {
        this->points = points;
    }
    else {
        this->points = points;
        std::reverse(this->points.begin(), this->points.end());
    }
}


bool Polygon::intersectWithRay(Point rayStart, Vector rayDirection, Intersection &intersection) {
    Point A = points[0];
    Point B = points[1];
    Point C = points[2];
    Point rayPoint = rayStart + rayDirection;

    double V1 = mixedProduct(A - rayStart, B - rayStart, C - rayStart);
    double V2 = mixedProduct(A - rayPoint, B - rayPoint, C - rayPoint);

    if (Point::doubleEqual(0, V1 - V2) || Point::doubleLessOrEqual(V1 / (V1 - V2), 0)) {
        return false;
    }

    Point X = rayStart + rayDirection * (V1 / (V1 - V2));

    bool flag = true;
    for (size_t i = 0; i < points.size() && flag; ++i) {
        if (Point::doubleGreater(0, ((points[(i + 1) % points.size()] - points[i]) * (X - points[i])) %
                                    ((points[(i + 1) % points.size()] - points[i]) *
                                     (points[(i + 2) % points.size()] - points[i])))) {
            flag = false;
        }
    }
    if (!flag) {
        return false;
    }

    if (V1 > 0) {
        intersection.color = Color(0x00, 0x00, 0x00);
        intersection.isFrontFace = false;
        intersection.surfaceNormal = ((B - A) * (C - A)).normalise(-1);
    }
    else {
        intersection.color = getMaterial().getColor();
        intersection.isFrontFace = true;
        intersection.surfaceNormal = ((B - A) * (C - A)).normalise(1);
    }
    intersection.point = X;
    intersection.distance = (X - rayStart).length();
    intersection.primitive = this;

    return true;
}

BoundingBox Polygon::getBoundingBox() const {
    Point minCorner = points[0];
    Point maxCorner = points[0];

    for (size_t i = 1; i < points.size(); ++i) {
        minCorner = BoundingBox::uniteMinCorners(minCorner, points[i]);
        maxCorner = BoundingBox::uniteMaxCorners(maxCorner, points[i]);
    }

    return BoundingBox(minCorner, maxCorner);
}


bool Sphere::intersectWithRay(Point rayStart, Vector rayDirection, Intersection &intersection) {

    Point H = center.putHeight(rayStart, rayStart + rayDirection);

    if ((H - center).length() > radius) {
        return false;
    }

    double chordLength = 2 * sqrt(radius * radius - (H - center).squaredLength());
    intersection.distance = (H - rayStart) % rayDirection.normalise() - chordLength / 2;

    if (Point::doubleLessOrEqual(intersection.distance, 0)) {
        if (Point::doubleLessOrEqual(intersection.distance + chordLength, 0)) {
            return false;
        }
        else {
            intersection.distance += chordLength;
            intersection.point = rayStart + rayDirection.normalise(intersection.distance);
            intersection.surfaceNormal = (center - intersection.point).normalise();
            intersection.color = Color(0x00, 0x00, 0x00);
            intersection.isFrontFace = false;
        }
    }
    else {
        intersection.point = rayStart + rayDirection.normalise(intersection.distance);
        intersection.surfaceNormal = (intersection.point - center).normalise();
        intersection.color = getMaterial().getColor();
        intersection.isFrontFace = true;
    }
    intersection.primitive = this;

    return true;
}


BoundingBox Sphere::getBoundingBox() const {
    return BoundingBox(center - Vector(radius, radius, radius), center + Vector(radius, radius, radius));
}


void BoundingBox::boundPrimitives(const std::vector<Primitive *> primitives) {
    if (primitives.empty()) {
        minCorner = Point(0, 0, 0);
        maxCorner = Point(0, 0, 0);
        return;
    }

    for (size_t i = 0; i < primitives.size(); ++i) {
        BoundingBox currentBoundingBox = primitives[i]->getBoundingBox();
        if (i == 0) {
            minCorner = currentBoundingBox.minCorner;
            maxCorner = currentBoundingBox.maxCorner;
        }
        else {
            minCorner = uniteMinCorners(minCorner, currentBoundingBox.minCorner);
            maxCorner = uniteMaxCorners(maxCorner, currentBoundingBox.maxCorner);
        }
    }
}

bool BoundingBox::intersectsWithRay(const Point &rayStart, const Vector &rayDirection) const {
    // corners
    double dx = (maxCorner - minCorner).getX();
    double dy = (maxCorner - minCorner).getY();
    double dz = (maxCorner - minCorner).getZ();
    Point A0 = minCorner;
    Point B0 = minCorner + Vector(dx, 0, 0);
    Point C0 = B0 + Vector(0, dy, 0);
    Point D0 = A0 + Vector(0, dy, 0);
    Point A1 = A0 + Vector(0, 0, dz);
    Point B1 = B0 + Vector(0, 0, dz);
    Point C1 = C0 + Vector(0, 0, dz);
    Point D1 = D0 + Vector(0, 0, dz);

    return isRayIntersectsWithSide(rayStart, rayDirection, A0, B0, C0, D0) ||
           isRayIntersectsWithSide(rayStart, rayDirection, A1, B1, C1, D1) ||
           isRayIntersectsWithSide(rayStart, rayDirection, A0, A1, D1, D0) ||
           isRayIntersectsWithSide(rayStart, rayDirection, B0, B1, C1, C0) ||
           isRayIntersectsWithSide(rayStart, rayDirection, A0, A1, B1, B0) ||
           isRayIntersectsWithSide(rayStart, rayDirection, D0, D1, C1, C0);
}

bool BoundingBox::isRayIntersectsWithSide(const Point &rayStart, const Vector &rayDirection, const Point &A,
                                          const Point &B, const Point &C, const Point &D) const {
    Point rayPoint = rayStart + rayDirection;

    double V1 = mixedProduct(A - rayStart, B - rayStart, C - rayStart);
    double V2 = mixedProduct(A - rayPoint, B - rayPoint, C - rayPoint);

    if (Point::doubleEqual(0, V1 - V2) || V1 / (V1 - V2) < 0) {
        return false;
    }

    Point X = rayStart + rayDirection * (V1 / (V1 - V2));

    return Point::doubleLessOrEqual(minCorner.getX(), X.getX()) &&
           Point::doubleLessOrEqual(X.getX(), maxCorner.getX()) &&
           Point::doubleLessOrEqual(minCorner.getY(), X.getY()) &&
           Point::doubleLessOrEqual(X.getY(), maxCorner.getY()) &&
           Point::doubleLessOrEqual(minCorner.getZ(), X.getZ()) &&
           Point::doubleLessOrEqual(X.getZ(), maxCorner.getZ());
}












