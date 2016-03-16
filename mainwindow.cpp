#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Utility/opencv_qt.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"));
    cv::Mat mat = cv::imread(fileName.toStdString());
    QImage image = matToImage(mat);
    QPixmap pixmap = QPixmap::fromImage(image);
    if ( ! pixmap.isNull() )
    {
        if (scene)
            delete scene;
        scene = new QGraphicsScene(this);
        ui->graphicsView->setScene(scene);
        pixmapItem = scene->addPixmap(pixmap);
    }
    else
        ui->statusBar->showMessage(tr("File opening error"), 3000);
}

void MainWindow::addNextPicture(const cv::Mat &mat)
{
    qreal nextY = pixmapItem->y() + pixmapItem->boundingRect().height() + 1;
    pixmapItem = scene->addPixmap(QPixmap::fromImage(matToImage(mat)));
    pixmapItem->setY(nextY);
}

void MainWindow::on_actionProcess_triggered()
{
    if ( ! scene )
        return;
    // Step 0
    QImage image = pixmapItem->pixmap().toImage();
    cv::Mat mat = imageToMat(image);
    // Step 1
    int size = 42;
//    cv::Mat circle = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size/2+5, size/2+5), cv::Point(20, 20));
//    circle = cv::Scalar(255);
    cv::Mat circle = cv::Mat::zeros(size, size, CV_8U);
    size /= 2;
    cv::circle(circle, cv::Point(size, size), size, cv::Scalar(255));
    cv::Mat mat1;
    cv::erode(mat, mat1, circle);

    addNextPicture(mat1);
    // Step 2
    size = 40;
    cv::Mat circle2 = cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size(size, size) );
    cv::Mat mat2;
    cv::dilate(mat1, mat2, circle2);

    addNextPicture(mat2);
    // Step 3
    cv::Mat mat3;
    cv::bitwise_or(mat, mat2, mat3);

    addNextPicture(mat3);
    // Step 4
    size = 117;
    cv::Mat circle3 = cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size(size, size) );
    cv::Mat innerCircle;
    cv::morphologyEx(mat3, innerCircle, cv::MORPH_OPEN, circle3);

    cv::Mat center;
    cv::erode(mat3, center, circle3);

    size = 119;
    cv::Mat circle4 = cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size(size, size) );
    cv::Mat innerCircleBig;
    cv::dilate(center, innerCircleBig, circle4);

    size = 127;
    cv::Mat circle5 = cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size(size, size) );
    cv::Mat outerCircle;
    cv::dilate(center, outerCircle, circle5);

    cv::Mat mat4;
    cv::bitwise_xor(outerCircle, innerCircleBig, mat4);

    addNextPicture(mat4);
    // Step 5
    cv::Mat mat5;
    cv::bitwise_and(mat, mat4, mat5);

    addNextPicture(mat5);
    // Step 6
    size = 8;
    cv::Mat circle6 = cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size(size, size) );
    cv::Mat mat6;
    cv::dilate(mat5, mat6, circle6);

    addNextPicture(mat6);
    // Step 7
    size = 4;
    cv::Mat circle7 = cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size(size, size) );
    cv::Mat mat7_a;
    cv::dilate(mat4, mat7_a, circle7);
//    addNextPicture(mat7_a);

    cv::Mat mat7_b;
    cv::bitwise_xor(mat7_a, mat6, mat7_b);
//    addNextPicture(mat7_b);

    cv::Mat circle8 = cv::Mat::zeros(size, size, CV_8U);
    size /= 2;
    cv::circle(circle8, cv::Point(size, size), size, cv::Scalar(255));
    cv::Mat mat7_c;
    cv::erode(mat7_b, mat7_c, circle8);
//    addNextPicture(mat7_c);

    size = 25;
    cv::Mat circle9 = cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size(size, size) );
    cv::Mat mat7_d;
    cv::dilate(mat7_c, mat7_d, circle9);
//    addNextPicture(mat7_d);

    cv::Mat mat7;
    cv::bitwise_or(mat7_d, mat6, mat7);
    addNextPicture(mat7);
}
