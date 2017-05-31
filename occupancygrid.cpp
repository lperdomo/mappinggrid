#include "occupancygrid.h" 
#include <iostream> 
 
OccupancyGrid::OccupancyGrid(double width, double height, double cellSize, double cellScale) : 
    matrix(width+1, std::vector<OccupancyGridCell*>(height+1, NULL)), 
    QObject() 
{ 
    this->width = width; 
    this->height = height; 
    this->cellSize = cellSize; 
    this->cellScale = cellScale;
} 
 
OccupancyGrid::~OccupancyGrid() 
{ 
} 
 
double OccupancyGrid::getWidth() 
{ 
    return width; 
} 
 
double OccupancyGrid::getHeight() 
{ 
    return height; 
} 
 
double OccupancyGrid::getCellSize() 
{ 
    return this->cellSize; 
} 
 
double OccupancyGrid::getCellScale() 
{ 
    return this->cellScale; 
} 
 
OccupancyGridCell *OccupancyGrid::at(double x, double y) 
{ 
    x = x + (width/2); 
    y = (height/2) - y; 
    return matrix[x][y]; 
} 
 
void OccupancyGrid::assign(double x, double y, double sensorId)
{ 
    x = x + (width/2); 
    y = (height/2) - y; 
    if (!matrix[x][y]) { 
        matrix[x][y] = new OccupancyGridCell(sensorId);
    } else { 
        matrix[x][y]->setSensorId(sensorId);
    } 
}

/*void OccupancyGrid::updateWithBayesianOld(Bot *bot)
{
    double rangeA = (3000+200)/cellScale,
           rangeB = (1500-200)/cellScale,
           rangeC = 5000/cellScale,
           botx = round(bot->getX()/cellScale),
           boty = round(bot->getY()/cellScale),
           botth = bot->getTh()*-1;

    vector<ArSensorReading> sensor = bot->getSonar();

    double botWidth = (botx+rangeC > this->getWidth()/2 ? this->getWidth()/2 : botx+rangeC),
           botHeight = (boty+rangeC > this->getHeight()/2 ? this->getHeight()/2 : boty+rangeC);


    for (double x = botx-rangeC-1; x <= botWidth+1; x++) {
        for (double y = boty-rangeC-1; y <= botHeight+1; y++) {
            double range = Util::distanceBetweenPoints(x, y, botx, boty, botWidth, botHeight),
                   angle = Util::angleBetweenPoints(x, y, botx, boty, botWidth, botHeight);

            if (x == botx && y == boty) {
                this->assign(x, y, 0);
            } else if (range <= rangeC) {
                bool atRange = false;
                for (int i = 0; (i < sensor.size() && atRange == false); i++) {
                    double angleSonar = sensor.at(i).getSensorTh()*-1,
                           rangeSensor = sensor.at(i).getRange()/cellScale;

                    if (i == 0 || i == 7) atRange = Util::isAngleAtRange(botth+angleSonar, angle, 15);
                    else if (i >= 1 && i <= 6)
                        if (Util::isAngleAtRange(botth+angleSonar, angle, 15)
                            && !Util::isAngleAtRange(botth+sensor.at(i+1).getSensorTh()*-1, angle, 10)
                            && !Util::isAngleAtRange(botth+sensor.at(i-1).getSensorTh()*-1, angle, 10)) atRange = true;
                        else if (Util::isAngleAtRange(botth+angleSonar+10, angle, 0.5)) atRange = true;

                    if (atRange) {
                        if (range >= rangeB && range <= rangeA) {
                            this->assign(x, y, i+1);
                            if (rangeSensor >= rangeB && rangeSensor <= rangeA) this->at(x, y)->setRegion(1);
                            else this->at(x, y)->setRegion(3);
                            this->at(x, y)->bayesianProbability(range, rangeC, fabs(angle-botth-angleSonar), 15);
                        } else if (range < rangeB) {
                            this->assign(x, y, i+1.5);
                            if (rangeSensor < rangeB) this->at(x, y)->setRegion(2);
                            else this->at(x, y)->setRegion(3);
                            this->at(x, y)->bayesianProbability(range, rangeC, fabs(angle-botth-angleSonar), 15);
                        } else {
                            this->assign(x, y, i+1.5);
                            this->at(x, y)->setRegion(3);
                        }
                    }
                }
                if (atRange == false && this->at(x, y)) this->assign(x, y, -1);
            } else if (this->at(x, y)) this->assign(x, y, -1);
        }
    }
}*/

