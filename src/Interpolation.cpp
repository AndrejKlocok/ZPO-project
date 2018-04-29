#include "headers/Interpolation.h"

Interpolation::Interpolation()
{

}
/**
 * @brief Interpolation::NearestNeighbor
 * @param src
 * @param dst
 * @param transformation
 */
void Interpolation::NearestNeighbor(cv::Mat &src, cv::Mat &dst, cv::Mat &transformation, cv::Size size)
{
    float xn, yn;
    cv::Mat T = transformation.inv();
    dst = cv::Mat::zeros(size, src.type());

    for( int x=0; x<size.height;x++){
        for(int y=0; y<size.width; y++){
            xn = x*T.at<float>(0,0) + y*T.at<float>(0,1) + T.at<float>(0,2);
            yn = x*T.at<float>(1,0) + y*T.at<float>(1,1) + T.at<float>(1,2);

            if(xn < 0 || xn >= src.rows || yn < 0 || yn >= src.cols)
                continue;
            dst.at<cv::Vec3b>(x, y) = src.at<cv::Vec3b>(cvRound(xn), cvRound(yn));
        }
    }

}
/**
 * @brief Interpolation::Bilinear
 * @param src
 * @param dst
 * @param transformation
 */
void Interpolation::Bilinear(cv::Mat &src, cv::Mat &dst, cv::Mat &transformation, cv::Size size)
{
    float xn, yn, a, b;
    float R, G, B;
    int l,k;
    cv::Mat T = transformation.inv();
    dst = cv::Mat::zeros(size, src.type());

    for( int x=0; x<size.height;x++){
        for(int y=0; y<size.width; y++){
            xn =x*T.at<float>(0,0) + y*T.at<float>(0,1) + T.at<float>(0,2);
            yn =x*T.at<float>(1,0) + y*T.at<float>(1,1) + T.at<float>(1,2);
            //vypocet hodnot dle vzorce v prednaskach
            l = cvRound(xn);
            k = cvRound(yn);

            a = xn-l;
            b = yn-k;
            // kontrola zda nejsem mimo obraz
            if(xn < 0 || xn >= src.rows || yn < 0 || yn >= src.cols)
                continue;
            //vypocet pro kazdou slozku pixelu
            B = (1-a)*(1-b)*src.at<cv::Vec3b>(l,k).val[0]+a*(1-b)*src.at<cv::Vec3b>(l+1,k).val[0]+b*(1-a)*src.at<cv::Vec3b>(l,k+1).val[0]+a*b*src.at<cv::Vec3b>(l+1,k+1).val[0];
            G = (1-a)*(1-b)*src.at<cv::Vec3b>(l,k).val[1]+a*(1-b)*src.at<cv::Vec3b>(l+1,k).val[1]+b*(1-a)*src.at<cv::Vec3b>(l,k+1).val[1]+a*b*src.at<cv::Vec3b>(l+1,k+1).val[1];
            R = (1-a)*(1-b)*src.at<cv::Vec3b>(l,k).val[2]+a*(1-b)*src.at<cv::Vec3b>(l+1,k).val[2]+b*(1-a)*src.at<cv::Vec3b>(l,k+1).val[2]+a*b*src.at<cv::Vec3b>(l+1,k+1).val[2];

            //normalizace hodnot
            if(B > 255){
                B = 255;
            }
            if(B < 0){
                B = 0;
            }
            if(G > 255){
                G = 255;
            }
            if(G < 0){
                G = 0;
            }
            if(R > 255){
                R = 255;
            }
            if(R < 0){
                R = 0;
            }

            //ulozeni nove hodnoty
            dst.at<cv::Vec3b>(x, y).val[0]=B;
            dst.at<cv::Vec3b>(x, y).val[1]=G;
            dst.at<cv::Vec3b>(x, y).val[2]=R;
        }
    }

}

//vypocet interpolace pro zadane hodnoty
float Interpolation::cubicInterpolateRow (float p0, float p1, float p2, float p3, float x) {
    return p1+0.5*x*(p2-p0+x*(2.0*p0-5.0*p1+4.0*p2-p3+x*(3.0*(p1-p2)+p3-p2)));
}
/**
 * @brief Interpolation::Bicubic
 * @param src
 * @param dst
 * @param transformation
 */
