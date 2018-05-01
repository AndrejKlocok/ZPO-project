#include "headers/Transformation.h"
#include <QDebug>
Transformation::Transformation()
{
    this->interpolation = new Interpolation();
}
/**
 * @brief Transformation::createRotateShearMatrix - Funkcia vytvori transformacnu maticu rotacie pomocou skosenia
 * @param degree - uhol rotacie
 * @param middl  - stred
 * @return       - matica rotacie pomocou skosenia
 */
cv::Mat Transformation::createRotateShearMatrix(int degree, cv::Point2f middle)
{
    double rad;
    double alpha, beta;

    /* Pouzite matitce */
    cv::Mat M = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat shearX = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat shearY = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat translate = cv::Mat::eye(3, 3, CV_32FC1);


    rad = degree*(CV_PI/180.f);

    /*  Koeficienty */
    alpha = -tan(rad/2.0);
    beta = sin(rad);

    /* Vytvorenie translacnej matice */
    translate.at<float>(0,2) = -middle.y;
    translate.at<float>(1,2) = -middle.x;

    /* Vytvorenie matice skosenia v smere osi x */
    shearX.at<float>(0,1) = alpha;

    /*  Vytvorenie matice skosenia v smere osi y */
    shearY.at<float>(1,0) = beta;

    /*  Aplikovanie transformacnych matic */
    M = translate.inv() * (shearX *(shearY * (shearX * translate)));

    return M;
}
/**
 * @brief Transformation::adjustSize - Funkcia zaisti zobrazenie celeho obrazku a volanie interpolacie
 * @param result    - vysledny obrazok
 * @param M         - rotana matica
 * @param img       - vstupny obrazok
 * @param degree    - uhol
 * @param type      - typ interpolacie
 * @param resized   - bool urcujuci existenciu boudning boxu
 */
void Transformation::adjustSize(cv::Mat &result,cv::Mat &M, cv::Mat &img, int degree, INTERPOLATIONS type, bool resized)
{
    /*  Zobrazenie celeho obrazku pri aplikovani rotacie */
    if(!resized){
        cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), img.size(), degree).boundingRect2f();
        M.at<float>(1,2) += bbox.width/2.0 - img.cols/2.0;
        M.at<float>(0,2) += bbox.height/2.0 - img.rows/2.0;

        cv::Size size = bbox.size();
        this->interpolate(type, img, result, M, size);
    }
    /*  Obrazok je uz zvacsany  */
    else {
        this->interpolate(type, img, result, M, img.size());
    }
}

/**
 * @brief Rotation::rotateShear - Funkcia sluzi na rotovanie obrazka pomocou skosenia
 * @param degree    - uhol
 * @param img       - obrazok
 * @param type      - typ Interpolacie
 * @param middle    - stred rotacie
 * @param resized   - bool urcujuci existenciu boudning boxu
 * @return          - vysledny obrazok
 */
cv::Mat Transformation::rotateShear(int degree, cv::Mat &img, INTERPOLATIONS type, cv::Point2f middle, bool resized)
{
    cv::Mat result, M;

    M = this->createRotateShearMatrix(degree, middle);
    this->adjustSize(result, M, img, degree, type, resized);

    return result;
}
/**
 * @brief Transformation::rotatePart - Funkcia sluzi na rotovanie vybranej casti obrazka danej *rectangle
 * @param degree    - uhol
 * @param img       - obrazok
 * @param type      - typ Interpolacie
 * @param rectangle - vyrez obrazka pre rotaciu
 * @param middle    - stred rotacie
 * @param rotate    - typ rotacie
 * @return          - vysledny obrazok
 */
