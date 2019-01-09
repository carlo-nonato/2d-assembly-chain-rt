#ifndef ITEMSTACK_HPP
#define ITEMSTACK_HPP

#include <QGraphicsRectItem>

class ItemStack : public QGraphicsRectItem 
{
public:
    ItemStack(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
protected:
    void advance(int step) override;

private:
    void createItem();
};

#endif // ITEMSTACK_HPP
