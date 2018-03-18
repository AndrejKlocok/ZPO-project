#include "../headers/Adapter.h"

QPixmap Adapter::convertMatToQPixmap(cv::Mat img)
{
    cv::Mat newImg;
    cv::cvtColor(img, newImg, CV_BGR2RGB); // Convert colors
    //convert to QImage
    return QPixmap::fromImage(QImage(newImg.data, newImg.cols, newImg.rows, newImg.step,
                                     QImage::Format_RGB888));
}

Adapter::Adapter()
{

}
