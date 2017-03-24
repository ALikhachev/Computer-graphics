#ifndef UTILS_H
#define UTILS_H

namespace FilterUtils {
    void inline emptyImage(QImage &image) {
        memset(image.bits(), 0xFF, image.height() * image.bytesPerLine());
    }
}

#endif // UTILS_H
