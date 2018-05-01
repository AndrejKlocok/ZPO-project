#ifndef ROTATION_H
#define ROTATION_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <./headers/Interpolation.h>

#include<QObject>
#include<QPixmap>

/**
 * @brief The Transformation class
 */

enum ROTATIONS {normal, shear};

class Transformation
{
public:

    Transformation();
    cv::Mat rotateShear(int degree, cv::Mat &img, INTERPOLATIONS type, cv::Point2f middle, bool resized);
    cv::Mat rotatePart(int degree, cv::Mat &img, INTERPOLATIONS type, cv::RotatedRect *rectangle, cv::Point2f *middle, ROTATIONS rotate);
    cv::Mat rotateNormal(int degree, cv::Mat &img, INTERPOLATIONS type, cv::Point2f middle, bool resized);
    cv::Mat scale(float times, cv::Mat &img);

private:
    void interpolate(INTERPOLATIONS type, cv::Mat &img, cv::Mat &result, cv::Mat &M, cv::Size size);
    cv::Mat createRotateShearMatrix(int degree, cv::Point2f middle);
    void adjustSize(cv::Mat &result, cv::Mat &M, cv::Mat &img, int degree, INTERPOLATIONS type, bool resized);

private:
    Interpolation *interpolation;
};

#endif // ROTATION_H
