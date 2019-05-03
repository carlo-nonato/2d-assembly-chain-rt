 #include "graphicsutils.hpp"
 
 void rotateAroundCenter(QGraphicsItem *item, qreal angle) {
    QPointF center = item->mapToParent(item->boundingRect().center());
    item->setRotation(angle);
    QPointF newCenter = item->mapToParent(item->boundingRect().center());
    QPointF offset = center - newCenter;
    item->moveBy(offset.x(), offset.y());
}

bool isColorDark(QColor color) {
    double darkness = 1 - (0.299*color.red() + 0.587*color.green()
                           + 0.114*color.blue())/255;    
    return darkness >= 0.7;
}

QAbstractGraphicsShapeItem *octagonItem(int x, int y, int width, int height,
                                        QGraphicsItem *parent) {
    QRectF rect(x, y, width, height);        

    qreal startBottomX1 = rect.width()/3;
    qreal startBottomY1 = rect.height();

    qreal endBottomX1 = rect.width()/3*2;
    qreal endBottomY1 = rect.height();
    
    qreal startRX2 = rect.width();
    qreal startRY2 = rect.height()/3*2;

    qreal endRX2 = rect.width();
    qreal endRY2 = rect.height()/3;

    qreal startTopX3 = endBottomX1;
    qreal startTopY3 = rect.y();

    qreal endTopX3 = startBottomX1;
    qreal endTopY3 = rect.y();

    qreal startLX4 = endRX2 - rect.width();
    qreal startLY4 = endRY2;

    qreal endLX4 = startRX2 - rect.width();
    qreal endLY4 = startRY2;

    QPainterPath path;
    // Set pen to this point.
    path.moveTo(startBottomX1, startBottomY1);
    // Draw line from pen point to this point.
    path.lineTo(endBottomX1, endBottomY1);
    path.lineTo(startRX2, startRY2);
    path.lineTo(endRX2, endRY2);
    path.lineTo(startTopX3, startTopY3);
    path.lineTo(endTopX3, endTopY3);
    path.lineTo(startLX4, startLY4);
    path.lineTo(endLX4, endLY4);

    QAbstractGraphicsShapeItem *item = new QGraphicsPathItem(path, parent);

    return item;
}