#ifndef SCENEGRID_H 
#define SCENEGRID_H 
 
#include <QGraphicsScene> 
#include <QGraphicsItem> 
#include <QPainter> 
#include <QKeyEvent> 

#include "occupancygrid.h" 
#include "util.h"
 
class SceneGridItem : public QGraphicsItem 
{ 
public: 
    SceneGridItem(OccupancyGrid *grid); 
    ~SceneGridItem(); 
    QRectF boundingRect() const; 
    OccupancyGrid *grid; 
    double scale;
protected: 
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0); 
private:
    void drawColoredRect(QPainter *painter, double x, double y, QColor color);
}; 
 
class SceneGrid : public QGraphicsScene 
{ 
public: 
    SceneGrid(qreal x, qreal y, qreal width, qreal height, OccupancyGrid *grid); 
    ~SceneGrid(); 
private:
    SceneGridItem *gridItem; 
    void drawGrid(QPainter *painter, const QRectF &rect);
protected: 
    void drawForeground(QPainter *painter, const QRectF &rect);
    void keyPressEvent(QKeyEvent *event); 
    void keyReleaseEvent(QKeyEvent *event); 
}; 
 
#endif // SCENEGRID_H 
