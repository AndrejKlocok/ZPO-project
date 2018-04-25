#ifndef IMAGEGRAPHICSVIEW_H
#define IMAGEGRAPHICSVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>

/**
 * @brief The ImageGraphicsView class
 */
class ImageGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ImageGraphicsView(QWidget *parent = 0);

signals:
    void sendMousePoint(QPointF point);

public slots:
    void mousePressEvent(QMouseEvent *e);

private:
    QGraphicsScene *scene;

};

#endif // IMAGEGRAPHICSVIEW_H
