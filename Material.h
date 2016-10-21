#ifndef RAYTRACING_COLOR_H
#define RAYTRACING_COLOR_H

#include <QRgb>

class Color {
    double r, g, b;

public:
    Color() = default;

    Color(double r, double g, double b) : r(r), g(g), b(b) { }

    QRgb toQRgb() const {
        return qRgb(std::min(255.0, r * 255), std::min(255.0, g * 255), std::min(255.0, b * 255));
    }

    friend Color operator*(double m, const Color &c) {
        return Color(m * c.r, m * c.g, m * c.b);
    }

    friend Color operator*(const Color &c, double m) {
        return Color(m * c.r, m * c.g, m * c.b);
    }

    friend Color operator+(const Color &c1, const Color &c2) {
        return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
    }
};

class Material {
    Color color;
    double reflect;
    double refract;

public:
    Material() = default;

    Material(const Color &color, double reflect, double refract) :
            color(color), reflect(reflect), refract(refract) { }

    const Color &getColor() const {
        return color;
    }

    double getReflect() const {
        return reflect;
    }

    double getRefract() const {
        return refract;
    }
};


#endif //RAYTRACING_COLOR_H