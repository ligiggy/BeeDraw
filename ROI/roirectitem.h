#ifndef ROIRECTITEM_H
#define ROIRECTITEM_H

#include <QGraphicsItem>
#include <sizehandlerect.h>
#include <QStyleOptionGraphicsItem>

class RoiRectItemPrivate;

class RoiRectItem
    : public QGraphicsRectItem
{
public:
    explicit RoiRectItem(QGraphicsItem *parent = Q_NULLPTR);
    explicit RoiRectItem(const QRectF &rect, QGraphicsItem *parent = Q_NULLPTR);
    explicit RoiRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = Q_NULLPTR);

    ~RoiRectItem();

public:
    enum EnMode
    {
        DrawMode,
        MoveMode,
        ChooseMode,
    };

public:
    void refreshSizeHandleRect();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;

public:
    void initSizeHandleRect();

private:
    QScopedPointer<RoiRectItemPrivate> d_ptr;

};

#endif // ROIRECTITEM_H
