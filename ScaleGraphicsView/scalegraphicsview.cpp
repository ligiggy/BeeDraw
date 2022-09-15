#include <QGraphicsItem>
#include <QtMath>
#include <QScrollBar>

#include "scalegraphicsview.h"

class ScaleGraphicsViewPrivate
{
public:
    double m_dScaleRatio = 1.;

    bool m_bMoving = false;
    QPoint sliderValue;
    QPoint startViewPos;


    bool m_bMoveable = true;
};

ScaleGraphicsView::ScaleGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
    , d_ptr(new ScaleGraphicsViewPrivate())
{
    //
    setRenderHint(QPainter::Antialiasing);
}
ScaleGraphicsView::~ScaleGraphicsView()
{

}

void ScaleGraphicsView::setScaleRatio(double ratio)
{
    Q_D(ScaleGraphicsView);
    d->m_dScaleRatio = ratio;
    QMatrix matrix;
    matrix *= QMatrix().scale(ratio, ratio);
    //    matrix *= QMatrix(1,0,0,-1,0,0);
    setMatrix(matrix);
}
double ScaleGraphicsView::currentScaleRatio() const
{
    Q_D(const ScaleGraphicsView);
    return d->m_dScaleRatio;
}
void ScaleGraphicsView::shrinkToViewPort()
{
    QRectF sceneRect = this->sceneRect();

    if(sceneRect.isNull())
    {
        return;
    }

    this->setScaleRatio(getAutoFitScale());
}

void ScaleGraphicsView::setMoveable(bool able)
{
    Q_D(ScaleGraphicsView);

    d->m_bMoveable = able;
}

void ScaleGraphicsView::mousePressEvent(QMouseEvent *e)
{
    Q_D(ScaleGraphicsView);

    if(e->buttons().testFlag(Qt::LeftButton))
    {
        d->startViewPos = e->pos();
        d->sliderValue = QPoint(horizontalScrollBar()->value()
                                , verticalScrollBar()->value());
        d->m_bMoving = true;
    }

    return QGraphicsView::mousePressEvent(e);
}

void ScaleGraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
    Q_D(ScaleGraphicsView);
    d->m_bMoving = false;

    if(e->button() == (Qt::LeftButton))
    {
        setCursor(Qt::CrossCursor);
    }

    return QGraphicsView::mouseReleaseEvent(e);
}

void ScaleGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::RightButton)
    {
        this->shrinkToViewPort();
    }

    return QGraphicsView::mouseDoubleClickEvent(event);
}

void ScaleGraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    Q_D(ScaleGraphicsView);

    if(d->m_bMoving && d->m_bMoveable)
    {
        setCursor(Qt::ClosedHandCursor);
        QPoint offset = e->pos() - d->startViewPos;
        offset = d->sliderValue - offset;
        horizontalScrollBar()->setValue(offset.x());
        verticalScrollBar()->setValue(offset.y());
    }

    return QGraphicsView::mouseMoveEvent(e);
}

void ScaleGraphicsView::wheelEvent(QWheelEvent *e)
{
    if(sceneRect().isNull())
    {
        return;
    }

    //    if(!e->modifiers().testFlag(Qt::ControlModifier) && d->m_bCtrlScale){
    //        return;
    //    }

    QPoint cursorPoint = e->pos();
    QPointF scenePos = mapToScene(cursorPoint.x(), cursorPoint.y());

    qreal viewWidth = viewport()->width();
    qreal viewHeight = viewport()->height();

    qreal hScale = cursorPoint.x() / viewWidth;
    qreal vScale = cursorPoint.y() / viewHeight;

    int wheelDeltaValue = e->delta();

    double ratio = 1.2;

    if(wheelDeltaValue < 0)
    {
        ratio = 1 / ratio;
    }

    setScaleRatio(ratio * currentScaleRatio());

    QPointF viewPoint = matrix().map(scenePos);
    horizontalScrollBar()->setValue(qRound(viewPoint.x() - viewWidth * hScale));
    verticalScrollBar()->setValue(qRound(viewPoint.y() - viewHeight * vScale));
    return;
}

double ScaleGraphicsView::getAutoFitScale(bool *bOk) const
{
    if(bOk)
    {
        *bOk = false;
    }

    double value(0.);
    QRectF sceneRect = this->sceneRect();

    if(sceneRect.isNull())
    {
        return value;
    }

    double h = 3.;
    double w = 3.;

    if(auto hor = horizontalScrollBar())
    {
        if(hor->isVisible())
        {
            h = hor->height();
        }
    }

    if(auto ver = verticalScrollBar())
    {
        if(ver->isVisible())
        {
            w = ver->width();
        }
    }

    double r1 = static_cast<double>(this->width() - w) / (sceneRect.width());
    double r2 = static_cast<double>(this->height() - h) / (sceneRect.height());
    value = qMin(r1, r2);

    if(bOk)
    {
        *bOk = true;
    }

    ///
    return value;
}
