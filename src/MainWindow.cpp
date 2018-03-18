#include "../headers/MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->controler = new MainController();
    this->setUpSingnals();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpSingnals()
{
    connect(ui->btnLoadImg, &QPushButton::clicked, this, (&MainWindow::onLoadImageClick));
    connect(ui->btnSaveImg, &QPushButton::clicked, this, (&MainWindow::onSaveImageClick));
    connect(ui->btnRotateRows, &QPushButton::clicked, this, (&MainWindow::onRotateImageRowsClick));
    connect(ui->degreeSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValChanged(int)));
}

void MainWindow::onLoadImageClick()
{
    // choose file
    QString filename = QFileDialog::getOpenFileName(this, tr("Select Image"), "",
                                 tr("Image Files (*.png)"));
    if(filename.isEmpty()){
        return ;
    }

    //load image
    this->controler->loadImage(filename);

    //print
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addPixmap(controler->showImage(controler->getSrcImage()));
    ui->srcImageView->setScene(scene); //setPixmap(controler->showImage());
}

void MainWindow::onSaveImageClick()
{
    // choose file
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), "",
                                 tr("Image Files (*.png)"));
    try{
        this->controler->saveImage(filename);
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Unable to save file") );
    }
}

void MainWindow::onRotateImageRowsClick()
{
    try{
        this->controler->rotateImgRows(ui->degreeSlider->value());

        QGraphicsScene *scene = new QGraphicsScene(this);
        scene->addPixmap(controler->showImage(controler->getDstImage()));
        ui->dstImageView->setScene(scene); //setPixmap(controler->showImage());
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Unable to rotate image") );
    }

}

void MainWindow::onSliderValChanged(int val)
{
    ui->SliderValue->setText(QString::number(val)+"°");
}
