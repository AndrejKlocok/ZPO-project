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
}
/**
 * @brief ImageGraphicsView::mousePressEvent - Event zachitavajuci klik na View
 * @param e - event
 */
void ImageGraphicsView::mousePressEvent(QMouseEvent *e)
{
    QPointF pt =mapToScene(e->pos());
    emit sendMousePoint(pt);
}
