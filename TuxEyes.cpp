#include "TuxEyes.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QCursor>

#include <QPen>
#include <QColor>

#include <QDebug>

TuxEyes::TuxEyes()
{
    unsigned int size = 200;

    resize(size, size);

    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background:transparent;border-style:none;");
    setMouseTracking(true);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene = new QGraphicsScene();

    renderer = new QSvgRenderer(QLatin1String(":/svg/tux.svg"));
    tux = new QGraphicsSvgItem();
    beak = new QGraphicsSvgItem();
    leftPupil = new QGraphicsSvgItem();
    rightPupil = new QGraphicsSvgItem();
    leftEye = new QGraphicsSvgItem();
    rightEye = new QGraphicsSvgItem();

    tux->setSharedRenderer(renderer);
    beak->setSharedRenderer(renderer);
    leftPupil->setSharedRenderer(renderer);
    rightPupil->setSharedRenderer(renderer);
    leftEye->setSharedRenderer(renderer);
    rightEye->setSharedRenderer(renderer);

    tux->setElementId("tux");
    beak->setElementId("beak");
    leftPupil->setElementId("leftPupil");
    rightPupil->setElementId("rightPupil");
    leftEye->setElementId("leftEye");
    rightEye->setElementId("rightEye");

    beak->setZValue(2);

    scene->addItem(tux);
    scene->addItem(beak);
    scene->addItem(leftPupil);
    scene->addItem(rightPupil);

    leftMoveLine = scene->addLine(QLineF(leftBoundingBox.center(), QPointF(0,0)));
    rightMoveLine = scene->addLine(QLineF(rightBoundingBox.center(), QPointF(0, 0)));

    setScene(scene);

    QPointF center = tux->boundingRect().center();

    float x = center.x();
    float y = center.y();

    leftBoundingBox.setX(0);
    leftBoundingBox.setY(0);
    leftBoundingBox.setWidth(leftEye->boundingRect().width());
    leftBoundingBox.setHeight(leftEye->boundingRect().height());

    rightBoundingBox.setX(0);
    rightBoundingBox.setY(0);
    rightBoundingBox.setWidth(rightEye->boundingRect().width());
    rightBoundingBox.setHeight(rightEye->boundingRect().height());

    leftBoundingBox.translate(x - leftEye->boundingRect().width(),
                              y - leftEye->boundingRect().height()*1.3);

    rightBoundingBox.translate(x,
                               y - rightEye->boundingRect().height()*1.65);

    initDebug();

    timer.start(80);
    connect(&timer, &QTimer::timeout, this, &TuxEyes::updateEyes);

    toogleDebug();
}

TuxEyes::~TuxEyes()
{
    delete renderer;
    delete scene;
    //delete tux;
    //delete beak;
    //delete rightPupil;
    //delete leftPupil;
    delete leftEye;
    delete rightEye;
}

/*
 * drawing code ...
 */

void TuxEyes::initDebug()
{
    QPen pen;
    pen.setColor(QColor(255, 0, 0));
    pen.setWidth(3);

    QRectF lPupil = leftPupil->boundingRect();
    lPupil.translate(leftBoundingBox.center().x()
                     - leftPupil->boundingRect().width()*0.5,
                     leftBoundingBox.center().y()
                     - leftPupil->boundingRect().height()*0.5);

    leftEyeBoundingRect = scene->addRect(leftBoundingBox, pen);
    leftEyeBoundingRect->setZValue(2);

    leftEyeBoundingEllipse = scene->addEllipse(leftBoundingBox, pen);
    leftEyeBoundingEllipse->setZValue(2);

    leftPupilBoundingEllipse = scene->addEllipse(lPupil, pen);
    leftPupilBoundingEllipse->setZValue(2);

    leftMoveLine->setPen(pen);

    QRectF rPupil = rightPupil->boundingRect();
    rPupil.translate(rightBoundingBox.center().x()
                     - rightPupil->boundingRect().width()*0.5,
                     rightBoundingBox.center().y()
                     - rightPupil->boundingRect().height()*0.5);



    rightEyeBoundingRect = scene->addRect(rightBoundingBox, pen);
    rightEyeBoundingRect->setZValue(2);

    rightEyeBoundingEllipse = scene->addEllipse(rightBoundingBox, pen);
    rightEyeBoundingEllipse->setZValue(2);

    rightPupilBoundingEllipse = scene->addEllipse(rPupil, pen);
    rightPupilBoundingEllipse->setZValue(2);

    rightMoveLine->setPen(pen);
}

