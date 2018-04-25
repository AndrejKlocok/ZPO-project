#include "headers/Interpolation.h"

Interpolation::Interpolation()
{

}
/**
 * @brief Interpolation::NearestNeighbor
 * @param src
 * @param dst
 * @param transformation
 */
void Interpolation::NearestNeighbor(cv::Mat &src, cv::Mat &dst, cv::Mat &transformation)
{
    float xn, yn;
    cv::Mat T = transformation.inv();
    dst = cv::Mat::zeros(src.size(), src.type());

    for( int x=0; x<src.rows -1;x++){
        for(int y=0; y<src.cols -1; y++){
            xn = x*T.at<float>(0,0) + y*T.at<float>(0,1) + T.at<float>(0,2);
            yn = x*T.at<float>(1,0) + y*T.at<float>(1,1) + T.at<float>(1,2);

            if(xn < 0 || xn >= src.rows || yn < 0 || yn >= src.cols)
                continue;
            dst.at<cv::Vec3b>(x, y) = src.at<cv::Vec3b>(cvRound(xn), cvRound(yn));
        }
    }

}
/**
 * @brief Interpolation::Bilinear
 * @param src
 * @param dst
 * @param transformation
 */
void Interpolation::Bilinear(cv::Mat &src, cv::Mat &dst, cv::Mat &transformation)
{
    // float xn, yn;
    cv::Mat T = transformation.inv();
    dst = cv::Mat::zeros(src.size(), src.type());

}
/**
 * @brief Interpolation::Bicubic
 * @param src
 * @param dst
 * @param transformation
 */
void Interpolation::Bicubic(cv::Mat &src, cv::Mat &dst, cv::Mat &transformation)
{
    //float xn, yn;
    cv::Mat T = transformation.inv();
    dst = cv::Mat::zeros(src.size(), src.type());

}
