#include "headers/Rotation.h"
#include <QDebug>
Rotation::Rotation()
{

}

cv::Mat Rotation::rotateShear(int degree, cv::Mat &img)
{
    double rad;
    double alpha, beta;

    cv::Mat M = cv::Mat::eye(3, 3, CV_32F);
    cv::Mat shearX = cv::Mat::eye(3, 3, CV_32F);
    cv::Mat shearY = cv::Mat::eye(3, 3, CV_32F);
    cv::Mat translate = cv::Mat::eye(3, 3, CV_32F);
    cv::Mat result;

    cv::Point2f middle;

    rad = degree*(CV_PI/180.f);
    alpha = -tan(rad/2.0);
    beta = sin(rad);
    middle = cv::Point2f(0.5f*img.cols, 0.5f*img.rows);

    /* Create translate matrix */
    translate.at<float>(0,2) = -middle.y;
    translate.at<float>(1,2) = -middle.x;

    /* Create shearX matrix */
    shearX.at<float>(0,1) = alpha;

    /*  Create shearY matrix */
    shearY.at<float>(1,0) = beta;

    // rotation is equal to shearX * shearY * shearX
    M = translate.inv() * (shearX *(shearY * (shearX * translate)));
    geometricalTransform(img, result, M);

    return result;
}

void Rotation::geometricalTransform(cv::Mat &src, cv::Mat &dst, cv::Mat &transformation)
{
    float xn, yn;
    cv::Mat T = transformation.inv();
    dst = cv::Mat::zeros(src.size(), src.type());

    for( int x=0; x<src.rows;x++){
        for(int y=0; y<src.cols; y++){
            xn = x*T.at<float>(0,0) + y*T.at<float>(0,1) + T.at<float>(0,2);
            yn = x*T.at<float>(1,0) + y*T.at<float>(1,1) + T.at<float>(1,2);

            if(xn < 0 || xn >= src.rows || yn < 0 || yn >= src.cols)
                continue;
            dst.at<cv::Vec3b>(x, y) = src.at<cv::Vec3b>(cvRound(xn), cvRound(yn));
        }
    }

}
