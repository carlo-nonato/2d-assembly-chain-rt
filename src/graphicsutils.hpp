#include <QGraphicsItem>

/** Rotate the item around its center. */
void rotateAroundCenter(QGraphicsItem *item, qreal angle);

/** Calculate luminance of a color and return true if its darkness is at least 70%. */
bool isColorDark(QColor color);

/** Create an octagon */
QAbstractGraphicsShapeItem *octagonItem(int x, int y, int width, int height, QGraphicsItem *parent);