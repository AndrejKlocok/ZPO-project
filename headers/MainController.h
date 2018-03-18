#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<QString>
#include<QFileDialog>
#include<QPixmap>
#include<QObject>

#include "../headers/Adapter.h"

class MainController
{
public:
    MainController();
    void loadImage(QString path);
    void saveImage(QString path);
    void rotateImgRows(int degree);

    cv::Mat getSrcImage();
    cv::Mat getDstImage();
    QPixmap showImage(cv::Mat image);

private:
    Adapter *adapter;
    cv::Mat srcImage;
    cv::Mat dstImage;
};

#endif // MAINCONTROLLER_H
