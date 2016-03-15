#ifndef OPENCV_QT_H
#define OPENCV_QT_H

#include <QImage>
#include <opencv2/imgproc/imgproc.hpp>

QImage matToImage(const cv::Mat &mat);
cv::Mat imageToMat(QImage &image);

#endif // OPENCV_QT_H
