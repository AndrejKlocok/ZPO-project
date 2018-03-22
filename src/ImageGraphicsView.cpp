#include "headers/ImageGraphicsView.h"
#include <QDebug>
#include "headers/MainWindow.h"

ImageGraphicsView::ImageGraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
    //scene = new QGraphicsScene();
}

void ImageGraphicsView::mousePressEvent(QMouseEvent *e)
{
    QPointF pt =mapToScene(e->pos());
    emit sendMousePoint(pt);
}
