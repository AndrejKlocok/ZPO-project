#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGraphicsScene>
#include<QMessageBox>

#include "../headers/MainController.h"


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

private:
    void setUpSingnals();
    void onLoadImageClick();
    void onSaveImageClick();
    void onRotateImageRowsClick();

    MainController *controler;
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
