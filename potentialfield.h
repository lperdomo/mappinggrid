#ifndef POTENTIALFIELD_H
#define POTENTIALFIELD_H

class PotentialField
{
public:
    PotentialField();
    void setValue(double value);
    double getValue();
private:
    double value;
};

#endif // POTENTIALFIELD_H


















































/***
 * class CampoPotencial : public QThread, public IRenderMap
{
    Q_OBJECT
public:
    CampoPotencial(Mapping *map,int mapSize);
    void MoveRobot();
    double distance(double x1, double y1, double x2, double y2);
    void render();
    void drawRobot();
    double **mPotencial, Dmax, Dmin, Dobstacle;
    int mMapSize, celRange;
    PaletaCores paleta;
    bool time;
    struct timeval begin,end;
    Mapping *mMapping;

    float angleAndar;
public slots:
    float ComputaDistancia();

protected:
    void run();
};*/
