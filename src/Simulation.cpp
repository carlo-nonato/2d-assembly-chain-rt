#include "Simulation.hpp"

#include "ConveyorBelt.hpp"
#include "CVUtils.hpp"
#include "ItemStack.hpp"
#include "randint.hpp"
#include "Robot.hpp"

#include <QIcon>
#include <QPainter>
#include <QTimer>
#include <cassert>

#include <QDebug>

Simulation::Simulation() {
    setBackgroundBrush(Qt::black);

    m_conveyorBelt = new ConveyorBelt(QRectF(0, 0, 300, 600));
    m_conveyorBelt->setTexture(QIcon(":/textures/belt.svg"));
    addItem(m_conveyorBelt);

    QGraphicsRectItem *producer = addRect(
        -20, -60, m_conveyorBelt->rect().width() + 40, 60, Qt::NoPen, Qt::gray
    );
    producer->setZValue(1);

    m_anomalyRobot = new Robot(QSizeF(40, 60), QSizeF(160, 20));
    addItem(m_anomalyRobot);
    m_anomalyRobot->moveBy(-40, 200);
    m_anomalyRobot->setZValue(2);

    m_stackingRobot = new Robot(QSizeF(40, 60), QSizeF(160, 20), 180, 0);
    addItem(m_stackingRobot);
    m_stackingRobot->moveBy(-40, 400);
    m_stackingRobot->setZValue(2);

    ItemStack *itemStack = new ItemStack(QRectF(0, 0, 60, 60));
    itemStack->setPen(Qt::NoPen);
    itemStack->setBrush(Qt::gray);
    addItem(itemStack);
    itemStack->moveBy(-200, 400);

    setSceneRect(itemsBoundingRect());

    QTimer *advanceTimer = new QTimer();
    connect(advanceTimer, &QTimer::timeout, this, &Simulation::advance);
    advanceTimer->start(1000/30); // a new frame every 33.3 ms (30 FPS)

    QTimer *createTimer = new QTimer();
    connect(createTimer,  &QTimer::timeout, this, [this]{ createItem(); });
    createTimer->start(5000);
}

/* Safe to call from non-main thread. By using QMetaObject::invokeMethod,
   the slot will be executed by the main thread and not by the caller one. */
QImage* Simulation::frameFromCamera(int x, int y, int width, int height) {
    QImage* frame; 
    QMetaObject::invokeMethod(
        this,
        "_frameFromCamera",
        Robot::determineConnectionType(),
        Q_RETURN_ARG(QImage*, frame),
        Q_ARG(int, x),
        Q_ARG(int, y),
        Q_ARG(int, width),
        Q_ARG(int, height)
    );
    return frame;
}

QImage* Simulation::_frameFromCamera(int x, int y, int width, int height) {
    QImage *frame = new QImage(width, height, QImage::Format_ARGB32);
    QPainter painter;
    painter.begin(frame);
    painter.setRenderHint(QPainter::Antialiasing);
    render(&painter, QRectF(), QRectF(x, y, width, height));
    painter.end();
    return frame;
}

void Simulation::createItem(int minWidth, int minHeight, int maxWidth, int maxHeight) {
    assert(minWidth >= 60 && minHeight >= 60 && maxWidth >=60 && maxHeight >= 60
        && minWidth <= maxWidth && minHeight <= maxHeight);
    
    QAbstractGraphicsShapeItem *item;
    QColor color(randint(60, 255), randint(60, 255), randint(60, 255), 255);

    int width = randint(minWidth, maxWidth);
    int height = randint(minHeight, maxHeight);
    
    // The minimum calculable area is the circle's one
    // A = PI * squared(r)
    m_minItemArea = M_PI * (minWidth/2 * minWidth/2);    

    if (randint(0, 1) == 0)
        item = new QGraphicsRectItem(0, 0, width, height, m_conveyorBelt);
    else
        item = new QGraphicsEllipseItem(0, 0, width, height, m_conveyorBelt);

    item->setPen(Qt::NoPen);
    item->setBrush(color);
    rotateAroundCenter(item, randint(0, 359));
    item->moveBy(100, -60);
    item->setZValue(1);
}

void Simulation::rotateAroundCenter(QGraphicsItem *item, qreal angle) {
    QPointF center = item->mapToParent(item->boundingRect().center());
    item->setRotation(angle);
    QPointF newCenter = item->mapToParent(item->boundingRect().center());
    QPointF offset = center - newCenter;
    item->moveBy(offset.x(), offset.y());
}