#include "sizehandlerect.h"
#include "qcursor.h"
#include "qdebug.h"
#include "qbitmap.h"
#include "roirectitem.h"
#include "qgraphicsscene.h"

class SizeHandleRectPrivate
{

public:
    SizeHandleRect::EnRectDirection m_rectDirection;
    QMap<SizeHandleRect::EnRectDirection, QCursor> m_rectCursor;
    const QCursor rotateCursor{QPixmap(":/Resource/Icon/rotate.png")};

public:
    double m_rectWidth = 2;
    double m_rectHeight = 2;

public:
    QGraphicsItem * m_parentItem;
    QRectF m_oldParentRect;

    QPointF m_oldRotatePoint{0, 0};
    QPointF m_oldParentCenter{0, 0};
    double m_dOldParentRotation = 0.0;

public:
    QPointF m_parentLTPoint{0, 0};
    QPointF m_parentRTPoint{0, 0};
    QPointF m_parentRBPoint{0, 0};
    QPointF m_parentLBPoint{0, 0};

public:
    void insertMap()
    {
        m_rectCursor.insert(SizeHandleRect::LeftTop, QCursor(Qt::SizeFDiagCursor));
        m_rectCursor.insert(SizeHandleRect::LeftMiddle, QCursor(Qt::SizeHorCursor));
        m_rectCursor.insert(SizeHandleRect::LeftBottom, QCursor(Qt::SizeBDiagCursor));
        m_rectCursor.insert(SizeHandleRect::CenterTop, QCursor(Qt::SizeVerCursor));
        m_rectCursor.insert(SizeHandleRect::CenterBottom, QCursor(Qt::SizeVerCursor));
        m_rectCursor.insert(SizeHandleRect::RightTop, QCursor(Qt::SizeBDiagCursor));
        m_rectCursor.insert(SizeHandleRect::RightMiddle, QCursor(Qt::SizeHorCursor));
        m_rectCursor.insert(SizeHandleRect::RightBottom, QCursor(Qt::SizeFDiagCursor));

        m_rectCursor.insert(SizeHandleRect::Rotation, rotateCursor);
    }

    //the Line is from center to corner, such as LeftTop corner
    QRectF getLTRect(QLineF lineLT, QLineF lineRB)
    {
        lineLT.setAngle(lineLT.angle() + m_dOldParentRotation);
        QPointF oldLT = lineLT.p2();

        lineRB.setAngle(lineRB.angle() + m_dOldParentRotation);
        QPointF oldRB = lineRB.p2();

        return QRectF(oldLT, oldRB);
    }

    QRectF getLBRect(QLineF lineLB, QLineF lineRT)
    {
        lineLB.setAngle(lineLB.angle() + m_dOldParentRotation);
        QPointF oldLB = lineLB.p2();

        lineRT.setAngle(lineRT.angle() + m_dOldParentRotation);
        QPointF oldRT = lineRT.p2();

        QRectF newRect;
        newRect.setTopRight(oldRT);
        newRect.setBottomLeft(oldLB);

        return newRect;
    }
};


///////////////////////////////////////////////////////////
SizeHandleRect::SizeHandleRect(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
    , d(new SizeHandleRectPrivate)
{
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsMovable, true);
    setAcceptHoverEvents(true);

    setPen(QPen(QColor(Qt::green), 0));

    d->m_parentItem = parent;

    d->insertMap();
}

void SizeHandleRect::setRectDirection(SizeHandleRect::EnRectDirection pos)
{
    d->m_rectDirection = pos;
}

