 #include "graphicsutils.hpp"
 
 void rotateAroundCenter(QGraphicsItem *item, qreal angle) {
    QPointF center = item->mapToParent(item->boundingRect().center());
    item->setRotation(angle);
    QPointF newCenter = item->mapToParent(item->boundingRect().center());
    QPointF offset = center - newCenter;
    item->moveBy(offset.x(), offset.y());
}