#include "opencv_qt.h"

#include <QImage>

QImage matToImage(const cv::Mat &mat)
{
    const uchar *qImageBuffer = (const uchar*) mat.data;
    QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return img.rgbSwapped();
}

cv::Mat imageToMat(QImage &image)
{
    QImage img = image.convertToFormat(QImage::Format_RGB888).rgbSwapped();
    uchar *matImageBuffer = img.bits();
    cv::Mat mat(img.height(), img.width(), CV_8UC3, matImageBuffer, img.bytesPerLine());
    return mat.clone();
}
