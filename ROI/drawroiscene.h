#ifndef DRAWROISCENE_H
#define DRAWROISCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

class DrawRoiScenePrivate;
class DrawRoiScene
    : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit DrawRoiScene(QGraphicsScene *parent = nullptr);

    ~DrawRoiScene();

public:
    enum EnMode
    {
        DrawMode,
        MoveMode,
        SelectMode,
    };

public:
    void setCurrentItem(QGraphicsItem *item, int itemType = 0);

    EnMode getMode();
    void setMode(EnMode mode);


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    QScopedPointer<DrawRoiScenePrivate> d_ptr;
};

#endif // DRAWROISCENE_H
