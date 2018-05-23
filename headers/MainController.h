#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<QString>
#include<QFileDialog>
#include<QPixmap>
#include<QObject>
#include<QElapsedTimer>

#include "../headers/Adapter.h"
#include "../headers/Transformation.h"

/**
 * @brief The MainController class
 */
class MainController
{
public:
    MainController();
    void loadImage(QString path);
    void saveImage(QString path);
    qint64 rotateImg(int degree, INTERPOLATIONS type, ROTATIONS rotate);
    void flipPoints();
    qint64 rotatePart(int degree, INTERPOLATIONS type, ROTATIONS rotate);
    void imageReset();
    void scaleImg(float times);

    cv::Mat getSrcImage();
    cv::Mat getDstImage();
    QPixmap showImage(cv::Mat image, bool GrayScale);

    QPointF getFirstPoint() const;
    void setPoint(const QPointF &value);
    QPointF getSecondPoint() const;

    bool getFlipPt() const;

    bool getResized() const;

private:
    Adapter *adapter;
    cv::Mat srcImage;
    cv::Mat dstImage;
    cv::RotatedRect rectangle;
    QPointF firstPoint = QPointF(0,0);
    QPointF secondPoint = QPointF(0,0);
    bool flipPt=false;
    Transformation *transformation;
    bool resized=false;


};

#endif // MAINCONTROLLER_H
