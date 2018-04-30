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
 * @brief MainWindow::setUpSingnals - Metoda inicializuje signaly, s ktorymi pracuje okno.
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
 * @brief MainWindow::displayImage  - Metoda zaistuje zobrazenie obrazku na View v okne.
 * @param img   - obrazok
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
 * @brief MainWindow::onLoadImageClick  - Osetrenie eventu na nacitanie obrazka.
 */
void MainWindow::onLoadImageClick()
{
    /*  Zvolenie suboru */
    QString filename = QFileDialog::getOpenFileName(this, tr("Select Image"), "",
                                 tr("Image Files (*.png *.jpg);;All files (*.*)"));
    /*  Osetrenie vstupu */
    if(filename.isEmpty()){
        return ;
    }

    /* Nacitanie obrazka    */
    controler->loadImage(filename);

    /* Zobrazenie obrazka   */
    displayImage(controler->getSrcImage(), true);
}
/**
 * @brief MainWindow::onSaveImageClick  - Osetrenie eventu na ulozenie obrazka.
 */
void MainWindow::onSaveImageClick()
{
    /*  Zvolenie suboru */
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), "",
                                 tr("Image Files (*.png *.jpg)"));
    /*  Osetrenie vstupu */
    if(filename.isEmpty())
        return;

    /* Ulozenie obrazka */
    try{
        controler->saveImage(filename);
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Unable to save file") );
    }
}
/**
 * @brief MainWindow::onRotateImageRowsClick - Osetrenie eventu kliku na rotaciu skosenim
 */
void MainWindow::onRotateImageRowsClick()
{
    try{
        Interpolation::INTERPOLATIONS type;
        Transformation::ROTATIONS rotate;

        /*  Osetrenie priblizeneho obrazka  */
        this->checkZoom();        
        /*  Skontrolovanie moznosti radiobutonov */
        this->checkOptions(&type, &rotate);
        /* Prevedenie rotacie */
        controler->rotateImg(ui->degreeSlider->value(), type, rotate);
        /* Zobrazenie obrazka */
        displayImage(controler->getDstImage(), true);
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Unable to rotate image") );
        this->onReloadBtnClick();
    }

}
/**
 * @brief MainWindow::onReloadBtnClick  - Osetrenie eventu obnovenia obrazka do povodneho stavu
 */
void MainWindow::onReloadBtnClick()
{
    try{
        /*  Zoom    */
        this->zoomCnt = 0;
        /* Zobrazenie povodneho obrazku */
        displayImage(controler->getSrcImage(), true);
        /* Resetovanie kontrolera */
        controler->imageReset();
        /* Resetovanie UI   */
        this->setScrollBars(false);
         ui->labelPointOne->setText("Point One: x: 0 y:0");
         ui->labelPointTwo->setText("Point One: x: 0 y:0");
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Unable to reload image") );
    }

}
/**
 * @brief MainWindow::onRotatePartClick - Osetrenie eventu kliku na ciastocnu rotaciu
 */
void MainWindow::onRotatePartClick()
{
    try{
        Interpolation::INTERPOLATIONS type;
        Transformation::ROTATIONS rotate;

        /*  Osetrenie priblizeneho obrazka  */
        this->checkZoom();
        /*  Skontrolovanie moznosti radiobutonov */
        this->checkOptions(&type, &rotate);
        /* Prevedenie rotacie */
        controler->rotatePart(ui->degreeSlider->value(), type, rotate);
        /* Zobrazenie obrazka */
        displayImage(controler->getDstImage(), true);
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Mark the area") );
    }
}
/**
 * @brief MainWindow::onZoomInClick - Priblizenie obrazka
 */
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
/**
 * @brief MainWindow::onZoomOutClick - Oddialenie obrazka
 */
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
/**
 * @brief MainWindow::setScrollBars - Upravenie scroll barov
 * @param value - boolean, na zaklade ktoreho sa scrollbary zobrazia
 */
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
/**
 * @brief MainWindow::checkZoom - Osetrenie priblizenia
 */
void MainWindow::checkZoom()
{
    if(this->zoomCnt != 0){
        this->onReloadBtnClick();
    }
}
/**
 * @brief MainWindow::checkOptions  - Zistenie moznosti z poli radiobutonov
 * @param type      - typ interpolacie
 * @param rotations - typ rotacie
 */
void MainWindow::checkOptions(Interpolation::INTERPOLATIONS *type, Transformation::ROTATIONS *rotations)
{
    /* Zistenie interpolacie */
    if(ui->radioNearest->isChecked()){
        *type = Interpolation::INTERPOLATIONS::nearest;
    }
    else if(ui->radioBilinear->isChecked()){
        *type = Interpolation::INTERPOLATIONS::bilinear;
    }
    else {
        *type = Interpolation::INTERPOLATIONS::bicubic;
    }

    /* Zistenie typu rotacie */
    if(ui->radioRotateNormal->isChecked()){
        *rotations = Transformation::ROTATIONS::normal;
    }
    else {
        *rotations = Transformation::ROTATIONS::shear;
    }

}
/**
 * @brief MainWindow::onSliderValChanged - Zapisanie hodnoty slideru do navestia
 * @param val   - hodnota
 */
void MainWindow::onSliderValChanged(int val)
{
    ui->SliderValue->setText("degrees: "+QString::number(val)+"°");
}
/**
 * @brief MainWindow::setMousePoint - Event kliku na obrazok
 * @param point - suradnice bodu kliku na obrazok
 */
void MainWindow::setMousePoint(QPointF point)
{
    bool pt = controler->getFlipPt();

    /* Jeden bod z mysleneho obdlznika */
    if(!pt)
        ui->labelPointOne->setText("Point One: x:"+ QString::number(point.x()) +" y:" + QString::number(point.y()));
    else
        ui->labelPointTwo->setText("Point Two: x:"+ QString::number(point.x()) +" y:" + QString::number(point.y()));

    controler->setPoint(point);
}
/**
 * @brief MainWindow::adjustWindowSize  - Obdnovenie velkosti okna na zaklade velkosti jednotlivych widgetov
 */
void MainWindow::adjustWindowSize()
{
    adjustSize();
}
