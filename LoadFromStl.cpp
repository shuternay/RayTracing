#include "LoadFromStl.h"

void LoadFromStl::load(const std::string &filePath,
                       const Material &material,
                       Scene &scene) {
    std::ifstream fileStream(filePath);

    if (!fileStream.good()) {
        throw "file opening error";
    }

    std::string currentToken;

    fileStream >> currentToken; // facet or endsolid
    fileStream >> currentToken; // facet or endsolid

    while (true) {
        fileStream >> currentToken; // facet or endsolid

        if (currentToken == "endsolid") {
            break;
        }
        else if (currentToken == "") {
            continue;
        }

        double nX, nY, nZ;
        fileStream >> currentToken; // normal
        fileStream >> nX >> nY >> nZ;

        fileStream >> currentToken; // outer
        fileStream >> currentToken; // loop

        double v1X, v1Y, v1Z;
        fileStream >> currentToken; // vertex
        fileStream >> v1X >> v1Y >> v1Z;
        double v2X, v2Y, v2Z;
        fileStream >> currentToken; // vertex
        fileStream >> v2X >> v2Y >> v2Z;
        double v3X, v3Y, v3Z;
        fileStream >> currentToken; // vertex
        fileStream >> v3X >> v3Y >> v3Z;

        fileStream >> currentToken; // endloop
        fileStream >> currentToken; // endfacet

        scene.addObject(
                new Triangle(Point(v1X, v1Y, v1Z), Point(v2X, v2Y, v2Z), Point(v3X, v3Y, v3Z), Vector(nX, nY, nZ),
                             material));
    }

    fileStream.close();
}