void OccupancyGrid::updateWithBayesian(Bot *bot)
{
    double rangeA = (3000+200)/cellScale,
           rangeB = (1500-200)/cellScale,
           rangeC = 5000/cellScale,
           botx = round(bot->getX()/cellScale),
           boty = round(bot->getY()/cellScale),
           botth = bot->getTh();

    vector<ArSensorReading> sensor = bot->getSonar();

    double rangeMax = 5000/cellScale,
           tolerance = 500/cellScale,
           botWidth = (botx+rangeMax > this->getWidth()/2 ? this->getWidth()/2 : botx+rangeMax),
           botHeight = (boty+rangeMax > this->getHeight()/2 ? this->getHeight()/2 : boty+rangeMax);

    for (double x = botx-rangeMax-1; x <= botWidth+1; x++) {
        for (double y = boty-rangeMax-1; y <= botHeight+1; y++) {
            if (x == botx && y == boty) {
                this->assign(x, y, 0);
            } else {
                if (this->at(x, y)) this->assign(x, y, -1);
            }
        }
    }

    for (int i = 0; i < sensor.size(); i++) {
        double angleSonar = sensor.at(i).getSensorTh()*-1,
               rangeSensor = sensor.at(i).getRange()/cellScale,
               rangeA = (rangeSensor+tolerance > rangeMax ? rangeMax : rangeSensor+tolerance),
               rangeB = (rangeSensor-tolerance < 1 ? 1 : rangeSensor-tolerance),
               angleStart, angleEnd;

        if (i == 0 || i == sensor.size()-1) {
            angleStart = angleSonar-15;
            angleEnd = angleSonar+15;
        } else {
            if (angleSonar-15 > sensor.at(i-1).getSensorTh()+15) {
                angleStart = angleSonar-10;
            } else if (angleSonar-15 < sensor.at(i-1).getSensorTh()+15) {
                angleStart = angleSonar-10;
            }
            if (angleSonar+15 > sensor.at(i-1).getSensorTh()-15) {
                angleEnd = angleSonar+10;
            } else if (angleSonar+15 < sensor.at(i-1).getSensorTh()-15) {
                angleEnd = angleSonar+10;
            }
        }
        for (double angle = angleStart; angle <= angleEnd; angle+=0.5) {
            //std::cout << "i" << i << " angle" << angle << std::endl;
            int x = round(rangeMax*cos((botth-angle)*M_PI/180)),
                y = round(rangeMax*sin((botth-angle)*M_PI/180));
            //std::cout << "x" << x << " y" << y << std::endl;
            /*if (rangeSensor >= rangeMax) {
                this->assign(x, y, i+1.5);
                this->at(x, y)->setRegion(3);
            }*/
            double rangeX = x,
                   rangeY = y,
                   m = max(fabs(rangeX), fabs(rangeY)),
                   xStep = x/m,
                   yStep = y/m;

            for (int n = 0; n <= m; n++) {
                int nX = n*xStep;
                int nY = n*yStep;
                double range = Util::distanceBetweenPoints(nX, nY, botx, boty, botWidth, botHeight);
                nX += botx;
                nY += boty;
                if (!(nX == botx && nY == boty)) {
                    if (rangeSensor >= rangeMax) {
                        this->assign(nX, nY, i+1.5);
                        this->at(nX, nY)->setRegion(2);
                        this->at(nX, nY)->bayesianProbability(rangeSensor, rangeMax, fabs(botth-angle), 15);
                    } else if (rangeA < range) {
                        this->assign(nX, nY, i+1.5);
                        this->at(nX, nY)->setRegion(3);
                    } else if (rangeB <= range && range <= rangeA) {
                        this->assign(nX, nY, i+1);
                        this->at(nX, nY)->setRegion(1);
                        this->at(nX, nY)->bayesianProbability(rangeSensor, rangeMax, fabs(botth-angle), 15);
                    } /*else if (range < rangeB) {
                        this->assign(nX, nY, i+1.5);
                        this->at(nX, nY)->setRegion(2);
                        this->at(nX, nY)->bayesianProbability(rangeSensor, rangeMax, fabs(botth-angle), 15);
                    }*/
                }
            }
        }

        /*if (i == 0 || i == 7) atRange = Util::isAngleAtRange(botth+angleSonar, angle, 15);
        else if (i >= 1 && i <= 6)
            if (Util::isAngleAtRange(botth+angleSonar, angle, 15)
                && !Util::isAngleAtRange(botth+sensor.at(i+1).getSensorTh()*-1, angle, 10)
                && !Util::isAngleAtRange(botth+sensor.at(i-1).getSensorTh()*-1, angle, 10)) atRange = true;
            else if (Util::isAngleAtRange(botth+angleSonar+10, angle, 0.5)) atRange = true;*/

        /*if () {
            this->assign(x, y, i+1);
            if (rangeSensor >= rangeB && rangeSensor <= rangeA) this->at(x, y)->setRegion(1);
            else this->at(x, y)->setRegion(3);
            this->at(x, y)->bayesianProbability(range, rangeC, fabs(angle-botth-angleSonar), 15);
        } else if (range < rangeB) {
            this->assign(x, y, i+1.5);
            if (rangeSensor < rangeB) this->at(x, y)->setRegion(2);
            else this->at(x, y)->setRegion(3);
            this->at(x, y)->bayesianProbability(range, rangeC, fabs(angle-botth-angleSonar), 15);
        } else {
            this->assign(x, y, i+1.5);
            this->at(x, y)->setRegion(3);
        }*/
    }


    /*for (double x = botx-rangeC-1; x <= botWidth+1; x++) {
        for (double y = boty-rangeC-1; y <= botHeight+1; y++) {
            double range = Util::distanceBetweenPoints(x, y, botx, boty, botWidth, botHeight),
                   angle = Util::angleBetweenPoints(x, y, botx, boty, botWidth, botHeight);

            if (x == botx && y == boty) {
                this->assign(x, y, 0);
            } else if (range <= rangeC) {
                bool atRange = false;
                if (atRange == false && this->at(x, y)) this->assign(x, y, -1);
            } else if (this->at(x, y)) this->assign(x, y, -1);
        }
    }*/
}

