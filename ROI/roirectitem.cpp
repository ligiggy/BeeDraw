#include "roirectitem.h"
#include "qpen.h"
#include "qdebug.h"
#include "qgraphicsscene.h"
#include "QGraphicsSceneEvent"
#include "qgraphicsview.h"
#include "scalegraphicsview.h"
#include "sizehandlerect.h"


class RoiRectItemPrivate
{
public:
    QList<SizeHandleRect *> rectBoundingList;

    QRectF oldRect;
    QPointF oldPos;
};




////////////////////////////////////////////////////////////////////////////////////////////
RoiRectItem::RoiRectItem(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
    , d_ptr(new RoiRectItemPrivate)
{
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsMovable, true);

    setFlag(ItemClipsToShape, true);

    this->setAcceptHoverEvents(true);
    this->setFiltersChildEvents(true);

    for(int i = 0; i < SizeHandleRect::RectDirectionNum; i++)
    {
        d_ptr->rectBoundingList.append(new SizeHandleRect(this));
        d_ptr->rectBoundingList.at(i)->setRectDirection(SizeHandleRect::EnRectDirection(i));
        d_ptr->rectBoundingList.at(i)->setParentItem(this);
    }
}

RoiRectItem::RoiRectItem(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
    , d_ptr(new RoiRectItemPrivate)
{
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsMovable, true);
    this->setAcceptHoverEvents(true);

    setRect(rect);
}

RoiRectItem::RoiRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
    , d_ptr(new RoiRectItemPrivate)
{
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsMovable, true);
    this->setAcceptHoverEvents(true);

    setRect(x, y, w, h);
}

RoiRectItem::~RoiRectItem()
{

}

void RoiRectItem::refreshSizeHandleRect()
{
    for(int i = 0; i < SizeHandleRect::RectDirectionNum; i++)
    {
        d_ptr->rectBoundingList.at(i)->setNewDirection(boundingRect());
    }
}



void RoiRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);

    if(option->state & QStyle::State_Selected)
    {
        op.state = QStyle::State_None;
    }

    QGraphicsRectItem::paint(painter, &op, widget);

    if(option->state & QStyle::State_Selected)
    {
        initSizeHandleRect();
    }
    else
    {
        for(auto boundingRect : d_ptr->rectBoundingList)
        {
            if(boundingRect->isSelected())
            {
                return;
            }
        }

        for(auto boundingRect : d_ptr->rectBoundingList)
        {
            boundingRect->hide();
        }
    }
}

QVariant RoiRectItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedChange && scene())
    {
        bool bItemSelected = value.toBool();

        if(bItemSelected)
        {
            for(int i = 0; i < SizeHandleRect::RectDirectionNum; i++)
            {
                d_ptr->rectBoundingList.at(i)->setSelected(true);
            }
        }

        return bItemSelected;
    }

    return QGraphicsItem::itemChange(change, value);
}

void RoiRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setSelected(true);

    d_ptr->oldPos = event->scenePos();
    d_ptr->oldRect = rect();
    QGraphicsRectItem::mousePressEvent(event);
}

void RoiRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF curPos = event->scenePos();

    double moveX = curPos.x() - d_ptr->oldPos.x();
    double moveY = curPos.y() - d_ptr->oldPos.y();

    //must not use QGraphicsRectItem::mouseMoveEvent
    QRectF curRect = d_ptr->oldRect;
    curRect.translate(moveX, moveY);
    setRect(curRect);
    setTransformOriginPoint(d_ptr->oldRect.center() + QPointF(moveX, moveY));
}

void RoiRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mouseReleaseEvent(event);
}

void RoiRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setCursor(QCursor(Qt::SizeAllCursor));

    QGraphicsRectItem::hoverEnterEvent(event);
}

void RoiRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsRectItem::hoverEnterEvent(event);
}

bool RoiRectItem::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    SizeHandleRect *childRect = static_cast<SizeHandleRect *>(watched);
    if(d_ptr->rectBoundingList.contains(childRect))
    {
        if(event->type() == QEvent::GraphicsSceneHoverEnter)
        {
        }
    }

    return QGraphicsRectItem::sceneEventFilter(watched, event);
}

void RoiRectItem::initSizeHandleRect()
{
    for(int i = 0; i < SizeHandleRect::RectDirectionNum; i++)
    {
        d_ptr->rectBoundingList.at(i)->setNewRect(boundingRect());
        d_ptr->rectBoundingList.at(i)->show();
    }
}
