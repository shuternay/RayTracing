#ifndef RAYTRACING_VECTOR_H
#define RAYTRACING_VECTOR_H

#include <cmath>

class Vector {
    double x, y, z;

    static constexpr double eps = 1e-6;
    
 public:
    Vector() = default;

    Vector(double x, double y, double z) : x(x), y(y), z(z) { }

    double getX() const { return x; }

    double getY() const { return y; }

    double getZ() const { return z; }

    void setX(double _x) { x = _x; }

    void setY(double _y) { y = _y; }

    void setZ(double _z) { z = _z; }

    Vector operator+(const Vector &v) const {
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vector operator-(const Vector &v) const {
        return Vector(x - v.x, y - v.y, z - v.z);
    }

    friend Vector operator*(double m, const Vector &v) {
        return Vector(m * v.x, m * v.y, m * v.z);
    }

    friend Vector operator*(const Vector &v, double m) {
        return Vector(m * v.x, m * v.y, m * v.z);
    }

    friend double operator%(const Vector &a, const Vector &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    friend Vector operator*(const Vector &a, const Vector &b) {
        return Vector(a.y * b.z - a.z * b.y,
                      a.z * b.x - a.x * b.z,
                      a.x * b.y - a.y * b.x);
    }

    friend double mixedProduct(const Vector &a, const Vector &b, const Vector &c) {
        return a.x * (b.y * c.z - b.z * c.y) + a.y * (b.z * c.x - b.x * c.z) + a.z * (b.x * c.y - b.y * c.x);
    }

    double length() const {
        return std::sqrt(*this % *this);
    }

    double squaredLength() const {
        return *this % *this;
    }

    Vector normalise(double k = 1) const {
        return Vector(x * k / length(), y * k / length(), z * k / length());
    }

    Vector reflect(const Vector &normal) const {
        return *this - normal.normalise(2 * (*this) % normal);
    }

    Vector refract(const Vector &normal, double n) const {
        Vector projection = normal.normalise(*this % normal);
        Vector height = *this - projection;
        double sin2 = n * sqrt(1 - projection.squaredLength() / this->squaredLength());
        if (doubleGreaterOrEqual(sin2, 1)) {  // internal reflection
            return reflect(normal);
        }

        double cos2 = sqrt(1 - sin2 * sin2);
        return projection.normalise(cos2) + height.normalise(sin2);
    }

    Vector putHeight(const Vector &A, const Vector &B) const {
        return A + (B - A).normalise((B - A) % (*this - A) / (B - A).length());
    }

    static bool doubleEqual(double a, double b) {
        return std::abs(a - b) < eps;
    }

    static bool doubleGreater(double a, double b) {
        return a > b && !doubleEqual(a, b);
    }

    static bool doubleLess(double a, double b) {
        return a < b && !doubleEqual(a, b);
    }

    static bool doubleLessOrEqual(double a, double b) {
        return a < b || doubleEqual(a, b);
    }

    static bool doubleGreaterOrEqual(double a, double b) {
        return a > b || doubleEqual(a, b);
    }
};

typedef Vector Point;


double tetrahedronOriendtedVolume(const Point &O, const Point &A, const Point &B, const Point &C);

#endif //RAYTRACING_VECTOR_H
