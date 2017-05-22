#ifndef BOT_H 
#define BOT_H 
 
#include <QObject> 
 
using namespace std; 
 
#include "Aria.h" 
 
 
class Bot : public QObject, public ArRobot 
{ 
    Q_OBJECT 
public: 
    explicit Bot(int *argc, char **argv); 
    ~Bot(); 
    bool start(); 
    void shutdown(); 
    void stop(); 
    void readingSensors(); 
    int getLaserRange(int angle); 
    void move(double leftW, double rightW); 
    double getVelForward(); 
private: 
    ArRobotConnector botConn; 
    ArArgumentParser parser; 
    ArSick sick; 
    ArLaserConnector laserConn; 
    ArSonarDevice sonar; 
    vector<ArSensorReading> *lasers = NULL; 
    double velForward; 
signals: 
    void moving(); 
public slots: 
    void doTeleOp(); 
    void doWallFollowing(); 
    void correctWithPID(double pid); 
}; 
 
#endif // BOT_H 
