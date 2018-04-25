#ifndef ROTATION_H
#define ROTATION_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <../headers/Interpolation.h>

#include<QObject>
#include<QPixmap>

class Rotation
{
public:
    Rotation();
    cv::Mat rotateShear(int degree, cv::Mat &img, Interpolation::INTERPOLATIONS type, cv::Point2f middle);
    cv::Mat rotateShearPart(int degree, cv::Mat &img, Interpolation::INTERPOLATIONS type, QPointF firstPoint, QPointF secondPoint);
    void interpolate(Interpolation::INTERPOLATIONS type, cv::Mat &img, cv::Mat &result, cv::Mat &M);

private:
    Interpolation *interpolation;
};

#endif // ROTATION_H
