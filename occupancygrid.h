#ifndef OCCUPANCYGRID_H 
#define OCCUPANCYGRID_H 
 
#include <QObject>
#include <QTime>
#include <vector>

#include "bot.h"
#include "bayesian.h"
#include "histogramic.h"
#include "potentialfield.h"
#include "util.h"

class OccupancyGridCell : public QObject 
{ 
    Q_OBJECT 
public: 
    explicit OccupancyGridCell(double sensorId);
    void setSensorId(double value);
    double getSensorId();
    bool isChanged();
    void setRegion(int region);
    void setScanTime();
    bool isTimeToScanAgain();
    Bayesian *getBayesian();
    void bayesianProbability(double r, double R, double alpha, double beta);
    Histogramic *getHistogramic();
    void setHistogramic();
    PotentialField *getPotentialField();
    void updatePotentialField(double potential, double th);
private:
    double sensorId;
    bool changed;
    int region;
    QTime *scanTime;
    Bayesian *bayesian;
    Histogramic *histogramic;
    PotentialField *potentialField;
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
    void reset(double botx, double boty);
    void updateWithBayesian(Bot *bot);
    void updateWithHistogramic(Bot *bot);
    void updatePotentialFields(Bot *bot);
    void calculatePotential(double x, double y);
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
