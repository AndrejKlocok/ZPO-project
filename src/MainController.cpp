#include "../headers/MainController.h"
#include<QDebug>
/**
 * @brief MainController::MainController
 */
MainController::MainController()
{
    this->adapter = new Adapter();
    this->transformation = new Transformation();
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

bool MainController::getResized() const
{
    return resized;
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
    if(!this->flipPt){
        firstPoint = value;
    }
    else {
        secondPoint = value;
    }
    cv::Rect rec(firstPoint.x(), firstPoint.y(), abs(secondPoint.x() - firstPoint.x())+ 1, abs(secondPoint.y() - firstPoint.y()) + 1);
    cv::Point2f middle = cv::Point2f(abs(secondPoint.x() - firstPoint.x())/2 + firstPoint.x(), abs(secondPoint.y() - firstPoint.y())/2 + firstPoint.y());

    this->rectangle = cv::RotatedRect(middle, rec.size(), 0);
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
    this->resized = false;
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
    this->dstImage = transformation->rotateShear(degree, img, type, middle, this->resized);
    this->resized = true;
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

    cv::Point2f middle = cv::Point2f(abs(secondPoint.x() - firstPoint.x())/2 + firstPoint.x(), abs(secondPoint.y() - firstPoint.y())/2 + firstPoint.y());
    cv::Rect rec(firstPoint.y(), firstPoint.x(), abs(secondPoint.y() - firstPoint.y()) + 1, abs(secondPoint.x() - firstPoint.x())+ 1);

    this->dstImage = transformation->rotateShearPart(degree, img, type, &rectangle, &middle);
    this->rectangle = cv::RotatedRect(middle, rec.size(), degree);
}
/**
 * @brief MainController::imageReset
 */
void MainController::imageReset()
{
    this->firstPoint = QPointF(0,0);
    this->secondPoint = QPointF(0,0);
    this->dstImage = srcImage.clone();
    this->resized = false;
}
/**
 * @brief MainController::scaleImg
 * @param times
 */
void MainController::scaleImg(float times)
{
    cv::Mat img;
    img = this->getDstImage();
    this->dstImage = transformation->scale(times, img);
    if(times > 1.0)
        this->resized = true;
    else
        this->resized = false;
}
