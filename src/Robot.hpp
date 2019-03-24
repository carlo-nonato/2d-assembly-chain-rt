#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <QGraphicsItem>
#include <QMutex>
#include <QObject>

class QGraphicsRectItem;

class Robot : public QObject, public QGraphicsItem 
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    static const double DEFAULT_ROTATION_SPEED;

    Robot(const QSizeF &baseSize, const QSizeF &armSize, double startAngle = 0,
          double endAngle = 180, QGraphicsItem *parent = nullptr);
    
    QRectF boundingRect() const;

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
    
private:
    Q_PROPERTY (double armRotation READ armRotation WRITE setArmRotation)

    QSizeF m_baseSize;
    QSizeF m_armSize;
    QGraphicsRectItem *m_arm;
    double m_rotationSpeed;
    QGraphicsItem *m_item;
    double m_startAngle;
    double m_endAngle;
    QMutex m_executing;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget = nullptr) override;

    double armRotation() const { return m_arm->rotation(); }
    void setArmRotation(double rotation);
    
    QGraphicsItem *itemBelowArm();

private slots:
    void rotateFromTo(double from, double to);
};

#endif // ROBOT_HPP
