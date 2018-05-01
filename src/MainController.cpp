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
/**
 * @brief MainController::setPoint  - Metoda prideluje bod obdlznikoveho vyrezu na zaklade bool flipP
 * @param value - aktualne bod
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

    /* Vytvorenie vyrezu    */
    this->rectangle = cv::RotatedRect(middle, rec.size(), 0);
    this->flipPoints();
}
/**
 * @brief MainController::loadImage - Nacitanie obrazka.
 * @param path  - cesta k obrazku
 */
void MainController::loadImage(QString path)
{
    this->srcImage = cv::imread(path.toStdString());
    this->dstImage = this->srcImage.clone();
    this->resized = false;
}
/**
 * @brief MainController::saveImage - Ulozenie obrazka
 * @param path  - cesta k obrazku
 */
void MainController::saveImage(QString path)
{
    cv::imwrite(path.toStdString(),this->dstImage);
}
/**
 * @brief MainController::rotateImg - Metoda sluzi na rotovanie obrazka
 * @param degree    - uhol rotacie
 * @param type      - typ interpolacie
 * @param rotate    - typ rotacie
 */
qint64 MainController::rotateImg(int degree, INTERPOLATIONS type, ROTATIONS rotate)
{
    QElapsedTimer timer;
    cv::Mat img;
    img = this->getDstImage();

    /*  Vytvorenie stredu rotacie   */
    cv::Point2f middle = cv::Point2f(0.5f*img.cols, 0.5f*img.rows);
    /* Spustime casovac */
    timer.start();

    /*  Na zaklade typu rotacie volam metodu z moznosti */
    switch (rotate) {
    case ROTATIONS::normal:
        this->dstImage = transformation->rotateNormal(degree, img, type, middle, this->resized);
        break;
    case ROTATIONS::shear:
        this->dstImage = transformation->rotateShear(degree, img, type, middle, this->resized);
        break;
    default:
        break;
    }
    /*  Aplikovane zvacsenia    */
    this->resized = true;

    /* Posleme vysledny cas */
    return timer.elapsed();
}
/**
 * @brief MainController::flipPoints - Otocenie bodov reprezentujucich obdlznikovy vyrez
 */
void MainController::flipPoints()
{
    this->flipPt= !flipPt;
}
/**
 * @brief MainController::rotatePart - Metoda rotuje vyrez urceny obldznikom.
 * @param degree    - uhol rotacie
 * @param type      - typ interpolacie
 * @param rotate    - typ rotacie
 */
qint64 MainController::rotatePart(int degree, INTERPOLATIONS type, ROTATIONS rotate)
{
    QElapsedTimer timer;
    cv::Mat img;
    img = this->getDstImage();
    /*  Zistenie sterdu */
    cv::Point2f middle = cv::Point2f(abs(secondPoint.x() - firstPoint.x())/2 + firstPoint.x(), abs(secondPoint.y() - firstPoint.y())/2 + firstPoint.y());

    /* Spustime casovac */
    timer.start();
    /*  Rotacia */
    this->dstImage = transformation->rotatePart(degree, img, type, &rectangle, &middle, rotate);

    /* Uprava vyrezu    */
    cv::Rect rec(firstPoint.y(), firstPoint.x(), abs(secondPoint.y() - firstPoint.y()) + 1, abs(secondPoint.x() - firstPoint.x())+ 1);
    this->rectangle = cv::RotatedRect(middle, rec.size(), degree);

    /* Posleme vysledny cas */
    return timer.elapsed();
}
/**
 * @brief MainController::imageReset - Obdnovenie obrazka do povodneho stavu.
 */
void MainController::imageReset()
{
    this->firstPoint = QPointF(0,0);
    this->secondPoint = QPointF(0,0);
    this->dstImage = srcImage.clone();
    this->resized = false;
}
/**
 * @brief MainController::scaleImg  - Skalovanie obrazka
 * @param times - nasobnost
 */
void MainController::scaleImg(float times)
{
    cv::Mat img;
    img = this->getDstImage();
    /*  Volanie prislusnej transformacie*/
    this->dstImage = transformation->scale(times, img);
    /*  Korekcia    */
    if(times > 1.0)
        this->resized = true;
    else
        this->resized = false;
}