/**
 * void Mapping::calculateMapBayes()
{
    cout << "Calculando o mapa pelo metodo de Bayes" << endl;

    float variacao = 100;//10 centimetros
    float pMax = 0.90;
    double aberturaSonar = 20.0;

    for(int x=0; x<MAP_LENGTH_WORLD;x++)
    {
        for(int y=0; y<MAP_LENGTH_WORLD;y++)
        {
            double angle = round(atan2(
                                  ((float)MAP_LENGTH_WORLD/2 - y + 0.5 )*celRange-yRobo,
                                  ((float)x + 0.5 - MAP_LENGTH_WORLD/2)*celRange-xRobo
                                  )*180/M_PI)-thRobo;
            float distance = sqrt(
                        pow(
                            (x + 0.5 - MAP_LENGTH_WORLD/2)*celRange - xRobo,
                            2
                            )
                        +pow(
                            (MAP_LENGTH_WORLD/2 - y + 0.5 )*celRange - yRobo,
                            2
                            )
                        );

            if(angle < -180)
                angle += 360;
            if(angle > 180)
                angle -= 360;

            for(int i =0;i<sonares->size();i++)
            {

                float angleSensor = sonares->at(i).getSensorTh();

                if(fabs(angle-angleSensor) <= aberturaSonar/2)
                {
                    float reading = sonares->at(i).getRange();
                    double pOcupada, pVazia;
                    pOcupada = pVazia = 0.5;

                    if(reading > rangeMax)
                    {
                        reading = rangeMax;
                    }
                    if(reading + variacao < distance)
                    {
                        //cout << "Área desconhecida... " << endl;
                    }
                    else if(reading == rangeMax)
                    {
                        pVazia = pMax;
                        pOcupada = 1.0-pVazia;
                    }
                    else if(reading - variacao > distance)
                    {
                        //cout << "Área vaga... " << endl;
                        pVazia = 0.5*((2*rangeMax-distance)/(2*rangeMax) + (aberturaSonar-fabs(angle-angleSensor))/aberturaSonar);
                        pVazia = max(pVazia, 1.0-pMax);
                        pOcupada = 1.0 - pVazia;

                    }
                    else if((reading - variacao <= distance) && (reading + variacao >= distance))
                    {
                        //cout << "Parede... " << endl;
                        pOcupada = 0.5*pMax*((2*rangeMax-distance)/(2*rangeMax) + (aberturaSonar-fabs(angle-angleSensor))/aberturaSonar);
                        //pVazia = max(pOcupada, 1.0-pMax);
                        pVazia = 1.0 - pOcupada;

                    }
                    mapCell[x][y].setProbabilidadeOcupada(
                                pOcupada*mapCell[x][y].probabilidadeOcupada()
                                /
                                (pOcupada*mapCell[x][y].probabilidadeOcupada() + pVazia*mapCell[x][y].probabilidadeVazia())
                                );

                    if(mapCell[x][y].probabilidadeVazia() == 0.0)
                        cout << "ZERO!!!!" << endl;
                    break;
                }
            }
        }
    }

}*/


