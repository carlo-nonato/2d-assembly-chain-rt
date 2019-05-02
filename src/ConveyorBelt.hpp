#ifndef CONVEYORBELT_HPP
#define CONVEYORBELT_HPP

#include <QGraphicsRectItem>
#include <QIcon>
#include <QObject>

class ConveyorBelt : public QObject, public QGraphicsRectItem 
{
    Q_OBJECT
    
    Q_PROPERTY(double speed MEMBER m_speed READ speed WRITE setSpeed)

public:
    ConveyorBelt(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    int barsWidth() const { return m_barsWidth; }
    void setBarsWidth(int barsWidth);

    double speed() const { return m_speed; }
    void setTexture(const QIcon &texture);

    double usableWidth() const { return rect().width() - 2*m_barsWidth; }

public slots:
    void setSpeed(double speed);

protected:
    void advance(int step) override;

private:
    int m_barsWidth = 10;
    double m_level = 0;
    double m_speed = 2;
    QIcon m_texture;
    QPixmap m_tile;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget = nullptr) override;
};

#endif // CONVEYORBELT_HPP
