#include "Simulation.hpp"

#include "ConveyorBelt.hpp"
#include "ItemStack.hpp"
#include "Robot.hpp"

#include <QIcon>
#include <QPainter>
#include <QTimer>

#include <QDebug>

Simulation::Simulation() : rng(dev()) {
    
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
    connect(createTimer, &QTimer::timeout, this, &Simulation::createItem);
    createTimer->start(5000);
}

QImage Simulation::frameFromCamera(int x, int y, int width, int height) {
    QImage frame(width, height, QImage::Format_ARGB32);
    QPainter painter;
    painter.begin(&frame);
    render(&painter, QRectF(), QRectF(x, y, width, height));
    painter.end();
    return frame;
}

/*
    Rotate the item around its center while the transform origin is top left
*/
static void offsetRotation(qreal angle, QGraphicsItem * i) {
    QPointF c = i->mapToScene(i->boundingRect().center());
    i->setRotation(angle);
    QPointF cNew = i->mapToScene((i->boundingRect()).center());
    QPointF offset = c - cNew;
    i->moveBy(offset.x(), offset.y());
}

void Simulation::createItem() {
    // TODO: init these vars in constructor (?)
    // TODO: add other shapes (trinagles, hexagons, ...)

    // Create random number generators
    
    std::uniform_int_distribution<std::mt19937::result_type> distShape(1, 3); // distribution in range [1, 3]

    std::uniform_int_distribution<std::mt19937::result_type> distColor(60, 255); // distribution in range [60, 255]

    std::uniform_int_distribution<std::mt19937::result_type> distAngle(0, 359); // distribution in range [0, 359]

    std::uniform_int_distribution<std::mt19937::result_type> distSize(60, 90); // distribution in range [60, 90]

    QColor col(distColor(rng), distColor(rng), distColor(rng), 255);

    int choice = distShape(rng);
    int angle = distAngle(rng);
    int width = distSize(rng);
    int height = distSize(rng);

    if (choice == 1) {
        QGraphicsRectItem *item = new QGraphicsRectItem(0, 0, width, height,
                                                    m_conveyorBelt);
        item->setPen(Qt::NoPen);
        item->setBrush(col);                  
        
        offsetRotation(-angle, item);

        item->moveBy(100, 50);
        item->setZValue(1);
    } else if (choice == 2) {
        QGraphicsEllipseItem *item = new QGraphicsEllipseItem(0, 0, width, height,
                                                    m_conveyorBelt);
        item->setPen(Qt::NoPen);
        item->setBrush(col);

        offsetRotation(-angle, item); 

        item->moveBy(100, 50);
        item->setZValue(1);
    } else if (choice == 3) {
        //quadrato
        QGraphicsRectItem *item = new QGraphicsRectItem(0, 0, width, width,
                                                    m_conveyorBelt);
        item->setPen(Qt::NoPen);
        item->setBrush(col);

        offsetRotation(-angle, item); 

        item->moveBy(100, 50);
        item->setZValue(1);
    }    
}