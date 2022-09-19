#ifndef SIZEHINTRECT_H
#define SIZEHINTRECT_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

class SizeHandleRectPrivate;
class SizeHandleRect
    : public QGraphicsRectItem
{
public:
    SizeHandleRect(QGraphicsItem *parent = Q_NULLPTR);

public:
    enum EnRectDirection
    {
        LeftTop,
        LeftMiddle,
        LeftBottom,
        CenterTop,
        CenterBottom,
        RightTop,
        RightMiddle,
        RightBottom,

        Rotation,
    };

    static const int RectDirectionNum = Rotation + 1;

public:
    void setRectDirection(EnRectDirection pos);
    void setNewDirection(const QRectF &rect);

    void setNewRect(const QRectF &rect);
    void setNewRect(const QRectF &rect, double scaleRatio);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    void resizeEvent(QGraphicsSceneMouseEvent *event);

    void resizeRelease(QGraphicsSceneMouseEvent *event);

    void rotateEvent(QGraphicsSceneMouseEvent *event);

private:
    QScopedPointer<SizeHandleRectPrivate> d;



};

#endif // SIZEHINTRECT_H
