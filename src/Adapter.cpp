#include "../headers/Adapter.h"
/**
 * @brief Adapter::convertMatToQPixmap  - Transformacie obrazku charakterizovaneho cv::Mat do QPixmap
 * @param img   - reprezentacia obrazka v openCV
 * @return
 */
QPixmap Adapter::convertMatToQPixmap(cv::Mat img)
{
    cv::Mat newImg;
    /*  Konvertovanie farieb    */
    cv::cvtColor(img, newImg, CV_BGR2RGB);
    /*  Konvertovanie do QPixmap  */
    return QPixmap::fromImage(QImage(newImg.data, newImg.cols, newImg.rows, newImg.step,
                                     QImage::Format_RGB888));
}
/**
 * @brief Adapter::Adapter
 */
Adapter::Adapter()
{

}
