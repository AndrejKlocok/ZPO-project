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

void MainController::rotateImgShifts(int degree, Interpolation::INTERPOLATIONS type)
{
    cv::Mat img;
    img = this->getDstImage();
    cv::Point2f middle = cv::Point2f(0.5f*img.cols, 0.5f*img.rows);
    this->dstImage = rotation->rotateShear(degree, img, type, middle);
}

void MainController::flipPoints()
{
    this->flipPt= !flipPt;
}

void MainController::rotatePart(int degree, Interpolation::INTERPOLATIONS type)
{
    cv::Mat img;
    img = this->getDstImage();
    this->dstImage = rotation->rotateShearPart(degree, img, type, firstPoint, secondPoint);
}

void MainController::imageReset()
{
    this->dstImage = srcImage.clone();
}
