#ifndef CONVEYORBELT_HPP
#define CONVEYORBELT_HPP

#include <QGraphicsRectItem>
#include <QIcon>
#include <QObject>

class ConveyorBelt : public QObject, public QGraphicsRectItem 
{
    Q_OBJECT
    
    Q_PROPERTY (double speed MEMBER m_speed READ speed WRITE setSpeed)

public:
    ConveyorBelt(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    double speed() const { return m_speed; }
    void setTexture(const QIcon &texture);

public slots:
    void setSpeed(double speed);

protected:
    void advance(int step) override;

private:
    double m_level;
    double m_speed;
    QIcon m_texture;
    QPixmap m_tile;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget = nullptr) override;
};

#endif // CONVEYORBELT_HPP
