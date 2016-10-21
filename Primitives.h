#ifndef RAYTRACING_PRIMITIVES_H
#define RAYTRACING_PRIMITIVES_H

#include <vector>
#include "Vector.h"
#include "Material.h"

class Primitive;

class BoundingBox {
    bool isRayIntersectsWithSide(const Point &rayStart, const Vector &rayDirection, const Point &A,
                                 const Point &B, const Point &C, const Point &D) const;

public:
    Point minCorner;
    Point maxCorner;

    BoundingBox() { }

    BoundingBox(const Point &minCorner, const Point &maxCorner) : minCorner(minCorner), maxCorner(maxCorner) { }

    static Point uniteMinCorners(const Point &A, const Point &B) {
        return Point(std::min(A.getX(), B.getX()),
                     std::min(A.getY(), B.getY()),
                     std::min(A.getZ(), B.getZ()));
    }

    static Point uniteMaxCorners(const Point &A, const Point &B) {
        return Point(std::max(A.getX(), B.getX()),
                     std::max(A.getY(), B.getY()),
                     std::max(A.getZ(), B.getZ()));
    }

    void boundPrimitives(const std::vector<Primitive *> primitives);

    bool intersectsWithRay(const Point &rayStart, const Vector &rayDirection) const;
};

struct Intersection {
    Intersection() = default;

    Point point;
    double distance;
    Vector surfaceNormal;
    Color color;
    Primitive *primitive;
    bool isFrontFace;

    bool operator<(const Intersection &intersection) const {
        return distance < intersection.distance;
    }
};

class Primitive {
    Material material;

public:
    Primitive(const Material &material) : material(material) { }

    virtual bool intersectWithRay(Point rayStart, Vector rayDirection, Intersection &intersection) = 0;

    const Material &getMaterial() const { return material; }

    virtual BoundingBox getBoundingBox() const = 0;
};

class Triangle : public Primitive {
    Point A, B, C;

public:
    Triangle(const Point &A, const Point &B, const Point &C, const Material &material) :
            A(A), B(B), C(C), Primitive(material) { }

    Triangle(const Point &A, const Point &B, const Point &C, const Vector &normal, const Material &material);

    virtual bool intersectWithRay(Point rayStart, Vector rayDirection, Intersection &intersection);

    BoundingBox getBoundingBox() const;
};

class Polygon : public Primitive {
    std::vector<Point> points;

public:
    Polygon(const std::vector<Point> points, const Material &material) :
            points(points), Primitive(material) { }

    Polygon(const std::vector<Point> points, const Vector &normal, const Material &material);

    virtual bool intersectWithRay(Point rayStart, Vector rayDirection, Intersection &intersection);

    BoundingBox getBoundingBox() const;
};

class Sphere : public Primitive {
    Point center;
    double radius;

public:
    Sphere(const Point &center, double radius, const Material &material) :
            center(center), radius(radius), Primitive(material) { }

    virtual bool intersectWithRay(Point rayStart, Vector rayDirection, Intersection &intersection);

    BoundingBox getBoundingBox() const;
};


class LightSource {
    Point position;
    double intensity;

public:
    LightSource() : intensity(0) { }

    LightSource(const Point &position, double intensity) : position(position), intensity(intensity) { }

    Point getPosition() const { return position; }

    double getIntensity() const { return intensity; }
};


#endif //RAYTRACING_PRIMITIVES_H
