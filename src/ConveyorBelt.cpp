#include "ConveyorBelt.hpp"

#include <QPainter>

ConveyorBelt::ConveyorBelt(const QRectF &rect, QGraphicsItem *parent) 
: QGraphicsRectItem(rect, parent) {}

void ConveyorBelt::setBarsWidth(int barsWidth) {
    m_barsWidth = barsWidth;
}

void ConveyorBelt::setSpeed(double speed) {
    m_speed = speed;
}

void ConveyorBelt::setTexture(const QIcon &texture) {
    m_texture = texture;
    m_tile = m_texture.pixmap(rect().size().toSize());
}

void ConveyorBelt::advance(int step) {
    if (!step || m_speed == 0)
        return;

    m_level = m_level + m_speed;
    if (m_level >= m_tile.height())
        m_level -= m_tile.height();

    for (QGraphicsItem *child : childItems()) {
        child->moveBy(0, m_speed);

        QPointF centerInParent = mapFromItem(child,
                                             child->boundingRect().center());
        if (centerInParent.y() > rect().height())
            delete child;
    }

    update();
}

void ConveyorBelt::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawTiledPixmap(rect(), m_tile, QPointF(0, -m_level));

    const QBrush oldBrush = painter->brush();
    const QPen oldPen = painter->pen();

    painter->setBrush(Qt::white);
    painter->setPen(Qt::NoPen);
    painter->drawRect(0, 0, m_barsWidth, rect().height());
    painter->drawRect(rect().width() - m_barsWidth, 0,
                      m_barsWidth, rect().height());

    painter->setBrush(oldBrush);
    painter->setPen(oldPen);
}