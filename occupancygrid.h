#ifndef OCCUPANCYGRID_H 
#define OCCUPANCYGRID_H 
 
#include <QObject> 
#include <math.h> 
 
class OccupancyGridCell : public QObject 
{ 
    Q_OBJECT 
public: 
    explicit OccupancyGridCell(double value); 
    void setValue(double value); 
    double getValue(); 
private: 
    double value; 
signals: 
 
public slots: 
 
}; 
 
class OccupancyGrid : public QObject 
{ 
    Q_OBJECT 
public: 
    explicit OccupancyGrid(double width, double height, double cellSize, double cellScale); 
    ~OccupancyGrid(); 
    double getWidth(); 
    double getHeight(); 
    double getCellSize(); 
    double getCellScale(); 
    OccupancyGridCell *at(double x, double y); 
    void assign(double x, double y, double value); 
    bool isAngleAtRange(double angle1, double angle2, double range); 
private: 
    std::vector<std::vector<OccupancyGridCell*> > matrix; 
    double width; 
    double height; 
    double cellSize; 
    double cellScale; 
signals: 
 
public slots: 
 
}; 
 
#endif // OCCUPANCYGRID_H 
