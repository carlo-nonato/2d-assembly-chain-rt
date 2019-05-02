#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <QGraphicsItem>
#include <QMutex>
#include <QObject>

class QGraphicsRectItem;

/** The robot class represents a robot that can grab an item, rotate, and
 *  release it. It can also rotate itself over its pivot, so it can be used for
 *  any task that involves moving items. The item rotation happens while the
 *  robot's arm is moving. */
class Robot : public QObject, public QGraphicsItem 
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    static Qt::ConnectionType determineConnectionType();

    Robot(const QSizeF &baseSize, const QSizeF &armSize, double startAngle = 0,
          double endAngle = 180, QGraphicsItem *parent = nullptr);
    
    QRectF boundingRect() const override;

    QSizeF armSize() const { return m_armSize; }

    double rotationSpeed() const { return m_rotationSpeed; }
    void setRotationSpeed(double rotationSpeed);
    
    double startAngle() const { return m_startAngle; }
    double endAngle() const { return m_endAngle; }
    void setStartEndAngle(double startAngle, double endAngle);

public slots:
    void grab();
    void release();
    void rotateToEnd();
    void rotateToStart();
    void scheduleItemRotation(double angle);
    
private:
    Q_PROPERTY(double armRotation READ armRotation WRITE setArmRotation)
    Q_PROPERTY(double itemRotation READ itemRotation WRITE setItemRotation)

    QGraphicsItem *m_item = nullptr;
    double m_scheduledItemRotation = 0;
    double m_rotationSpeed = 260;
    QGraphicsRectItem *m_arm;
    QSizeF m_baseSize;
    QSizeF m_armSize;
    double m_endAngle;
    QMutex m_executing;
    double m_startAngle;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget = nullptr) override;

    double armRotation() const { return m_arm->rotation(); }
    void setArmRotation(double rotation);

    double itemRotation() const;
    void setItemRotation(double angle);

    QGraphicsItem *itemBelowArm();

private slots:
    void rotateFromTo(double from, double to);
    void _grab();
    void _release();
};

#endif // ROBOT_HPP