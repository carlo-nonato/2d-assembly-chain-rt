#include "Simulation.hpp"

#include "ConveyorBelt.hpp"
#include "cvutils.hpp"
#include "graphicsutils.hpp"
#include "ItemStack.hpp"
#include "randint.hpp"
#include "Robot.hpp"

#include <QIcon>
#include <QPainter>
#include <QTimer>

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
    m_anomalyRobot->moveBy(-40, 150);
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

void Simulation::setItemSizeRange(int minWidth, int minHeight, int maxWidth,
                                  int maxHeight) {
    m_minItemWidth = minWidth;
    m_minItemHeight = minHeight;
    m_maxItemWidth = maxWidth;
    m_maxItemHeight = maxHeight;
}

int Simulation::minItemArea() const {
    // The minimum area is the circle's one -> A = PI*squared(r)
    return M_PI*(pow(m_minItemWidth/2, 2));
}

/* Safe to call from non-main thread.
   By using QMetaObject::invokeMethod the slot will be executed by the main 
   thread and not by the caller one. */
QImage* Simulation::frameFromCamera() {
    QImage* frame; 
    QMetaObject::invokeMethod(
        this,
        "_frameFromCamera",
        Robot::determineConnectionType(),
        Q_RETURN_ARG(QImage*, frame)
    );
    return frame;
}

QImage* Simulation::_frameFromCamera() {
    double x = conveyorBelt()->x() + conveyorBelt()->barsWidth();
    double y = conveyorBelt()->y();
    double width = conveyorBelt()->usableWidth();
    double height = conveyorBelt()->rect().height();
    
    QImage *frame = new QImage(width, height, QImage::Format_ARGB32);
    QPainter painter;
    painter.begin(frame);
    painter.setRenderHint(QPainter::Antialiasing);
    render(&painter, QRectF(), QRectF(x, y, width, height));
    painter.end();
    return frame;
}

void Simulation::createItem() {   
    QAbstractGraphicsShapeItem *item;
    QColor color(randint(60, 255), randint(60, 255), randint(60, 255), 255);
    while (isColorDark(color)) {
        color = QColor(randint(60, 255), randint(60, 255), randint(60, 255), 255);
    }

    int width = randint(m_minItemWidth, m_maxItemWidth);
    int height = randint(m_minItemHeight, m_maxItemHeight);

    int choice = randint(0, 2);

    if (choice == 0)
        item = new QGraphicsRectItem(0, 0, width, height, m_conveyorBelt);
    else if (choice == 1)
        item = new QGraphicsEllipseItem(0, 0, width, height, m_conveyorBelt);
    else if (choice == 2)
        item = octagonItem(0, 0, width, height, m_conveyorBelt);

    item->setPen(Qt::NoPen);
    item->setBrush(color);
    rotateAroundCenter(item, randint(0, 359));
    item->moveBy(100, -60);
    item->setZValue(1);
}