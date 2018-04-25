#include "../headers/MainWindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controler = new MainController();
    this->setUpSingnals();
    this->setScrollBars(false);
    ui->centralWidget->setMouseTracking(true);
}
/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}
/**
 * @brief MainWindow::setUpSingnals
 */
void MainWindow::setUpSingnals()
{
    //onClicks
    connect(ui->btnLoadImg, &QPushButton::clicked, this, (&MainWindow::onLoadImageClick));
    connect(ui->btnSaveImg, &QPushButton::clicked, this, (&MainWindow::onSaveImageClick));
    connect(ui->btnRotateRows, &QPushButton::clicked, this, (&MainWindow::onRotateImageRowsClick));
    connect(ui->btnReload, &QPushButton::clicked, this, (&MainWindow::onReloadBtnClick));
    connect(ui->btnRotatePart, &QPushButton::clicked, this, (&MainWindow::onRotatePartClick));
    connect(ui->btnZoomIn, &QPushButton::clicked, this, (&MainWindow::onZoomInClick));
    connect(ui->btnZoomOut, &QPushButton::clicked, this, (&MainWindow::onZoomOutClick));

    //onValueChanged
    connect(ui->degreeSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValChanged(int)));

    //onMouseClicked
    connect(ui->srcImageView, SIGNAL(sendMousePoint(QPointF)), this, SLOT(setMousePoint(QPointF)));
}
/**
 * @brief MainWindow::displayImage
 * @param img
 */
void MainWindow::displayImage(cv::Mat img, bool setSize)
{
    scene = new QGraphicsScene(this);
    scene->addPixmap(controler->showImage(img));
    ui->srcImageView->setScene(scene);
    if(setSize)
        ui->srcImageView->setFixedSize(QSize(img.cols, img.rows));

    adjustSize();
}
/**
 * @brief MainWindow::onLoadImageClick
 */
void MainWindow::onLoadImageClick()
{
    // choose file
    QString filename = QFileDialog::getOpenFileName(this, tr("Select Image"), "",
                                 tr("Image Files (*.png *.jpg);;All files (*.*)"));
    if(filename.isEmpty()){
        return ;
    }

    //load image
    controler->loadImage(filename);

    //print
    displayImage(controler->getSrcImage(), true);
}
/**
 * @brief MainWindow::onSaveImageClick
 */
void MainWindow::onSaveImageClick()
{
    // choose file
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), "",
                                 tr("Image Files (*.png *.jpg)"));
    if(filename.isEmpty())
        return;
    try{
        controler->saveImage(filename);
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Unable to save file") );
    }
}
/**
 * @brief MainWindow::onRotateImageRowsClick
 */
void MainWindow::onRotateImageRowsClick()
{
    try{
        this->checkZoom();
        if(ui->radioNearest->isChecked()){
            controler->rotateImgShifts(ui->degreeSlider->value(), Interpolation::INTERPOLATIONS::nearest);
        }
        else if(ui->radioBilinear->isChecked()){
            controler->rotateImgShifts(ui->degreeSlider->value(), Interpolation::INTERPOLATIONS::bilinear);
        }
        else {
            controler->rotateImgShifts(ui->degreeSlider->value(), Interpolation::INTERPOLATIONS::bicubic);
        }

        displayImage(controler->getDstImage(), true);
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Unable to rotate image") );
        this->onReloadBtnClick();
    }

}
/**
 * @brief MainWindow::onReloadBtnClick
 */
void MainWindow::onReloadBtnClick()
{
    try{
        this->zoomCnt = 0;
        displayImage(controler->getSrcImage(), true);
        controler->imageReset();
        this->setScrollBars(false);
         ui->labelPointOne->setText("Point One: x: 0 y:0");
         ui->labelPointTwo->setText("Point One: x: 0 y:0");
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Unable to reload image") );
    }

}
/**
 * @brief MainWindow::onRotatePartClick
 */
void MainWindow::onRotatePartClick()
{
    try{
        this->checkZoom();
        if(ui->radioNearest->isChecked()){
            controler->rotatePart(ui->degreeSlider->value(), Interpolation::INTERPOLATIONS::nearest);
        }
        else if(ui->radioBilinear->isChecked()){
            controler->rotatePart(ui->degreeSlider->value(), Interpolation::INTERPOLATIONS::bilinear);
        }
        else {
            controler->rotatePart(ui->degreeSlider->value(), Interpolation::INTERPOLATIONS::bicubic);
        }

        displayImage(controler->getDstImage(), true);
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Mark the area") );
    }
}

void MainWindow::onZoomInClick()
{
    try{
        this->zoomCnt+=1;
        controler->scaleImg(2);
        this->setScrollBars(true);
        displayImage(controler->getDstImage(), false);
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Unable to scale image") );
    }
}

void MainWindow::onZoomOutClick()
{
    try{
        this->zoomCnt-=1;
        controler->scaleImg(0.5);
        this->setScrollBars(true);
        displayImage(controler->getDstImage(), false);
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Unable to scale image") );
    }
}

void MainWindow::setScrollBars(bool value)
{
    if(value){
        ui->srcImageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->srcImageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    else{
        ui->srcImageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->srcImageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

void MainWindow::checkZoom()
{
    if(this->zoomCnt != 0){
        this->onReloadBtnClick();
    }
}
/**
 * @brief MainWindow::onSliderValChanged
 * @param val
 */
void MainWindow::onSliderValChanged(int val)
{
    ui->SliderValue->setText("degrees: "+QString::number(val)+"°");
}
/**
 * @brief MainWindow::setMousePoint
 * @param point
 */
void MainWindow::setMousePoint(QPointF point)
{
    bool pt = controler->getFlipPt();
    if(!pt)
        ui->labelPointOne->setText("Point One: x:"+ QString::number(point.x()) +" y:" + QString::number(point.y()));
    else
        ui->labelPointTwo->setText("Point Two: x:"+ QString::number(point.x()) +" y:" + QString::number(point.y()));

    controler->setPoint(point);
}
/**
 * @brief MainWindow::adjustWindowSize
 */
void MainWindow::adjustWindowSize()
{
    adjustSize();
}
