#include <fstream>

#include "LoadFromRt.h"


void LoadFromRt::load(const std::string &filePath, Scene &scene, Screen &screen) {
    std::ifstream fileStream(filePath);

    if (!fileStream.good()) {
        throw "file opening error";
    }

    std::string currentToken;
    std::map<std::string, Material> materials;

    while (true) {
        fileStream >> currentToken;

        if (currentToken == "viewport") {
            screen = readViewport(fileStream);
        }
        else if (currentToken == "materials") {
            materials = readMaterials(fileStream);
        }
        else if (currentToken == "lights") {
            readLights(fileStream, scene);
        }
        else { // geometry
            readGeometry(fileStream, scene, materials);
            break;
        }
    }

    fileStream.close();
}

Point LoadFromRt::readPoint(std::ifstream &fileStream) {
    double x, y, z;
    fileStream >> x >> y >> z;
    return Point(x, y, z);
}

Color LoadFromRt::readColor(std::ifstream &fileStream) {
    double x, y, z;
    fileStream >> x >> y >> z;
    return Color(x / 255, y / 255, z / 255);
}

Screen LoadFromRt::readViewport(std::ifstream &fileStream) {
    std::string currentToken;

    Point origin, topLeftCorner, bottomLeftCorner, topRightCorner;
    for (int i = 0; i < 4; ++i) {
        fileStream >> currentToken;
        Point currentPoint = readPoint(fileStream);
        if (currentToken == "origin") {
            origin = currentPoint;
        }
        else if (currentToken == "topleft") {
            topLeftCorner = currentPoint;
        }
        else if (currentToken == "bottomleft") {
            bottomLeftCorner = currentPoint;
        }
        else { // topright
            topRightCorner = currentPoint;
        }
    }

    fileStream >> currentToken; // endviewport

    return Screen(origin, bottomLeftCorner, topRightCorner - topLeftCorner, topLeftCorner - bottomLeftCorner);
}

std::map<std::string, Material> LoadFromRt::readMaterials(std::ifstream &fileStream) {
    std::string currentToken;
    std::map<std::string, Material> materials;

    while (true) {
        fileStream >> currentToken; // entry or endmaterials

        if (currentToken == "endmaterials") {
            break;
        }

        std::string name;
        Color color(0, 0, 0);
        double reflect = 0, refract = 0;

        for (int i = 0; i < 5; ++i) {
            fileStream >> currentToken;
            if (currentToken == "name") {
                fileStream >> name;
            }
            else if (currentToken == "color") {
                color = readColor(fileStream);
            }
            else if (currentToken == "reflect") {
                fileStream >> reflect;
            }
            else if (currentToken == "refract") {
                fileStream >> refract;
            }
            else {
                break; // endentry
            }
        }

        materials[name] = Material(color, reflect, refract);
    }

    return materials;
}

void LoadFromRt::readLights(std::ifstream &fileStream, Scene &scene) {
    std::string currentToken;
    double referencePower = 1, referenceDistance = 1;

    while (true) {
        fileStream >> currentToken; // point or endlights

        if (currentToken == "reference") {
            for (int i = 0; i < 2; ++i) {
                fileStream >> currentToken;
                if (currentToken == "power") {
                    fileStream >> referencePower;
                }
                else if (currentToken == "distance") {
                    fileStream >> referenceDistance;
                }
            }
            fileStream >> currentToken; // endreference
            continue;
        }
        else if (currentToken == "endlights") {
            break;
        }

        Point point(0, 0, 0);
        double power = 1;

        for (int i = 0; i < 2; ++i) {
            fileStream >> currentToken;
            if (currentToken == "coords") {
                point = readPoint(fileStream);
            }
            else { // power
                fileStream >> power;
            }
        }
        fileStream >> currentToken; // endpoint

        scene.addLightSource(LightSource(point, power / referencePower * referenceDistance * referenceDistance));
    }
}

void LoadFromRt::readGeometry(std::ifstream &fileStream, Scene &scene, std::map<std::string, Material> &materials) {
    std::string currentToken;

    while (true) {
        fileStream >> currentToken; // point or endlights

        if (currentToken == "sphere") {
            readSphere(fileStream, scene, materials);
        }
        else if (currentToken == "triangle") {
            readTriangle(fileStream, scene, materials);
        }
        else if (currentToken == "quadrangle") {
            readQuadrangle(fileStream, scene, materials);
        } else { // endgeometry
            break;
        }
    }
}

void LoadFromRt::readSphere(std::ifstream &fileStream, Scene &scene, std::map<std::string, Material> &materials) {
    std::string currentToken;
    Point center;
    double radius = 1;
    std::string materialName;

    for (int i = 0; i < 3; ++i) {
        fileStream >> currentToken;
        if (currentToken == "coords") {
            center = readPoint(fileStream);
        }
        else if (currentToken == "radius") {
            fileStream >> radius;
        }
        else { // material
            fileStream >> materialName;
        }
    }
    fileStream >> currentToken; // endsphere

    scene.addObject(new Sphere(center, radius, materials[materialName]));
}

void LoadFromRt::readTriangle(std::ifstream &fileStream, Scene &scene, std::map<std::string, Material> &materials) {
    std::string currentToken;
    std::vector<Point> vertices;
    std::string materialName;

    for (int i = 0; i < 4; ++i) {
        fileStream >> currentToken;
        if (currentToken == "vertex") {
            vertices.push_back(readPoint(fileStream));
        }
        else { // material
            fileStream >> materialName;
        }
    }
    fileStream >> currentToken; // endtriangle

    scene.addObject(new Triangle(vertices[0], vertices[1], vertices[2], materials[materialName]));
}

void LoadFromRt::readQuadrangle(std::ifstream &fileStream, Scene &scene, std::map<std::string, Material> &materials) {
    std::string currentToken;
    std::vector<Point> vertices;
    std::string materialName;

    for (int i = 0; i < 5; ++i) {
        fileStream >> currentToken;
        if (currentToken == "vertex") {
            vertices.push_back(readPoint(fileStream));
        }
        else { // material
            fileStream >> materialName;
        }
    }
    fileStream >> currentToken; // endquadrangle

    scene.addObject(new Polygon(vertices, materials[materialName]));
}

















