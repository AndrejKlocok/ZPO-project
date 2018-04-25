#ifndef ROTATION_H
#define ROTATION_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <../headers/Interpolation.h>

#include<QObject>
#include<QPixmap>

/**
 * @brief The Rotation class
 */
class Transformation
{
public:
    Transformation();
    cv::Mat createRotateShearMatrix(int degree, cv::Point2f middle);
    cv::Mat rotateShear(int degree, cv::Mat &img, Interpolation::INTERPOLATIONS type, cv::Point2f middle, bool resized);
    cv::Mat rotateShearPart(int degree, cv::Mat &img, Interpolation::INTERPOLATIONS type, cv::RotatedRect *rectangle, cv::Point2f *middle);
    cv::Mat scale(float times, cv::Mat &img);
    void interpolate(Interpolation::INTERPOLATIONS type, cv::Mat &img, cv::Mat &result, cv::Mat &M, cv::Size size);


private:
    Interpolation *interpolation;
};

#endif // ROTATION_H
