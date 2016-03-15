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

void MainWindow::on_actionProcess_triggered()
{
    QImage image = pixmapItem->pixmap().toImage();
    cv::Mat mat = imageToMat(image);
    cv::Mat mat1( mat.rows, mat.cols, mat.type() );
    cv::Mat circle = cv::Mat::zeros(41, 41, CV_8U);
    cv::circle(circle, cv::Point(41.0 / 2, 41.0 / 2), 41.0 / 2, cv::Scalar(255));
    cv::erode(mat, mat1, circle);
    qreal nextY = pixmapItem->boundingRect().height() + 1;
    pixmapItem = scene->addPixmap(QPixmap::fromImage(matToImage(mat1)));
    pixmapItem->setY(nextY);
}
