#ifndef SCENEGRID_H 
#define SCENEGRID_H 
 
#include <QGraphicsScene> 
#include <QGraphicsItem> 
#include <QPainter> 
#include <QKeyEvent> 

#include "occupancygrid.h" 
#include "util.h"

class SceneGridBot
{ 
public: 
    SceneGridBot();
    void setXY(double x, double y); 
    void setTh(double th); 
    double getX(); 
    double getY(); 
    double getTh(); 
private: 
    double x; 
    double y; 
    double th; 
}; 
 
class SceneGridItem : public QGraphicsItem 
{ 
public: 
    SceneGridItem(OccupancyGrid *grid); 
    ~SceneGridItem(); 
    QRectF boundingRect() const; 
    OccupancyGrid *grid; 
protected: 
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0); 
}; 
 
class SceneGrid : public QGraphicsScene 
{ 
public: 
    SceneGrid(qreal x, qreal y, qreal width, qreal height, OccupancyGrid *grid); 
    ~SceneGrid(); 
    SceneGridBot *bot;
private: 
    SceneGridItem *gridItem; 
    void drawGrid(QPainter *painter, const QRectF &rect);
    void drawBot(QPainter *painter, const QRectF &rect);
    void drawColoredRect(QPainter *painter, double x, double y, QColor color);
protected: 
    void drawForeground(QPainter *painter, const QRectF &rect); 
    void keyPressEvent(QKeyEvent *event); 
    void keyReleaseEvent(QKeyEvent *event); 
}; 
 
#endif // SCENEGRID_H 
