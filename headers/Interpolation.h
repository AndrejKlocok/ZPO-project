#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/**
 * @brief The Interpolation class
 */
class Interpolation
{

public:
    enum INTERPOLATIONS {nearest, bilinear, bicubic};
    Interpolation();
    void NearestNeighbor(cv::Mat &src, cv::Mat &dst, cv::Mat &transformation);
    void Bilinear(cv::Mat &src, cv::Mat &dst, cv::Mat &transformation);
    void Bicubic(cv::Mat &src, cv::Mat &dst, cv::Mat &transformation);
};

#endif // INTERPOLATION_H
