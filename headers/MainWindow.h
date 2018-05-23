#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGraphicsScene>
#include<QMessageBox>

#include "../headers/MainController.h"
#include "../headers/ImageGraphicsView.h"
#include <string>
namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onSliderValChanged(int val);
    void setMousePoint(QPointF point);
    void adjustWindowSize();

private:
    void setUpSingnals();
    void displayImage(cv::Mat img, bool setSize,bool GreyScale);
    void onLoadImageClick();
    void onSaveImageClick();
    void onRotateImageRowsClick();
    void onReloadBtnClick();
    void onRotatePartClick();
    void onZoomInClick();
    void onZoomOutClick();
    void setScrollBars(bool value);
    void checkZoom();
    void checkOptions(INTERPOLATIONS *type, ROTATIONS *rotations);
    void setTimeLabel(qint64 time);
    void setDifferenceLabel(double difference);
    void onTestClick();
    void onCompareClick();
    void onCompareGrayScaleClick();
    cv::Mat testDiff(cv::Mat DstImage,cv::Mat SourceImg);
    cv::Mat testDiffGrayScale(cv::Mat DstImage,cv::Mat SourceImg);
    cv::Mat toGrayScale(cv::Mat SourceImg);

    MainController *controler;
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    int zoomCnt = 0;
};

#endif // MAINWINDOW_H
