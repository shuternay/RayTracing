#ifndef RAYTRACING_DISPLAYTOQT_H
#define RAYTRACING_DISPLAYTOQT_H

#include <QImage>
#include "Render.h"

class ConvertToQImage {
    QImage qImage;

public:
    ConvertToQImage(Render *render);

    QImage getQImage() const {
        return qImage;
    }
};


#endif //RAYTRACING_DISPLAYTOQT_H
