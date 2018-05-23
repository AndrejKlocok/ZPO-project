#include "../headers/MainWindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <string>
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
    //testovani
    connect(ui->btnCompare, &QPushButton::clicked, this, (&MainWindow::onCompareClick));
    connect(ui->btnCompareGrayScale, &QPushButton::clicked, this, (&MainWindow::onCompareGrayScaleClick));
    connect(ui->btnTest, &QPushButton::clicked, this, (&MainWindow::onTestClick));

    //onValueChanged
    connect(ui->degreeSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValChanged(int)));

    //onMouseClicked
    connect(ui->srcImageView, SIGNAL(sendMousePoint(QPointF)), this, SLOT(setMousePoint(QPointF)));

}
/**
 * @brief MainWindow::displayImage  - Metoda zaistuje zobrazenie obrazku na View v okne.
 * @param img   - obrazok
 */
void MainWindow::displayImage(cv::Mat img, bool setSize,bool GreyScale)
{
    scene = new QGraphicsScene(this);
    scene->addPixmap(controler->showImage(img,GreyScale));
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
    displayImage(controler->getSrcImage(), true,false);
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
        INTERPOLATIONS type;
        ROTATIONS rotate;
        qint64 time;

        /*  Osetrenie priblizeneho obrazka  */
        this->checkZoom();        
        /*  Skontrolovanie moznosti radiobutonov */
        this->checkOptions(&type, &rotate);
        /* Prevedenie rotacie */
        time = controler->rotateImg(ui->degreeSlider->value(), type, rotate);
        /* Zobraz cas */
        this->setTimeLabel(time);
        /* Zobrazenie obrazka */
        displayImage(controler->getDstImage(), true,false);
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
        displayImage(controler->getSrcImage(), true,false);
        /* Resetovanie kontrolera */
        controler->imageReset();
        /* Resetovanie UI   */
        this->setScrollBars(false);
        ui->labelPointOne->setText("Point One: x: 0 y: 0");
        ui->labelPointTwo->setText("Point One: x: 0 y: 0");
        this->setTimeLabel(0);
        this->setDifferenceLabel(0);
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
        INTERPOLATIONS type;
        ROTATIONS rotate;
        qint64 time;

        /*  Osetrenie priblizeneho obrazka  */
        this->checkZoom();
        /*  Skontrolovanie moznosti radiobutonov */
        this->checkOptions(&type, &rotate);
        /* Prevedenie rotacie */
        time = controler->rotatePart(ui->degreeSlider->value(), type, rotate);
        /* Zobraz cas */
        this->setTimeLabel(time);
        /* Zobrazenie obrazka */
        displayImage(controler->getDstImage(), true,false);
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
        displayImage(controler->getDstImage(), false,false);
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
        displayImage(controler->getDstImage(), false,false);
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
void MainWindow::checkOptions(INTERPOLATIONS *type, ROTATIONS *rotations)
{
    /* Zistenie interpolacie */
    if(ui->radioNearest->isChecked()){
        *type = INTERPOLATIONS::nearest;
    }
    else if(ui->radioBilinear->isChecked()){
        *type = INTERPOLATIONS::bilinear;
    }
    else {
        *type = INTERPOLATIONS::bicubic;
    }

    /* Zistenie typu rotacie */
    if(ui->radioRotateNormal->isChecked()){
        *rotations = ROTATIONS::normal;
    }
    else {
        *rotations = ROTATIONS::shear;
    }

}
/**
 * @brief MainWindow::setDifferenceLabel - Zapise vyslednou differenci
 */
void MainWindow::setDifferenceLabel(double differenc)
{
    ui->labelDifference->setText("Difference: " + QString::number(differenc) + " %");
}
/**
 * @brief MainWindow::setTimeLabel - Zapise vysledny cas na label
 * @param time  - cas v milisekundach
 */
void MainWindow::setTimeLabel(qint64 time)
{
    ui->labelTime->setText("Time: " + QString::number(time) + " ms");
}
/**
 * @brief MainWindow::onTestClick   -Testovanie casovej zlozitosti
 */
void MainWindow::onTestClick()
{
    INTERPOLATIONS type;
    ROTATIONS rotate;
    qint64 time;
    std::vector<qint64> times;
    /*  Osetrenie priblizeneho obrazka  */
    this->checkZoom();
    /*  Skontrolovanie moznosti radiobutonov */
    this->checkOptions(&type, &rotate);
    /* Prevedenie rotacie */
    for(int i =0; i<50; i++){
        time = controler->rotateImg(ui->degreeSlider->value(), type, rotate);
        times.push_back(time);
    }
    size_t n = times.size()/2;
    std::nth_element(times.begin(), times.begin() + n, times.end() );
    this->setTimeLabel(times[n]);
}
/**
 * @brief MainWindow::onCompareGrayScaleClick - porovnani vysledku rotace a interpolace s puvodnim obrazem v GrayScale
 */
void MainWindow::onCompareGrayScaleClick()
{
    cv::Mat sourceImg;
    cv::Mat sourceImgGrayScale;
    cv::Mat rotatedImg;
    cv::Mat differenceImg;
    cv::Mat rot_mat( 2, 3, CV_32FC1 );
    cv::Mat cropped;
    cv::Mat croppedGrayScale;
    cv::Point2f middle;

    try{

        INTERPOLATIONS type;
        ROTATIONS rotate;
        qint64 time;
        //nactu si source image
        sourceImg = controler->getSrcImage();
        //vytvori si matici v grayscale formatu
        croppedGrayScale =  cv::Mat::zeros(sourceImg.cols,sourceImg.rows, CV_8UC1);
        middle = cv::Point2f(0.5f*sourceImg.cols, 0.5f*sourceImg.rows);
        //vytvoreni rotacni matice
        rot_mat = getRotationMatrix2D(middle, ui->degreeSlider->value(), 1 );

        /*  Osetrenie priblizeneho obrazka  */
        this->checkZoom();
        /*  Skontrolovanie moznosti radiobutonov */
        this->checkOptions(&type, &rotate);
        //provedeni rotace o uhel a zpetna rotace
        time = controler->rotateImg(ui->degreeSlider->value(), type, rotate);
        time =time + controler->rotateImg(-(ui->degreeSlider->value()), type, rotate);
        /* Zobraz cas */
        this->setTimeLabel(time);
        //oriznuti rotovaneho obrazu, chci se zbavit cernych pixelu
        cv::Rect myROI((controler->getDstImage().cols-sourceImg.cols)/2, (controler->getDstImage().rows-sourceImg.rows)/2, sourceImg.cols, sourceImg.rows);
        cv::Mat croppedRef(controler->getDstImage(), myROI);
        // okopiruji data do nove matice
        croppedRef.copyTo(cropped);
        //prevedu oba obrazy do grayscale
        cv::cvtColor(cropped, croppedGrayScale, CV_RGB2GRAY);
        cv::cvtColor(sourceImg, sourceImgGrayScale, CV_RGB2GRAY);
        //zavolam fci, ktera vypocita rozdil obrazu
        differenceImg = testDiffGrayScale(croppedGrayScale,sourceImgGrayScale);
        //zobrazim tento rozdil
        displayImage(differenceImg, true,true);
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Unable to rotate image") );
        this->onReloadBtnClick();
    }
}

/**
 * @brief MainWindow::onCompareClick - porovnani vysledku rotace a interpolace s puvodnim obrazem
 */
void MainWindow::onCompareClick()
{
    cv::Mat sourceImg;
    cv::Mat rotatedImg;
    cv::Mat differenceImg;
    cv::Mat rot_mat( 2, 3, CV_32FC1 );
    cv::Point2f middle;
    try{
        INTERPOLATIONS type;
        ROTATIONS rotate;
        qint64 time;
        //nactu si source image
        sourceImg = controler->getSrcImage();
        middle = cv::Point2f(0.5f*sourceImg.cols, 0.5f*sourceImg.rows);
        rot_mat = getRotationMatrix2D(middle, ui->degreeSlider->value(), 1 );

        /*  Osetrenie priblizeneho obrazka  */
        this->checkZoom();
        /*  Skontrolovanie moznosti radiobutonov */
        this->checkOptions(&type, &rotate);
        // provedeni rotace o zadany uhel a zpet
        time = controler->rotateImg(ui->degreeSlider->value(), type, rotate);
        time = controler->rotateImg(-(ui->degreeSlider->value()), type, rotate);
        /* Zobraz cas */
        this->setTimeLabel(time);
        //oriznuti obrazu, zbavim se cernych pixelu
        cv::Rect myROI((controler->getDstImage().cols-sourceImg.cols)/2, (controler->getDstImage().rows-sourceImg.rows)/2, sourceImg.cols, sourceImg.rows);
        cv::Mat croppedRef(controler->getDstImage(), myROI);
        cv::Mat cropped;
        //okopirovani noveho orabzu
        croppedRef.copyTo(cropped);
        //vypocet difference obrazu v RGB
        differenceImg = testDiff(cropped,sourceImg);
        //zobrazeni vysledku difference
        displayImage(differenceImg, true,false);
    }
    catch (std::exception e){
        QMessageBox::information(this,tr("Error"), tr("Unable to rotate image") );
        this->onReloadBtnClick();
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
        ui->labelPointOne->setText("Point One: x: "+ QString::number(point.x()) +" y: " + QString::number(point.y()));
    else
        ui->labelPointTwo->setText("Point Two: x: "+ QString::number(point.x()) +" y: " + QString::number(point.y()));

    controler->setPoint(point);
}
/**
 * @brief MainWindow::adjustWindowSize  - Obdnovenie velkosti okna na zaklade velkosti jednotlivych widgetov
 */
void MainWindow::adjustWindowSize()
{
    adjustSize();
}

/**
 * @brief MainWindow::testDiff  - vypocte difference jednotlivych pixelu
 */
cv::Mat MainWindow::testDiff(cv::Mat DstImage,cv::Mat SourceImg)
{
    double diff;
    uchar newPixel;
    //inicializace vysledne matice
    cv::Mat outputMat;
    outputMat =  cv::Mat::zeros(DstImage.rows,DstImage.cols, DstImage.type());
    //pixel po pixelu odecitam odpovidajici hodnoty obrazu
    for( int x=0; x<DstImage.rows;x++){
        for(int y=0; y<DstImage.cols; y++){
            //novy pixel, ulozim si hodnoty, pouziji je pro vypocet difference
            newPixel = abs(DstImage.at<cv::Vec3b>(x,y).val[0] - SourceImg.at<cv::Vec3b>(x,y).val[0]);
            outputMat.at<cv::Vec3b>(x, y).val[0] = newPixel;
            diff += (double)newPixel/255;
            newPixel = abs(DstImage.at<cv::Vec3b>(x,y).val[1] - SourceImg.at<cv::Vec3b>(x,y).val[1]);
            outputMat.at<cv::Vec3b>(x, y).val[1] = newPixel;
            diff += (double)newPixel/255;
            newPixel = abs(DstImage.at<cv::Vec3b>(x,y).val[2] - SourceImg.at<cv::Vec3b>(x,y).val[2]);
            outputMat.at<cv::Vec3b>(x, y).val[2] = newPixel;
            diff += (double)newPixel/255;

        }
    }
    //vypocet difference = soucet vsech absolutnich hodnot rozdilu pixelu/velikost obrazu*3barvy to cele *100(procenta)
    diff = (diff/(DstImage.rows*DstImage.cols*3))*100;
    this->setDifferenceLabel(diff);

    return outputMat;
}
/**
 * @brief MainWindow::testDiffGrayScale  - vypocte difference jednotlivych pixelu GrayScale
 */
cv::Mat MainWindow::testDiffGrayScale(cv::Mat DstImage,cv::Mat SourceImg)
{
    double diff;
    uchar newPixel;
    //inicializace vystupni matice
    cv::Mat outputMat;
    outputMat =  cv::Mat::zeros(DstImage.rows,DstImage.cols, DstImage.type());
    // pixel po pixelu odecitam jedinou slozku grayscale obrazu
    for( int x=0; x<DstImage.rows;x++){
        for(int y=0; y<DstImage.cols; y++){
            newPixel = abs(DstImage.at<uchar>(x,y) - SourceImg.at<uchar>(x,y));
            diff += (double)newPixel/255;
            outputMat.at<uchar>(x, y) = newPixel;

        }
    }
    //vypocet difference = soucet vsech absolutnich hodnot rozdilu pixelu/velikost obrazu to cele *100(procenta)
    diff = (diff/(DstImage.rows*DstImage.cols))*100;
    this->setDifferenceLabel(diff);
    return outputMat;
}

