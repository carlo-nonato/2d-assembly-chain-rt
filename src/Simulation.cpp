#include "Simulation.hpp"

#include "ConveyorBelt.hpp"
#include "ItemStack.hpp"
#include "Robot.hpp"

#include <QIcon>
#include <QPainter>
#include <QTimer>

Simulation::Simulation() {
    //QT stuff
    setBackgroundBrush(Qt::black);

    //create new conveyor belt from texture
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
}

void Simulation::createItem() {
    QGraphicsRectItem *item = new QGraphicsRectItem(0, 0, 50, 70,
                                                    m_conveyorBelt);
    item->setPen(Qt::NoPen);
    item->setBrush(Qt::red);
    item->setRotation(-90);
    item->moveBy(100, 50);
    item->setZValue(1);
}

//move conveyor belt
void Simulation::start(const int speed) {    
    QTimer *timer = new QTimer();
    //advance: inherited method from QGraphicsScene (needed for moving animation)
    //every 1000/33 advance is called -> advance simply call advance method on every child class
    //Simulation::advance() -> ConveyotBelt::advance()
    //                      -> ItemStack::advance()


    connect(timer, &QTimer::timeout, this, &Simulation::advance, Qt::ConnectionType(Qt::QueuedConnection));
    //about 30 fps movement
    timer->start(1000/33);
    
    //start conveyor belt
    m_conveyorBelt->setSpeed(speed);
}

QImage Simulation::frameFromCamera(int left, int top) {
    QImage frame(250, 200, QImage::Format_ARGB32);
    QPainter painter;
    painter.begin(&frame);
    render(&painter, QRectF(), QRectF(left, top, 250, 200));
    painter.end();
    return frame;
}