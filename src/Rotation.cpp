#include "headers/Rotation.h"
#include <QDebug>
Rotation::Rotation()
{
    this->interpolation = new Interpolation();
}

cv::Mat Rotation::rotateShear(int degree, cv::Mat &img, Interpolation::INTERPOLATIONS type, cv::Point2f middle)
{
    double rad;
    double alpha, beta;

    cv::Mat M = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat shearX = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat shearY = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat translate = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat result;

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

    this->interpolate(type, img, result, M);

    cv::imwrite("obrazok.png", result);
    return result;
}

cv::Mat Rotation::rotateShearPart(int degree, cv::Mat &img, Interpolation::INTERPOLATIONS type, QPointF firstPoint, QPointF secondPoint)
{
    cv::Mat imgTmp, imgSmall, rotated;
    cv::Vec3b whiteColor;

    imgTmp = img.clone();
    imgSmall = img.clone();
    cv::Point2f middle = cv::Point2f(abs(secondPoint.x() - firstPoint.x())/2 + firstPoint.x(), abs(secondPoint.y() - firstPoint.y())/2 + firstPoint.y());

    //crop image
     cv::Rect rectangle(firstPoint.y(), firstPoint.x(), abs(secondPoint.y() - firstPoint.y()) + 1, abs(secondPoint.x() - firstPoint.x())+ 1);

    //white color
    whiteColor.val[0] = 0;
    whiteColor.val[1] = 0;
    whiteColor.val[2] = 0;

    for(int x=0; x<imgTmp.rows; x++)
        for(int y=0; y<imgTmp.cols; y++)
            // crop rectangle-> in original cut rectangle
            // -> in rotated one white out rest out of rectangle
            if(rectangle.contains(cv::Point(x,y)))
                imgTmp.at<cv::Vec3b>(x,y) = whiteColor;
            else
                imgSmall.at<cv::Vec3b>(x,y) = whiteColor;

    //rotate
    //rotationMatrix = cv::getRotationMatrix2D(middle, (double) degree, 1.0);
    //get center of rotation
    rotated = this->rotateShear(degree, imgSmall, type, middle);
    //cv::warpAffine(imgSmall, rotated, rotationMatrix, imgSmall.size(), cv::INTER_CUBIC , cv::BORDER_CONSTANT, cv::Scalar::all(0));

    for(int x=0; x<rotated.rows; x++)
        for(int y=0; y<rotated.cols; y++)
            // if pixel in croped image isnt white -> coppy
            if(rotated.at<cv::Vec3b>(x,y)!= whiteColor)
                imgTmp.at<cv::Vec3b>(x,y) = rotated.at<cv::Vec3b>(x,y);

    return imgTmp;
}

void Rotation::interpolate(Interpolation::INTERPOLATIONS type, cv::Mat &img, cv::Mat &result, cv::Mat &M)
{
    switch (type) {
    case Interpolation::INTERPOLATIONS::nearest:
        this->interpolation->NearestNeighbor(img, result, M);
        break;

    case Interpolation::INTERPOLATIONS::bilinear:
        this->interpolation->Bilinear(img, result, M);
        break;

    case Interpolation::INTERPOLATIONS::bicubic:
        this->interpolation->Bicubic(img, result, M);
        break;

    default:
        break;
    }

}
