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
    void setBot(Bot *bot);
    bool getStopPotential();
    OccupancyGridCell *at(double x, double y);
    void assign(double x, double y, double sensorId);
    void reset(double botx, double boty);
    void updateWithBayesian();
    void updateWithHistogramic();
    void updatePotentialFields();
    void calculatePotential(double x, double y);
    bool isExplorationEnd();
private:
    std::vector<std::vector<OccupancyGridCell*> > matrix; 
    double width; 
    double height; 
    double cellSize;
    double cellScale;
    int potentialIterations;
    bool stopPotential;
    Bot *bot;
signals: 
 
public slots: 
    void doOccupancyGrid();
}; 
 
#endif // OCCUPANCYGRID_H 
