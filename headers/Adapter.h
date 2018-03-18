#ifndef ADAPTER_H
#define ADAPTER_H

#include <opencv2/imgproc/imgproc.hpp>

#include <QPixmap>

class Adapter
{
public:
   Adapter();
   QPixmap convertMatToQPixmap(cv::Mat img);
};

#endif // ADAPTER_H
