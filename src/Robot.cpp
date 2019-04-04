#include "Robot.hpp"

#include <QEventLoop>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMetaObject>
#include <QPainter>
#include <QPropertyAnimation>

#include <cmath>

#include <QDebug>

const double Robot::DEFAULT_ROTATION_SPEED = 90;

Robot::Robot(const QSizeF &baseSize, const QSizeF &armSize, double startAngle,
             double endAngle, QGraphicsItem *parent) 
: QGraphicsItem(parent),
  m_baseSize(baseSize),
  m_armSize(armSize),
  m_rotationSpeed(Robot::DEFAULT_ROTATION_SPEED),
  m_item(nullptr)
{
    setFlag(QGraphicsItem::ItemHasNoContents);
    
    QGraphicsRectItem *base = new QGraphicsRectItem(
        QRectF(QPointF(0, 0), m_baseSize), this
    );
    base->setPen(Qt::NoPen);
    base->setBrush(Qt::gray);

    m_arm = new QGraphicsRectItem(
        QRectF(QPointF(-10, -m_armSize.height()/2), m_armSize),
        this
    );

    m_arm->setPen(Qt::NoPen);
    m_arm->setBrush(QColor(100, 100, 100));
    m_arm->moveBy(m_baseSize.width()/2, m_baseSize.height()/2);
    m_arm->setRotation(startAngle);

    QGraphicsEllipseItem *pivot = new QGraphicsEllipseItem(0, 0, 5, 5, this);
    pivot->setPen(Qt::NoPen);
    pivot->setBrush(Qt::black);
    pivot->moveBy(m_baseSize.width()/2 - 2.5, m_baseSize.height()/2 - 2.5);

    setStartEndAngle(startAngle, endAngle);
}

QRectF Robot::boundingRect() const {
    return childrenBoundingRect();
}

void Robot::setRotationSpeed(double rotationSpeed) {
    m_rotationSpeed = abs(rotationSpeed);
}

void Robot::setStartEndAngle(double startAngle, double endAngle) {
    m_startAngle = startAngle;
    m_endAngle = endAngle;
}

void Robot::grab() {
    if (m_item != nullptr || !m_executing.tryLock())
        return;
    
    m_item = itemBelowArm();
    // TODO: fix condition (zvalue may not be the best alternative)
    if (m_item != nullptr && m_item->zValue() == 1) {
        QPointF itemPos = m_arm->mapFromItem(m_item, QPointF(0,0));

        m_item->setParentItem(m_arm);
        m_item->setPos(itemPos);
        
        m_item->setFlag(QGraphicsItem::ItemStacksBehindParent);
    }
    else
        m_item = nullptr;
    
    m_executing.unlock();
}

QGraphicsItem *Robot::itemBelowArm() {
    QPointF point(m_arm->rect().right() + 1, m_arm->rect().height()/2);
    for (QGraphicsItem *item : scene()->items(m_arm->mapToScene(point))) {
        if (item != this && item != m_item)
            return item;
    }
    return nullptr;
}

void Robot::release() {
    if (m_item == nullptr || !m_executing.tryLock())
        return;
    
    QGraphicsItem *parent = itemBelowArm();
    if (parent == 0)
        delete m_item;
    else {
        QPointF itemPos = parent->mapFromItem(m_item, QPointF(0, 0));
        m_item->setFlags(m_item->flags() &
                         ~QGraphicsItem::ItemStacksBehindParent);
        m_item->setPos(itemPos);
        m_item->setTransform(
            m_item->transform() *
            QTransform().rotate(-m_arm->rotation() - parent->rotation())
        );
        m_item->setParentItem(parent);
    }
    m_item = nullptr;

    m_executing.unlock();
}

/* Safe to call from non-main thread. By using QMetaObject::invokeMethod,
   the slot will be executed by the main thread and not by the caller one. */
void Robot::rotateToEnd() {
    QMetaObject::invokeMethod(
        this,
        "rotateFromTo",
        Q_ARG(double, armRotation()),
        Q_ARG(double, m_endAngle)
    );
}

void Robot::rotateToStart() {
    QMetaObject::invokeMethod(
        this,
        "rotateFromTo",
        Q_ARG(double, armRotation()),
        Q_ARG(double, m_startAngle)
    );
}

void Robot::rotateFromTo(double from, double to) {
    if(from == to || !m_executing.tryLock())
        return;

    QPropertyAnimation animation(this, "armRotation");
    animation.setDuration(abs(to - from)/m_rotationSpeed*1000);
    animation.setStartValue(from);
    animation.setEndValue(to);

    QEventLoop eventLoop;
    connect(&animation, &QPropertyAnimation::finished,
            &eventLoop, &QEventLoop::quit);

    animation.start();
    eventLoop.exec();

    m_executing.unlock();
}

void Robot::setArmRotation(double rotation) {
    m_arm->setRotation(rotation);
}

void Robot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget) {
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}