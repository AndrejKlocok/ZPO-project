#include "../headers/MainController.h"
#include<QDebug>

MainController::MainController()
{
    this->adapter = new Adapter();
    this->rotation = new Rotation();
}

cv::Mat MainController::getSrcImage()
{
    return this->srcImage;
}

cv::Mat MainController::getDstImage()
{
    return this->dstImage;
}

QPixmap MainController::showImage(cv::Mat image)
{
    return this->adapter->convertMatToQPixmap(image);
}

QPointF MainController::getSecondPoint() const
{
    return secondPoint;
}

bool MainController::getFlipPt() const
{
    return flipPt;
}

QPointF MainController::getFirstPoint() const
{
    return firstPoint;
}
/** According to flipPt bool value (false -> sets firstPoint, true-> sets secondPoint)
 * @brief MainController::setPoint
 * @param value
 */
void MainController::setPoint(const QPointF &value)
{
    if(!this->flipPt)
        firstPoint = value;
    else
        secondPoint = value;

    this->flipPoints();
}

void MainController::loadImage(QString path)
{
    this->srcImage = cv::imread(path.toStdString());
    this->dstImage = this->srcImage.clone();
}

void MainController::saveImage(QString path)
{
    cv::imwrite(path.toStdString(),this->dstImage);
}

void MainController::rotateImgShifts(int degree)
{
    cv::Mat img;
    img = this->getDstImage();
    this->dstImage = rotation->rotateShear(degree, img);
}

void MainController::flipPoints()
{
    this->flipPt= !flipPt;
}

void MainController::rotatePart(int degree)
{
    cv::Point2f middle;
    cv::Mat rotationMatrix, imgTmp, imgSmall, rotated;
    cv::Vec3b whiteColor;

    imgTmp = getDstImage().clone();
    imgSmall = getDstImage().clone();

    middle = cv::Point2f(abs(secondPoint.x() - firstPoint.x())/2 + firstPoint.x(), abs(secondPoint.y() - firstPoint.y())/2 + firstPoint.y());


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
    rotated = rotation->rotateShear(degree, imgSmall);
    //cv::warpAffine(imgSmall, rotated, rotationMatrix, imgSmall.size(), cv::INTER_CUBIC , cv::BORDER_CONSTANT, cv::Scalar::all(0));

    for(int x=0; x<rotated.rows; x++)
        for(int y=0; y<rotated.cols; y++)
            // if pixel in croped image isnt white -> coppy
            if(rotated.at<cv::Vec3b>(x,y)!= whiteColor)
                imgTmp.at<cv::Vec3b>(x,y) = rotated.at<cv::Vec3b>(x,y);


    this->dstImage = imgTmp;
}

void MainController::imageReset()
{
    this->dstImage = srcImage.clone();
}
