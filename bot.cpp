#include "bot.h" 
#include "keyboard.h" 
#include "pid.h" 
 
#include <iostream> 
 
Bot::Bot(int *argc, char **argv) : 
    parser(argc, argv), 
    botConn(&parser, this), 
    laserConn(&parser, this, &botConn), 
    ArRobot(), 
    QObject() 
{ 
    velForward = 200;
    leftWheel = 0;
    rightWheel = 0;
} 
 
Bot::~Bot() 
{ 
    this->shutdown(); 
} 
 
bool Bot::start() 
{ 
    Aria::init(); 
    parser.addDefaultArgument("-rh 192.168.1.11 -remoteLaserTcpPort 10002");
    this->addRangeDevice(&sick);
    botConn.parseArgs(); 
    if (!botConn.connectRobot()) { 
        ArLog::log(ArLog::Terse, "bot: failed to connect with"); 
        return false; 
    } 
 
    /*qqlaserConn.setupLaser(&sick);
    if (!laserConn.connectLaser(&sick)) { 
        ArLog::log(ArLog::Terse, "sick: failed to connect with"); 
        return false; 
    }*/

    ArLog::log(ArLog::Normal,"bot: sucessfully connected"); 
    //sick.runAsync();
    this->runAsync(true); 
 
    this->lock(); 
    this->enableMotors(); 
    this->unlock();

    ArUtil::sleep(500);

    return true; 
} 
 
void Bot::stop2()
{ 
    this->lock();
    this->disableMotors(); 
    this->unlock();
} 
 
void Bot::shutdown()
{
    this->stop2();
    this->stopRunning(); 
    this->waitForRunExit();
    Aria::shutdown();
}
 
void Bot::setLeftWheel(double leftWheel)
{
    this->leftWheel = leftWheel;
}

void Bot::setRightWheel(double rightWheel)
{
    this->rightWheel = rightWheel;
}

bool Bot::isStarting()
{
    return starting;
}

void Bot::readingLaser()
{ 
    if (this->isConnected() && sick.isConnected()) { 
        this->lock();
        sick.lockDevice(); 
        //laser = sick.getRawReadingsAsVector();
        sick.unlockDevice(); 
        this->unlock();
    } 
}

vector<ArSensorReading> Bot::getLaser()
{
    return laser;
}


void Bot::readingSonar()
{
    if (this->isConnected()) {
        this->lock();
        sonar.clear();
        for (int i = 0; i <= 7; i++)
            sonar.push_back(*this->getSonarReading(i));
        this->reading();
        this->unlock();
    }
}

vector<ArSensorReading> Bot::getSonar()
{
    return sonar;
}

void Bot::doExploration()
{
    if (!this->start()) {
        return;
    }
    starting = true;
    while (Aria::getRunning()) {
        this->readingSonar();
        //if (starting == true) {
        //    this->moveAlt(0, 20);
        //    if (this->getTh() < 0) starting = false;
        //} else {
            this->moveAlt(leftWheel, rightWheel);
        //}
        if (Keyboard::getInstance()->isQ()) {
            break;
        }
        ArUtil::sleep(250);
    }
    if (Keyboard::getInstance()->isQ()) this->shutdown();
}

void Bot::doTeleOp()
{ 
    if (!this->start()) { 
        return; 
    } 
    while (Aria::getRunning()) {         
        this->lock();
        if (Keyboard::getInstance()->isArrowUp()) { 
            this->move(50);
        }
        if (Keyboard::getInstance()->isArrowDown()) { 
            this->move(-50);
        }
        if (Keyboard::getInstance()->isArrowLeft()) { 
            this->setDeltaHeading(10);
        }
        if (Keyboard::getInstance()->isArrowRight()) { 
            this->setDeltaHeading(-10);
        }
        if (Keyboard::getInstance()->isQ()) {
            break;
        }
        this->unlock();
        this->readingSonar();
        ArUtil::sleep(250);
    }
    if (Keyboard::getInstance()->isQ()) this->shutdown();
} 
 
void Bot::doWallFollowing() 
{ 
    if (!this->start()) { 
        return; 
    } 
 
    bool following, leftFollowing = false, rightFollowing = false 
    , frontWall = false, leftWall = false, rightWall = false; 
    double leftReading, rightReading, frontReading, range = 1500, timeInterval = 0.5; 
 
    PID *leftPID = new PID(1, 0.8, 0.002, 1000);//2, 1.8, 0.5 
    PID *rightPID = new PID(1, 0.8, 0.002, 1000); 
 
    while (Aria::getRunning()) { 
        leftReading = sick.currentReadingPolar(30, 60); 
        rightReading = sick.currentReadingPolar(-30, -60); 
        frontReading = sick.currentReadingPolar(-30, 30); 
        frontWall = (frontReading < range); 
        leftWall = (leftReading < range); 
        rightWall = (rightReading < range); 
        following = false; 
        if (frontWall) { 
            this->moveAlt(0, 0);
            if (rightWall) {//turning left 
                this->moveAlt(velForward*0.1, velForward);
            } else {//default or left wall, turn right 
                this->moveAlt(velForward, velForward*0.1);
            } 
        } 
 
        if (leftWall) {//sticking to the wall 
            following = true; 
            leftFollowing = true; 
            rightFollowing = false; 
            this->correctWithPID(leftPID->calculate(leftReading, timeInterval)); 
        } 
        if (rightWall) { 
            following = true; 
            rightFollowing = true; 
            leftFollowing = false; 
            this->correctWithPID(rightPID->calculate(rightReading, timeInterval)); 
        } 
 
        if (following == false) { 
            if (leftFollowing) {//small turn, just checking if it was a sharp wall edge 
                this->moveAlt(velForward*0.1, velForward);
                leftFollowing = false; 
            } else if (rightFollowing) { 
                this->moveAlt(velForward, velForward*0.1);
                rightFollowing = false; 
            } else {//wander 
                this->moveAlt(velForward, velForward);
            } 
        } 
        ArUtil::sleep(1000*timeInterval); 
    } 
} 
 
void Bot::correctWithPID(double pid) 
{ 
    double e1 = 0, e2 = 0, v1 = velForward, v2 = velForward; 
 
    if (pid > 0) { 
        e1 = pid; 
    } else { 
        e2 = -pid; 
    } 
    if (pid > v1) { 
        e1 = v1; 
    } else if (pid < -v1) { 
        e2 = v2; 
    } 
    this->moveAlt(v1-e1, v2-e2);
} 
 
void Bot::moveAlt(double leftW, double rightW)
{ 
    this->lock();
    this->setVel2(leftW, rightW); 
    this->unlock(); 
}
 
double Bot::getVelForward() 
{ 
    return velForward; 
} 
