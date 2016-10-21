#include "ConvertToQImage.h"

ConvertToQImage::ConvertToQImage(Render *render) {
    qImage = QImage(render->getWidth(), render->getHeight(), QImage::Format_RGB32);

    for (int y = 0; y < render->getHeight(); ++y) {
        for (int x = 0; x < render->getWidth(); ++x) {
            qImage.setPixel(x, y, render->getPixel(y, x).toQRgb());
        }
    }
};

