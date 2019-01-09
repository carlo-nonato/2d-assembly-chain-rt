#include "ItemStack.hpp"

#include <QPen>
#include <QBrush>

ItemStack::ItemStack(const QRectF &rect, QGraphicsItem *parent)
: QGraphicsRectItem(rect, parent)
{
    createItem();
}

void ItemStack::advance(int step) {
    if (step && childItems().count() == 0)
        createItem();
}

void ItemStack::createItem() {
    QGraphicsRectItem *item = new QGraphicsRectItem(
        QRectF(QPointF(0, 0), rect().size() - QSize(20, 20)),
        this
    );
    item->moveBy(10, 10);
    item->setPen(Qt::NoPen);
    item->setBrush(Qt::blue);
    item->setZValue(1);
}