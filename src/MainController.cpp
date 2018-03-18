#include "../headers/MainController.h"

MainController::MainController()
{
    this->adapter = new Adapter();
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

void MainController::loadImage(QString path)
{
    this->srcImage = cv::imread(path.toStdString());
}

void MainController::saveImage(QString path)
{
    cv::imwrite(path.toStdString(),this->dstImage);
}

void MainController::rotateImgRows(int degree)
{

    cv::Point2f point = cv::Point2f(0.5f*this->getSrcImage().cols, 0.5f*this->getSrcImage().rows);
    cv::Mat matrix = cv::getRotationMatrix2D(point, (double) degree, 1);
    cv::warpAffine(this->getSrcImage(), this->dstImage, matrix, this->getSrcImage().size(),cv::INTER_CUBIC, cv::BORDER_CONSTANT, cv::Scalar::all(0));

}