void SizeHandleRect::setNewDirection(const QRectF &rect)
{
    if(this->rect().center() == rect.topLeft())
    {
        d->m_rectDirection = SizeHandleRect::LeftTop;
    }
    else if(qFuzzyCompare(this->rect().center().x(), rect.left()) &&
            qFuzzyCompare(this->rect().center().y(), rect.center().y()))
    {
        d->m_rectDirection = SizeHandleRect::LeftMiddle;
    }
    else if(this->rect().center() == rect.bottomLeft())
    {
        d->m_rectDirection = SizeHandleRect::LeftBottom;
    }
    else if(qFuzzyCompare(this->rect().center().x(), rect.center().x()) &&
            qFuzzyCompare(this->rect().center().y(), rect.top()))
    {
        d->m_rectDirection = SizeHandleRect::CenterTop;
    }
    else if(qFuzzyCompare(this->rect().center().x(), rect.center().x()) &&
            qFuzzyCompare(this->rect().center().y(), rect.bottom()))
    {
        d->m_rectDirection = SizeHandleRect::CenterBottom;
    }
    else if(this->rect().center() == rect.topRight())
    {
        d->m_rectDirection = SizeHandleRect::RightTop;
    }
    else if(qFuzzyCompare(this->rect().center().x(), rect.right()) &&
            qFuzzyCompare(this->rect().center().y(), rect.center().y()))
    {
        d->m_rectDirection = SizeHandleRect::RightMiddle;
    }
    else if(this->rect().center() == rect.bottomRight())
    {
        d->m_rectDirection = SizeHandleRect::RightBottom;
    }

}

void SizeHandleRect::setNewRect(const QRectF &rect)
{
    switch(d->m_rectDirection)
    {
        case SizeHandleRect::LeftTop :
            setRect(rect.x() - d->m_rectWidth / 2, rect.y() - d->m_rectHeight / 2, d->m_rectWidth, d->m_rectHeight);
            break;
        case SizeHandleRect::LeftMiddle:
            setRect(rect.x() - d->m_rectWidth / 2, rect.y() + rect.height() / 2 - d->m_rectHeight / 2, d->m_rectWidth, d->m_rectHeight);
            break;
        case SizeHandleRect::LeftBottom:
            setRect(rect.x() - d->m_rectWidth / 2, rect.y() + rect.height() - d->m_rectHeight / 2, d->m_rectWidth, d->m_rectHeight);
            break;
        case SizeHandleRect::CenterTop:
            setRect(rect.x() + rect.width() / 2 - d->m_rectWidth / 2, rect.y() - d->m_rectHeight / 2, d->m_rectWidth, d->m_rectHeight);
            break;
        case SizeHandleRect::CenterBottom:
            setRect(rect.x() + rect.width() / 2 - d->m_rectWidth / 2, rect.y() + rect.height() - d->m_rectHeight / 2, d->m_rectWidth, d->m_rectHeight);
            break;
        case SizeHandleRect::RightTop:
            setRect(rect.x() + rect.width() - d->m_rectWidth / 2, rect.y() - d->m_rectHeight / 2, d->m_rectWidth, d->m_rectHeight);
            break;
        case SizeHandleRect::RightMiddle:
            setRect(rect.x() + rect.width() - d->m_rectWidth / 2, rect.y() + rect.height() / 2 - d->m_rectHeight / 2, d->m_rectWidth, d->m_rectHeight);
            break;
        case SizeHandleRect::RightBottom:
            setRect(rect.x() + rect.width() - d->m_rectWidth / 2, rect.y() + rect.height() - d->m_rectHeight / 2, d->m_rectWidth, d->m_rectHeight);
            break;

        case SizeHandleRect::Rotation:
            setRect(rect.center().x() - d->m_rectWidth / 2, rect.top() - 4 * d->m_rectHeight + d->m_rectHeight / 2, d->m_rectWidth, d->m_rectHeight);
            break;
    }
}

void SizeHandleRect::setNewRect(const QRectF &rect, double scaleRatio)
{
    d->m_rectWidth = d->m_rectHeight = 1 / scaleRatio * 4;
    setNewRect(rect);
}

void SizeHandleRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);

    if(option->state & QStyle::State_Selected)
    {
        op.state = QStyle::State_None;
    }

    QGraphicsRectItem::paint(painter, &op, widget);
}

void SizeHandleRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->setSelected(true);

    if(d->m_rectDirection == SizeHandleRect::Rotation)
    {
        d->m_oldRotatePoint = event->scenePos();
    }
    auto parentItem = static_cast<RoiRectItem *>(this->parentItem());

    d->m_oldParentRect = parentItem->rect();

    //QGraphicsItem's Rotation is clockwise
    d->m_dOldParentRotation = parentItem->rotation();
    d->m_oldParentCenter = parentItem->rect().center();

    //calculate LeftTop corner coordinate
    QLineF oldLTLine(d->m_oldParentCenter, parentItem->rect().topLeft());
    oldLTLine.setAngle(oldLTLine.angle() - d->m_dOldParentRotation);
    d->m_parentLTPoint = oldLTLine.p2();

    //calculate RightTop corner coordinate
    QLineF oldRTLine(d->m_oldParentCenter, parentItem->rect().topRight());
    oldRTLine.setAngle(oldRTLine.angle() - d->m_dOldParentRotation);
    d->m_parentRTPoint = oldRTLine.p2();

    //calculate RightBottom corner coordinate
    QLineF oldRBLine(d->m_oldParentCenter, parentItem->rect().bottomRight());
    oldRBLine.setAngle(oldRBLine.angle() - d->m_dOldParentRotation);
    d->m_parentRBPoint = oldRBLine.p2();

    //calculate LeftBottom corner coordinate
    QLineF oldLBLine(d->m_oldParentCenter, parentItem->rect().bottomLeft());
    oldLBLine.setAngle(oldLBLine.angle() - d->m_dOldParentRotation);
    d->m_parentLBPoint = oldLBLine.p2();
}

void SizeHandleRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::CrossCursor);

    if(!this->isSelected())
    {
        return;
    }

    if(d->m_rectDirection != SizeHandleRect::Rotation)
    {
        resizeEvent(event);
    }
    else
    {
        rotateEvent(event);
    }
}

void SizeHandleRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //After mouse move event the transform center should be changed,
    //so you should change it when mouse release.
    if(d->m_rectDirection != SizeHandleRect::Rotation)
    {
        resizeRelease(event);
    }

    this->setSelected(false);
}

void SizeHandleRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(d->m_rectCursor.value(d->m_rectDirection));

    QGraphicsRectItem::hoverEnterEvent(event);
}

void SizeHandleRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void SizeHandleRect::resizeEvent(QGraphicsSceneMouseEvent *event)
{
    auto parentItem = static_cast<RoiRectItem *>(this->parentItem());

    QPointF newPoint = event->scenePos();
    QRectF oldRect = parentItem->rect();

    QLineF lineBeforeRotate{d->m_oldParentCenter, newPoint};
    lineBeforeRotate.setAngle(lineBeforeRotate.angle() + d->m_dOldParentRotation);
    QPointF ptBeforeRotate = lineBeforeRotate.p2();

    switch(d->m_rectDirection)
    {
        case SizeHandleRect::LeftTop :
            oldRect.setTopLeft(ptBeforeRotate);
            break;
        case SizeHandleRect::LeftMiddle:
            oldRect.setLeft(lineBeforeRotate.p2().x());
            break;
        case SizeHandleRect::LeftBottom:
            oldRect.setBottomLeft(ptBeforeRotate);
            break;
        case SizeHandleRect::CenterTop:
            oldRect.setTop(lineBeforeRotate.p2().y());
            break;
        case SizeHandleRect::CenterBottom:
            oldRect.setBottom(lineBeforeRotate.p2().y());
            break;
        case SizeHandleRect::RightTop:
            oldRect.setTopRight(ptBeforeRotate);
            break;
        case SizeHandleRect::RightMiddle:
            oldRect.setRight(lineBeforeRotate.p2().x());
            break;
        case SizeHandleRect::RightBottom:
            oldRect.setBottomRight(ptBeforeRotate);
            break;
    }

    parentItem->setRect(oldRect);

    //Don't need to set transform origin and rotation again.

    //    parentItem->setTransformOriginPoint(d->m_oldParentCenter);
    //    parentItem->setRotation(d->m_dOldParentRotation);
    parentItem->initSizeHandleRect();
}