void Interpolation::Bicubic(cv::Mat &src, cv::Mat &dst, cv::Mat &transformation, cv::Size size)
{
    float xn, yn;
    float R, G, B;
    int l,k;
    cv::Mat T = transformation.inv();
    dst = cv::Mat::zeros(size, src.type());

    for( int x=0; x<size.height;x++){
        for(int y=0; y<size.width; y++){
            xn =x*T.at<float>(0,0) + y*T.at<float>(0,1) + T.at<float>(0,2);
            yn =x*T.at<float>(1,0) + y*T.at<float>(1,1) + T.at<float>(1,2);
            //vypocet hodnot dle vzorce v prednaskach
            l = cvRound(xn);
            k = cvRound(yn);
            // pro vypocet interpolace, chci mit hodnoty mezi 0 a 1
            float bicubx = xn - floor(xn);
            float bicuby = yn - floor(yn);

            // kontrola zda nejsem mimo obraz
            if(xn < 0 || xn >= src.rows || yn < 0 || yn >= src.cols)
                continue;
            // interpolace radku, pro slozku blue
            float rb1 = cubicInterpolateRow(src.at<cv::Vec3b>(l-1,k-1).val[0],src.at<cv::Vec3b>(l,k-1).val[0],src.at<cv::Vec3b>(l+1,k-1).val[0],src.at<cv::Vec3b>(l+2,k-1).val[0],bicuby);
            float rb2 = cubicInterpolateRow(src.at<cv::Vec3b>(l-1,k).val[0],src.at<cv::Vec3b>(l,k).val[0],src.at<cv::Vec3b>(l+1,k).val[0],src.at<cv::Vec3b>(l+2,k).val[0],bicuby);
            float rb3 = cubicInterpolateRow(src.at<cv::Vec3b>(l-1,k+1).val[0],src.at<cv::Vec3b>(l,k+1).val[0],src.at<cv::Vec3b>(l+1,k+1).val[0],src.at<cv::Vec3b>(l+2,k+1).val[0],bicuby);
            float rb4 = cubicInterpolateRow(src.at<cv::Vec3b>(l-1,k+2).val[0],src.at<cv::Vec3b>(l,k+2).val[0],src.at<cv::Vec3b>(l+1,k+2).val[0],src.at<cv::Vec3b>(l+2,k+2).val[0],bicuby);
            //interpolace vzniklych sloupcu blue
            float rb = cubicInterpolateRow(rb1,rb2,rb3,rb4,bicubx);
            //interpolace radku, pro slozku green
            float rg1 = cubicInterpolateRow(src.at<cv::Vec3b>(l-1,k-1).val[1],src.at<cv::Vec3b>(l,k-1).val[1],src.at<cv::Vec3b>(l+1,k-1).val[1],src.at<cv::Vec3b>(l+2,k-1).val[1],bicuby);
            float rg2 = cubicInterpolateRow(src.at<cv::Vec3b>(l-1,k).val[1],src.at<cv::Vec3b>(l,k).val[1],src.at<cv::Vec3b>(l+1,k).val[1],src.at<cv::Vec3b>(l+2,k).val[1],bicuby);
            float rg3 = cubicInterpolateRow(src.at<cv::Vec3b>(l-1,k+1).val[1],src.at<cv::Vec3b>(l,k+1).val[1],src.at<cv::Vec3b>(l+1,k+1).val[1],src.at<cv::Vec3b>(l+2,k+1).val[1],bicuby);
            float rg4 = cubicInterpolateRow(src.at<cv::Vec3b>(l-1,k+2).val[1],src.at<cv::Vec3b>(l,k+2).val[1],src.at<cv::Vec3b>(l+1,k+2).val[1],src.at<cv::Vec3b>(l+2,k+2).val[1],bicuby);
            //interpolace vzniklych sloupcu green
            float rg = cubicInterpolateRow(rg1,rg2,rg3,rg4,bicubx);
             //interpolace radku, pro slozku red
            float rr1 = cubicInterpolateRow(src.at<cv::Vec3b>(l-1,k-1).val[2],src.at<cv::Vec3b>(l,k-1).val[2],src.at<cv::Vec3b>(l+1,k-1).val[2],src.at<cv::Vec3b>(l+2,k-1).val[2],bicuby);
            float rr2 = cubicInterpolateRow(src.at<cv::Vec3b>(l-1,k).val[2],src.at<cv::Vec3b>(l,k).val[2],src.at<cv::Vec3b>(l+1,k).val[2],src.at<cv::Vec3b>(l+2,k).val[2],bicuby);
            float rr3 = cubicInterpolateRow(src.at<cv::Vec3b>(l-1,k+1).val[2],src.at<cv::Vec3b>(l,k+1).val[2],src.at<cv::Vec3b>(l+1,k+1).val[2],src.at<cv::Vec3b>(l+2,k+1).val[2],bicuby);
            float rr4 = cubicInterpolateRow(src.at<cv::Vec3b>(l-1,k+2).val[2],src.at<cv::Vec3b>(l,k+2).val[2],src.at<cv::Vec3b>(l+1,k+2).val[2],src.at<cv::Vec3b>(l+2,k+2).val[2],bicuby);
            //interpolace vzniklych sloupcu red
            float rr = cubicInterpolateRow(rr1,rr2,rr3,rr4,bicubx);
            //ulozeni hodnot

            B = rb;
            G = rg;
            R = rr;
            //printf("%f %f %f",B,G,R);
            //normalizace hodnot
            if(B > 255){
                B = 255;
            }
            if(B < 0){
                B = 0;
            }
            if(G > 255){
                G = 255;
            }
            if(G < 0){
                G = 0;
            }
            if(R > 255){
                R = 255;
            }
            if(R < 0){
                R = 0;
            }

            //ulozeni novych hodnot
            dst.at<cv::Vec3b>(x, y).val[0]=B;
            dst.at<cv::Vec3b>(x, y).val[1]=G;
            dst.at<cv::Vec3b>(x, y).val[2]=R;
        }
    }

}

