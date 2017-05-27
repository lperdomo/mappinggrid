#ifndef OCCUPANCYGRID_H 
#define OCCUPANCYGRID_H 
 
#include <QObject>
#include <QTime>
#include <math.h> 
 
#include "bayes.h"

class OccupancyGridCell : public QObject 
{ 
    Q_OBJECT 
public: 
    explicit OccupancyGridCell(double value); 
    void setSensorId(double value);
    double getSensorId();
    void setBayes();
    Bayes *getBayes();
    void setScanTime();
    bool isTimeToScanAgain();
private:
    double sensorId;
    Bayes *bayes;
    QTime *scanTime;
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
    void assign(double x, double y, double sensorId);
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