void TuxEyes::computePupilPosition(QPointF &mousePos)
{
    QVector2D leftToMouse1(mousePos - leftBoundingBox.center());

    QVector2D leftToMouse(mousePos - leftBoundingBox.center());
    leftToMouse.normalize();
    leftToMouse.setX(leftToMouse.x()*(leftBoundingBox.width() -
                                      2.3*leftPupil->boundingRect().width()));
    leftToMouse.setY(leftToMouse.y()*(leftBoundingBox.height() -
                                      2.2*leftPupil->boundingRect().height()));

    if(leftToMouse1.length() > leftToMouse.length())
        leftTarget = leftToMouse.toPointF() + leftBoundingBox.center();
    else leftTarget = leftToMouse1.toPointF() + leftBoundingBox.center();

    QVector2D rightToMouse1(mousePos - rightBoundingBox.center());

    QVector2D rightToMouse(mousePos - rightBoundingBox.center());
    rightToMouse.normalize();
    rightToMouse.setX(rightToMouse.x()*(rightBoundingBox.width() -
                                        2.6*rightPupil->boundingRect().width()));
    rightToMouse.setY(rightToMouse.y()*(rightBoundingBox.height() -
                                        1.6*rightPupil->boundingRect().height()));

    if(rightToMouse1.length() > rightToMouse.length())
        rightTarget = rightToMouse.toPointF() + rightBoundingBox.center();
    else rightTarget = rightToMouse1.toPointF() + rightBoundingBox.center();
}

void TuxEyes::toogleDebug()
{
    Qt::WindowFlags windowPropperties =
            Qt::Dialog | Qt::Tool | Qt::WindowStaysOnBottomHint;

    if(leftMoveLine->isVisible())
    {
        leftMoveLine->hide();
        leftEyeBoundingRect->hide();
        leftEyeBoundingEllipse->hide();
        leftPupilBoundingEllipse->hide();

        rightMoveLine->hide();
        rightEyeBoundingRect->hide();
        rightEyeBoundingEllipse->hide();
        rightPupilBoundingEllipse->hide();

        windowPropperties = windowPropperties | Qt::FramelessWindowHint;
    }
    else
    {
        leftMoveLine->show();
        leftEyeBoundingRect->show();
        leftEyeBoundingEllipse->show();
        leftPupilBoundingEllipse->show();

        rightMoveLine->show();
        rightEyeBoundingRect->show();
        rightEyeBoundingEllipse->show();
        rightPupilBoundingEllipse->show();
    }

    setWindowFlags(windowPropperties);

    show();

    setGeometry(QApplication::desktop()->geometry().width() - geometry().width(),
                QApplication::desktop()->geometry().height() - geometry().height(),
                geometry().width(),
                geometry().height());
}

void TuxEyes::updateEyes()
{
    QPointF mousePos = mapToScene(mapFromGlobal(QCursor::pos()));

    computePupilPosition(mousePos);

    leftPupil->setPos(leftTarget - leftPupil->boundingRect().center());
    rightPupil->setPos(rightTarget - rightPupil->boundingRect().center());

    leftMoveLine->setLine(QLineF(leftBoundingBox.center(), leftTarget));
    rightMoveLine->setLine(QLineF(rightBoundingBox.center(), rightTarget));

    fitInView(tux);
}

void TuxEyes::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_M)
    {
        toogleDebug();
    }
}