void SizeHandleRect::resizeRelease(QGraphicsSceneMouseEvent *event)
{
    auto parentItem = static_cast<RoiRectItem *>(this->parentItem());
    QPointF newPoint = event->scenePos();

    QPointF newCenter;
    QRectF newRect;
    switch(d->m_rectDirection)
    {
        case SizeHandleRect::LeftTop:
        {
            newCenter = (newPoint + d->m_parentRBPoint) / 2;
            newRect = d->getLTRect(QLineF(newCenter, newPoint), QLineF(newCenter, d->m_parentRBPoint));
            break;
        }
        case SizeHandleRect::LeftMiddle:
        {
            QLineF lineLeft;
            lineLeft.setP1(newPoint);
            lineLeft.setAngle(90 - d->m_dOldParentRotation);

            QLineF lineTop;
            lineTop.setP1(d->m_parentRTPoint);
            lineTop.setAngle(180 - d->m_dOldParentRotation);

            QPointF newLT;
            lineLeft.intersect(lineTop, &newLT);

            newCenter = (newLT + d->m_parentRBPoint) / 2;
            newRect = d->getLTRect(QLineF(newCenter, newLT), QLineF(newCenter, d->m_parentRBPoint));

            break;
        }
        case SizeHandleRect::LeftBottom:
        {
            newCenter = ((newPoint + d->m_parentRTPoint) / 2);
            newRect = d->getLBRect(QLineF(newCenter, newPoint), QLineF(newCenter, d->m_parentRTPoint));
            break;
        }
        case SizeHandleRect::CenterTop:
        {
            QLineF lineLeft;
            lineLeft.setP1(d->m_parentLBPoint);
            lineLeft.setAngle(90 - d->m_dOldParentRotation);

            QLineF lineTop;
            lineTop.setP1(newPoint);
            lineTop.setAngle(180 - d->m_dOldParentRotation);

            QPointF newLT;
            lineLeft.intersect(lineTop, &newLT);

            newCenter = ((newLT + d->m_parentRBPoint) / 2);
            newRect = d->getLTRect(QLineF(newCenter, newLT), QLineF(newCenter, d->m_parentRBPoint));
            break;
        }
        case SizeHandleRect::CenterBottom:
        {
            QLineF lineRight;
            lineRight.setP1(d->m_parentRTPoint);
            lineRight.setAngle(270 - d->m_dOldParentRotation);

            QLineF lineBottom;
            lineBottom.setP1(newPoint);
            lineBottom.setAngle(0 - d->m_dOldParentRotation);

            QPointF newRB;
            lineRight.intersect(lineBottom, &newRB);

            newCenter = ((newRB + d->m_parentLTPoint) / 2);
            newRect = d->getLTRect(QLineF(newCenter, d->m_parentLTPoint), QLineF(newCenter, newRB));
            break;
        }
        case SizeHandleRect::RightTop:
        {
            newCenter = ((newPoint + d->m_parentLBPoint) / 2);
            newRect = d->getLBRect(QLineF(newCenter, d->m_parentLBPoint), QLineF(newCenter, newPoint));
            break;
        }
        case SizeHandleRect::RightMiddle:
        {
            QLineF lineRight;
            lineRight.setP1(newPoint);
            lineRight.setAngle(270 - d->m_dOldParentRotation);

            QLineF lineBottom;
            lineBottom.setP1(d->m_parentLBPoint);
            lineBottom.setAngle(0 - d->m_dOldParentRotation);

            QPointF newRB;
            lineRight.intersect(lineBottom, &newRB);

            newCenter = ((newRB + d->m_parentLTPoint) / 2);
            newRect = d->getLTRect(QLineF(newCenter, d->m_parentLTPoint), QLineF(newCenter, newRB));
            break;
        }
        case SizeHandleRect::RightBottom:
        {
            newCenter = ((newPoint + d->m_parentLTPoint) / 2);
            newRect = d->getLTRect(QLineF(newCenter, d->m_parentLTPoint), QLineF(newCenter, newPoint));
            break;
        }
    }

    parentItem->setRect(newRect);
    parentItem->setTransformOriginPoint(newCenter);
    parentItem->initSizeHandleRect();
}

void SizeHandleRect::rotateEvent(QGraphicsSceneMouseEvent *event)
{
    auto parentItem = static_cast<RoiRectItem *>(this->parentItem());
    QPointF newPoint = event->scenePos();

    QLineF originLine(QPointF(0, 1), QPointF(0, 0));
    QLineF moveLine(d->m_oldParentCenter, newPoint);
    double moveAngle = moveLine.angleTo(originLine);

    parentItem->setTransformOriginPoint(d->m_oldParentCenter);
    parentItem->setRotation(moveAngle);
    parentItem->initSizeHandleRect();
}
