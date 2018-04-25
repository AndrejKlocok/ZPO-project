#include "headers/ImageGraphicsView.h"
#include <QDebug>
#include "headers/MainWindow.h"
/**
 * @brief ImageGraphicsView::ImageGraphicsView
 * @param parent
 */
ImageGraphicsView::ImageGraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
    //scene = new QGraphicsScene();
}
/**
 * @brief ImageGraphicsView::mousePressEvent
 * @param e
 */
void ImageGraphicsView::mousePressEvent(QMouseEvent *e)
{
    QPointF pt =mapToScene(e->pos());
    emit sendMousePoint(pt);
}
