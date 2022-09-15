#ifndef SCALEGRAPHICSVIEW_H
#define SCALEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class ScaleGraphicsViewPrivate;
class ScaleGraphicsView
    : public QGraphicsView
{
public:
    ScaleGraphicsView(QWidget *parent = nullptr);
    ~ScaleGraphicsView();
public:
    virtual void setScaleRatio(double ratio);
    double currentScaleRatio() const;
    void shrinkToViewPort();

    void setMoveable(bool able);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    double getAutoFitScale(bool *bOk = nullptr) const;
private:
    QScopedPointer<ScaleGraphicsViewPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ScaleGraphicsView)
};

#endif // SCALEGRAPHICSVIEW_H
