#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<QDebug>
/**
 * @brief The Interpolation class
 */

enum INTERPOLATIONS {nearest, bilinear, bicubic};

class Interpolation
{

public:
    Interpolation();
    void NearestNeighbor(cv::Mat &src, cv::Mat &dst, cv::Mat &transformation, cv::Size size);
    void Bilinear(cv::Mat &src, cv::Mat &dst, cv::Mat &transformation, cv::Size size);
    void Bicubic(cv::Mat &src, cv::Mat &dst, cv::Mat &transformation, cv::Size size);
    float cubicInterpolateRow (float p0, float p1, float p2, float p3, float x);
};

#endif // INTERPOLATION_H
