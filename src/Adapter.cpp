#include "../headers/Adapter.h"
/**
 * @brief Adapter::convertMatToQPixmap  - Transformacie obrazku charakterizovaneho cv::Mat do QPixmap
 * @param img   - reprezentacia obrazka v openCV
 * @return
 */
QPixmap Adapter::convertMatToQPixmap(cv::Mat img,bool GreyScale)
{
    cv::Mat newImg;
    //jestlize mame greyscale image, je treba provest konverzi pro QT
    if (GreyScale)
    {
        static QVector<QRgb>  sColorTable;
        //neni nastavena colorTable
        if ( sColorTable.isEmpty() )
        {
            sColorTable.resize( 256 );

            for ( int i = 0; i < 256; ++i )
            {
                sColorTable[i] = qRgb( i, i, i );
            }
        }
        //konverze do QImage
        QImage image( img.data,img.cols, img.rows,static_cast<int>(img.step),QImage::Format_Indexed8 );
        image.setColorTable( sColorTable );

        return QPixmap::fromImage(image);
    }
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
