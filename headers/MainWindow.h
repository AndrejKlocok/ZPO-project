#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGraphicsScene>
#include<QMessageBox>

#include "../headers/MainController.h"
#include "../headers/ImageGraphicsView.h"

namespace Ui {
class MainWindow;
}

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
    void displayImage(cv::Mat img);
    void onLoadImageClick();
    void onSaveImageClick();
    void onRotateImageRowsClick();
    void onReloadBtnClick();
    void onRotatePartClick();

    MainController *controler;
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
