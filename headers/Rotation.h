#ifndef ROTATION_H
#define ROTATION_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class Rotation
{
public:
    Rotation();
    cv::Mat rotateShear(int degree, cv::Mat &img);
    void geometricalTransform(cv::Mat &src, cv::Mat &dst, cv::Mat& transformation);

};

#endif // ROTATION_H