void OccupancyGrid::updateWithHistogramic(Bot *bot)
{
    double botx = round(bot->getX()/cellScale),
           boty = round(bot->getY()/cellScale),
           botth = bot->getTh(),
           angleSonar, rangeSensor;

    vector<ArSensorReading> sensor = bot->getSonar();

    double rangeMax = 5000/cellScale,
           botWidth = (botx+rangeMax > this->getWidth()/2 ? this->getWidth()/2 : botx+rangeMax),
           botHeight = (boty+rangeMax > this->getHeight()/2 ? this->getHeight()/2 : boty+rangeMax);

    for (double x = botx-rangeMax-1; x <= botWidth+1; x++) {
        for (double y = boty-rangeMax-1; y <= botHeight+1; y++) {
            if (x == botx && y == boty) {
                this->assign(x, y, 0);
            } else {
                if (this->at(x, y)) this->assign(x, y, -1);
            }
        }
    }

    for (int i = 0; i < sensor.size(); i++) {
        angleSonar = sensor.at(i).getSensorTh()*-1;
        rangeSensor = sensor.at(i).getRange()/cellScale;

        int x = floor(rangeSensor*cos((botth-angleSonar)*M_PI/180)),
            y = floor(rangeSensor*sin((botth-angleSonar)*M_PI/180));

        this->assign(x+botx, y+boty, i+1);
        if (!this->at(x+botx, y+boty)->getHistogramic()) this->at(x+botx, y+boty)->setHistogramic();
        if (rangeSensor < rangeMax) this->at(x+botx, y+boty)->getHistogramic()->addCV();

        double rangeX = x,
               rangeY = y,
               m = max(fabs(rangeX), fabs(rangeY)),
               xStep = x/m,
               yStep = y/m;

        for (int n = 0; n < m; n++) {
            int nX = n*xStep;
            int nY = n*yStep;
            nX += botx;
            nY += boty;
            if (!(nX == botx && nY == boty)) {
                this->assign(nX, nY, i+1.5);
                if (!this->at(nX, nY)->getHistogramic()) this->at(nX, nY)->setHistogramic();
                this->at(nX, nY)->getHistogramic()->subCV();
            }
        }
    }
}

OccupancyGridCell::OccupancyGridCell(double sensorId) :
    QObject() 
{ 
    this->sensorId = sensorId;
    bayesian = NULL;
    histogramic = NULL;
    scanTime = NULL;
    changed = false;
}
 
void OccupancyGridCell::setSensorId(double sensorId)
{ 
    changed = (changed ? changed : (this->sensorId != sensorId));
    this->sensorId = sensorId;
} 
 
double OccupancyGridCell::getSensorId()
{ 
    return sensorId;
}

bool OccupancyGridCell::isChanged()
{
    return changed;
}

void OccupancyGridCell::setRegion(int region)
{
    changed = (changed ? changed : (this->region != region));
    this->region = region;
}

void OccupancyGridCell::setScanTime()
{
    scanTime = new QTime();
    scanTime->start();
}

bool OccupancyGridCell::isTimeToScanAgain()
{
    if (scanTime) {
        QTime now;
        now.start();
        //std::cout << "eita " << scanTime->secsTo(now) << std::endl;
        if (scanTime->secsTo(now) >= 5) {
            scanTime = NULL;
            return true;
        }
        return false;
    }
    return true;
}

Bayesian *OccupancyGridCell::getBayesian()
{
    return bayesian;
}

void OccupancyGridCell::bayesianProbability(double r, double R, double alpha, double beta)
{
    if (!bayesian) bayesian = new Bayesian(0.5, 0.5);
    if (changed && bayesian->getOccupied() > 0.7) this->setScanTime();
    if (this->isTimeToScanAgain()) {
        if (region == 1) bayesian->setOccupiedRead(0.5*((R-r)/R + (beta-alpha)/beta)*0.98);
        else if (region == 2) bayesian->setEmptyRead(0.5*((R-r)/R + (beta-alpha)/beta)*0.98);
        else bayesian->setEmptyRead(Bayesian::max);
        bayesian->calculate();
    }
}


Histogramic *OccupancyGridCell::getHistogramic()
{
    return histogramic;
}

void OccupancyGridCell::setHistogramic()
{
    histogramic = new Histogramic();
}
