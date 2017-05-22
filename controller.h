#ifndef CONTROLLER_H 
#define CONTROLLER_H 
 
#include <QObject> 
#include <QThread> 
#include <QGraphicsView> 
#include "bot.h" 
#include "scenegrid.h" 
#include "occupancygrid.h" 
 
class Controller : public QObject 
{ 
    Q_OBJECT 
public: 
    explicit Controller(double width, double height, double cellSide, double cellSize); 
    ~Controller(); 
    void setBot(Bot *bot); 
    void run(); 
private: 
    Bot *bot; 
    SceneGrid *scene; 
    QGraphicsView *view; 
    OccupancyGrid *grid; 
    QThread *thread; 
    void showView(); 
    void updateBotOnGrid(); 
signals: 
 
public slots: 
    void update(); 
}; 
 
#endif // CONTROLLER_H 
