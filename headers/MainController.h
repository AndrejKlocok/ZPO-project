#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<QString>
#include<QFileDialog>
#include<QPixmap>
#include<QObject>

#include "../headers/Adapter.h"
#include "../headers/Rotation.h"

class MainController
{
public:
    MainController();
    void loadImage(QString path);
    void saveImage(QString path);
    void rotateImgShifts(int degree, Interpolation::INTERPOLATIONS type);
    void flipPoints();
    void rotatePart(int degree, Interpolation::INTERPOLATIONS type);
    void imageReset();

    cv::Mat getSrcImage();
    cv::Mat getDstImage();
    QPixmap showImage(cv::Mat image);

    QPointF getFirstPoint() const;
    void setPoint(const QPointF &value);
    QPointF getSecondPoint() const;

    bool getFlipPt() const;

private:
    Adapter *adapter;
    cv::Mat srcImage;
    cv::Mat dstImage;
    QPointF firstPoint = QPointF(0,0);
    QPointF secondPoint = QPointF(0,0);
    bool flipPt=false;  //flip first and second point of rectangle
    Rotation *rotation;
};

#endif // MAINCONTROLLER_H
