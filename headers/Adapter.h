#ifndef ADAPTER_H
#define ADAPTER_H

#include <opencv2/imgproc/imgproc.hpp>

#include <QPixmap>
/**
 * @brief The Adapter class
 */
class Adapter
{
public:
   Adapter();
   QPixmap convertMatToQPixmap(cv::Mat img,bool GreyScale);
};

#endif // ADAPTER_H
