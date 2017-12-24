#pragma once

#include <QObject>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>

#include <QSvgRenderer>
#include <QGraphicsSvgItem>

#include <QTimer>

class TuxEyes : public QGraphicsView
{
    Q_OBJECT
private:

    QSvgRenderer *renderer;
    QGraphicsScene *scene;

    QGraphicsLineItem *rightMoveLine;
    QGraphicsItem *rightEyeBoundingRect;
    QGraphicsItem *rightEyeBoundingEllipse;
    QGraphicsItem *rightPupilBoundingEllipse;

    QRectF leftBoundingBox;
    QRectF rightBoundingBox;
    QPointF leftTarget;
    QPointF rightTarget;

    QGraphicsSvgItem *tux;
    QGraphicsSvgItem *beak;
    QGraphicsSvgItem *leftEye;
    QGraphicsSvgItem *rightEye;
    QGraphicsSvgItem *leftPupil;
    QGraphicsSvgItem *rightPupil;
    
    QGraphicsLineItem *leftMoveLine;
    QGraphicsItem *leftEyeBoundingRect;
    QGraphicsItem *leftEyeBoundingEllipse;
    QGraphicsItem *leftPupilBoundingEllipse;

    QTimer timer;

    void initDebug();
    void toogleDebug();
    void computePupilPosition(QPointF &mousePos);

public:
    TuxEyes();
    ~TuxEyes();

public slots:
    void updateEyes();

protected:
    void keyPressEvent(QKeyEvent* event);
};
