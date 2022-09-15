#include "drawroiscene.h"
#include "qdebug.h"
#include "qgraphicsitem.h"
#include "roirectitem.h"
#include "scalegraphicsview.h"


class DrawRoiScenePrivate
{
public:
    QList<QGraphicsItem* > m_graphicsItems;

    QGraphicsItem* m_currentItem = nullptr;
    int  m_nItemType = 0;

    QPointF oldPos;

    DrawRoiScene::EnMode m_mode = DrawRoiScene::DrawMode;

    bool m_bStartDraw = false;
};



///////////////////////////////////////////////////////////////////////////
DrawRoiScene::DrawRoiScene(QGraphicsScene *parent)
    : QGraphicsScene(parent)
    , d_ptr(new DrawRoiScenePrivate)
{

}

DrawRoiScene::~DrawRoiScene()
{

}

void DrawRoiScene::setCurrentItem(QGraphicsItem *item, int itemType)
{
    auto gv = static_cast<ScaleGraphicsView *>(this->views().first());
    gv->setMoveable(false);

    d_ptr->m_currentItem = item;
    d_ptr->m_nItemType = itemType;
}

DrawRoiScene::EnMode DrawRoiScene::getMode()
{
    return d_ptr->m_mode;
}

void DrawRoiScene::setMode(DrawRoiScene::EnMode mode)
{
    d_ptr->m_mode = mode;

    auto gv = static_cast<ScaleGraphicsView *>(this->views().first());

    switch(mode)
    {
        case DrawRoiScene::DrawMode:
            gv->setMoveable(false);
            break;
        case DrawRoiScene::MoveMode:
            gv->setMoveable(true);
            break;
        case DrawRoiScene::SelectMode:
            gv->setMoveable(false);
            break;
        default:
            break;
    }

}

void DrawRoiScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(d_ptr->m_mode == DrawMode)
    {
        if(this->items().isEmpty())
        {
            this->clearSelection();
        }

        d_ptr->m_bStartDraw = true;

        this->clearSelection();

        if(mouseEvent->button() == Qt::LeftButton)
        {
            if(d_ptr->m_currentItem)
            {
                d_ptr->oldPos = mouseEvent->scenePos();
            }
        }
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void DrawRoiScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(d_ptr->m_mode == DrawMode)
    {
        if(d_ptr->m_bStartDraw)
        {
            if(d_ptr->m_nItemType == 0)
            {
                auto item = static_cast<RoiRectItem*>(d_ptr->m_currentItem);

                if(item)
                {
                    QPointF newPos = mouseEvent->scenePos();

                    if(newPos.x() < d_ptr->oldPos.x())
                    {
                        item->setRect(newPos.x(), newPos.y(),
                                      d_ptr->oldPos.x() - newPos.x(), d_ptr->oldPos.y() - newPos.y());
                    }
                    else
                    {
                        item->setRect(d_ptr->oldPos.x(), d_ptr->oldPos.y(),
                                      newPos.x() - d_ptr->oldPos.x(), newPos.y() - d_ptr->oldPos.y());
                    }
                }
            }
        }

    }

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void DrawRoiScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(d_ptr->m_mode == DrawMode)
    {
        if(d_ptr->m_currentItem)
        {
            d_ptr->m_currentItem->setSelected(true);
            d_ptr->m_bStartDraw = false;
            this->setMode(SelectMode);
        }
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
