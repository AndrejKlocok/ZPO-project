#include "headers/Transformation.h"
#include <QDebug>
Transformation::Transformation()
{
    this->interpolation = new Interpolation();
}
/**
 * @brief Transformation::createRotateShearMatrix
 * @param degree
 * @param middle
 * @return
 */
cv::Mat Transformation::createRotateShearMatrix(int degree, cv::Point2f middle)
{
    double rad;
    double alpha, beta;

    cv::Mat M = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat shearX = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat shearY = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat translate = cv::Mat::eye(3, 3, CV_32FC1);


    rad = degree*(CV_PI/180.f);
    alpha = -tan(rad/2.0);
    beta = sin(rad);

    /* Create translate matrix */
    translate.at<float>(0,2) = -middle.y;
    translate.at<float>(1,2) = -middle.x;

    /* Create shearX matrix */
    shearX.at<float>(0,1) = alpha;

    /*  Create shearY matrix */
    shearY.at<float>(1,0) = beta;

    // rotation is equal to shearX * shearY * shearX
    M = translate.inv() * (shearX *(shearY * (shearX * translate)));

    return M;
}
/**
 * @brief Rotation::rotateShear
 * @param degree
 * @param img
 * @param type
 * @param middle
 * @return
 */
cv::Mat Transformation::rotateShear(int degree, cv::Mat &img, Interpolation::INTERPOLATIONS type, cv::Point2f middle, bool resized)
{
    cv::Mat result, M;

    M = this->createRotateShearMatrix(degree, middle);

    if(!resized){
        cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), img.size(), degree).boundingRect2f();
        M.at<float>(1,2) += bbox.width/2.0 - img.cols/2.0;
        M.at<float>(0,2) += bbox.height/2.0 - img.rows/2.0;

        cv::Size size = bbox.size();
        this->interpolate(type, img, result, M, size);
    }
    else {
        this->interpolate(type, img, result, M, img.size());
    }

    return result;
}
/**
 * @brief Rotation::rotateShearPart
 * @param degree
 * @param img
 * @param type
 * @param firstPoint
 * @param secondPoint
 * @return
 */
cv::Mat Transformation::rotateShearPart(int degree, cv::Mat &img, Interpolation::INTERPOLATIONS type, cv::RotatedRect * rectangle, cv::Point2f * middle)
{
    cv::Mat imgTmp, imgSmall, rotated;
    cv::Vec3b whiteColor;

    imgTmp = img.clone();
    imgSmall = img.clone();

    //white color
    whiteColor.val[0] = 0;
    whiteColor.val[1] = 0;
    whiteColor.val[2] = 0;

    for(int x=0; x<imgTmp.rows; x++)
        for(int y=0; y<imgTmp.cols; y++)
            // crop rectangle-> in original cut rectangle
            // -> in rotated one white out rest out of rectangle
            if(rectangle->boundingRect().contains(cv::Point(y, x)))
                imgTmp.at<cv::Vec3b>(x,y) = whiteColor;
            else
                imgSmall.at<cv::Vec3b>(x,y) = whiteColor;

    rotated = this->rotateShear(degree, imgSmall, type, *middle, true);

    for(int x=0; x<rotated.rows; x++)
        for(int y=0; y<rotated.cols; y++)
            // if pixel in croped image isnt white -> coppy
            if(rotated.at<cv::Vec3b>(x,y)!= whiteColor)
                imgTmp.at<cv::Vec3b>(x,y) = rotated.at<cv::Vec3b>(x,y);

    return imgTmp;
}

cv::Mat Transformation::scale(float times, cv::Mat &img)
{
    cv::Mat scaleMatrix, scaled;
    cv::Size size;
    scaleMatrix = cv::Mat::eye(2, 3, CV_32FC1);

    scaleMatrix.at<float>(0,0) = times;
    scaleMatrix.at<float>(1,1) = times;

    if(times>1)
        size =  img.size() * 2;
    else
        size = img.size() / 2;

    cv::warpAffine(img, scaled, scaleMatrix, size);
    return scaled;
}
/**
 * @brief Rotation::interpolate
 * @param type
 * @param img
 * @param result
 * @param M
 * @param size
 */
void Transformation::interpolate(Interpolation::INTERPOLATIONS type, cv::Mat &img, cv::Mat &result, cv::Mat &M, cv::Size size)
{
    switch (type) {
    case Interpolation::INTERPOLATIONS::nearest:
        this->interpolation->NearestNeighbor(img, result, M, size);
        break;

    case Interpolation::INTERPOLATIONS::bilinear:
        this->interpolation->Bilinear(img, result, M,size);
        break;

    case Interpolation::INTERPOLATIONS::bicubic:
        this->interpolation->Bicubic(img, result, M, size);
        break;

    default:
        break;
    }

}
