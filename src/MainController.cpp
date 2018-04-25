#include "../headers/MainController.h"
#include<QDebug>
/**
 * @brief MainController::MainController
 */
MainController::MainController()
{
    this->adapter = new Adapter();
    this->rotation = new Rotation();
}
/**
 * @brief MainController::getSrcImage
 * @return
 */
cv::Mat MainController::getSrcImage()
{
    return this->srcImage;
}
/**
 * @brief MainController::getDstImage
 * @return
 */
cv::Mat MainController::getDstImage()
{
    return this->dstImage;
}
/**
 * @brief MainController::showImage
 * @param image
 * @return
 */
QPixmap MainController::showImage(cv::Mat image)
{
    return this->adapter->convertMatToQPixmap(image);
}
/**
 * @brief MainController::getSecondPoint
 * @return
 */
QPointF MainController::getSecondPoint() const
{
    return secondPoint;
}
/**
 * @brief MainController::getFlipPt
 * @return
 */
bool MainController::getFlipPt() const
{
    return flipPt;
}
/**
 * @brief MainController::getFirstPoint
 * @return
 */
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
/**
 * @brief MainController::loadImage
 * @param path
 */
void MainController::loadImage(QString path)
{
    this->srcImage = cv::imread(path.toStdString());
    this->dstImage = this->srcImage.clone();
}
/**
 * @brief MainController::saveImage
 * @param path
 */
void MainController::saveImage(QString path)
{
    cv::imwrite(path.toStdString(),this->dstImage);
}
/**
 * @brief MainController::rotateImgShifts
 * @param degree
 * @param type
 */
void MainController::rotateImgShifts(int degree, Interpolation::INTERPOLATIONS type)
{
    cv::Mat img;
    img = this->getDstImage();
    cv::Point2f middle = cv::Point2f(0.5f*img.cols, 0.5f*img.rows);
    this->dstImage = rotation->rotateShear(degree, img, type, middle);
}
/**
 * @brief MainController::flipPoints
 */
void MainController::flipPoints()
{
    this->flipPt= !flipPt;
}
/**
 * @brief MainController::rotatePart
 * @param degree
 * @param type
 */
void MainController::rotatePart(int degree, Interpolation::INTERPOLATIONS type)
{
    cv::Mat img;
    img = this->getDstImage();
    this->dstImage = rotation->rotateShearPart(degree, img, type, firstPoint, secondPoint);
}
/**
 * @brief MainController::imageReset
 */
void MainController::imageReset()
{
    this->dstImage = srcImage.clone();
}