cv::Mat Transformation::rotatePart(int degree, cv::Mat &img, INTERPOLATIONS type, cv::RotatedRect * rectangle, cv::Point2f * middle, ROTATIONS rotate)
{
    cv::Mat imgTmp, imgSmall, rotated;
    cv::Vec3b blackColor;

    imgTmp = img.clone();
    imgSmall = img.clone();

    /*  Cierna farba    */
    blackColor.val[0] = 0;
    blackColor.val[1] = 0;
    blackColor.val[2] = 0;

    for(int x=0; x<imgTmp.rows; x++)
        for(int y=0; y<imgTmp.cols; y++)
            // orezanie obldznika -> v originale orez obdlznik
            if(rectangle->boundingRect().contains(cv::Point(y, x)))
                imgTmp.at<cv::Vec3b>(x,y) = blackColor;
            // -> v rotovanom vyciernim vsetko okrem obdlznika
            else
                imgSmall.at<cv::Vec3b>(x,y) = blackColor;

    /* Rotacia */
    switch (rotate) {
    case ROTATIONS::normal:
        rotated = this->rotateNormal(degree, imgSmall, type, *middle, true);
        break;
    case ROTATIONS::shear:
        rotated = this->rotateShear(degree, imgSmall, type, *middle, true);
        break;
    default:
        break;
    }

    /* Korekcia */
    for(int x=0; x<rotated.rows; x++)
        for(int y=0; y<rotated.cols; y++)
            // kopirovanie neciernych pixelov
            if(rotated.at<cv::Vec3b>(x,y)!= blackColor)
                imgTmp.at<cv::Vec3b>(x,y) = rotated.at<cv::Vec3b>(x,y);

    return imgTmp;
}
/**
 * @brief Transformation::rotateNormal - Funkcia orotuje obrazok pomocou klasickej rotacnej matice
 * @param degree    - uhol
 * @param img       - obrazok
 * @param type      - typ Interpolacie
 * @param middle    - stred rotacie
 * @param resized   - bool urcujuci existenciu boudning boxu
 * @return          - vysledny obrazok
 */
cv::Mat Transformation::rotateNormal(int degree, cv::Mat &img, INTERPOLATIONS type, cv::Point2f middle, bool resized)
{
    double rad;

    cv::Mat M = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat rotate = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat translate = cv::Mat::eye(3, 3, CV_32FC1);
    cv::Mat result;

    rad = degree*(CV_PI/180.f);

    /* Vytvorenie translacnej matice */
    translate.at<float>(0,2) = -middle.y;
    translate.at<float>(1,2) = -middle.x;

    /* Vytvorenie rotacnej matice */
    rotate.at<float>(0,0) = cos(rad);
    rotate.at<float>(0,1) = sin(rad);
    rotate.at<float>(1,0) = -sin(rad);
    rotate.at<float>(1,1) = cos(rad);


   /* Aplikovanie transformacnych matic */
    M = translate.inv() * (rotate.inv() * translate);

    /*  Oprava velkosti a volanie interpolacie */
    this->adjustSize(result, M, img, degree, type, resized);

    return result;
}
/**
 * @brief Transformation::scale - Funkcia sluzi na zvacsenie obrazka
 * @param times - zoom
 * @param img   - vstupny obrazok
 * @return      - vysledny obrazok
 */
cv::Mat Transformation::scale(float times, cv::Mat &img)
{
    cv::Mat scaleMatrix, scaled;
    cv::Size size;
    scaleMatrix = cv::Mat::eye(2, 3, CV_32FC1);

    scaleMatrix.at<float>(0,0) = times;
    scaleMatrix.at<float>(1,1) = times;

    if(times>1)
        size =  img.size() * 2;
    else
        size = img.size() / 2;

    cv::warpAffine(img, scaled, scaleMatrix, size);
    return scaled;
}
/**
 * @brief Rotation::interpolate - Volanie interpolacnych metod
 * @param type      - typ interpolacie
 * @param img       - vstupny obrazok
 * @param result    - vystupny obrazok
 * @param M         - rotacna matica
 * @param size      - velkost vysledneho obrazka
 */
void Transformation::interpolate(INTERPOLATIONS type, cv::Mat &img, cv::Mat &result, cv::Mat &M, cv::Size size)
{
    /*  Typ interpolacie */
    switch (type) {
    case INTERPOLATIONS::nearest:
        this->interpolation->NearestNeighbor(img, result, M, size);
        break;

    case INTERPOLATIONS::bilinear:
        this->interpolation->Bilinear(img, result, M,size);
        break;

    case INTERPOLATIONS::bicubic:
        this->interpolation->Bicubic(img, result, M, size);
        break;

    default:
        break;
    }

}
