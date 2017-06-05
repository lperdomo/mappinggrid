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
        //PotentialField *potentialField = new PotentialField();
        matrix[x][y] = new OccupancyGridCell(sensorId);
    } else { 
        matrix[x][y]->setSensorId(sensorId);
    } 
}

void OccupancyGrid::reset(double botx, double boty)
{
    double rangeMax = 5000/cellScale,
           botWidth = (botx+rangeMax > width/2 ? width/2 : botx+rangeMax),
           botHeight = (boty+rangeMax > height/2 ? height/2 : boty+rangeMax);

    for (double x = botx-rangeMax-2; x <= botWidth+2; x++) {
        for (double y = boty-rangeMax-2; y <= botHeight+2; y++) {
            if (x == botx && y == boty) {
                this->assign(x, y, 0);
            } else {
                if (this->at(x, y)) this->assign(x, y, -1);
            }
        }
    }
}

void OccupancyGrid::updateWithBayesian(Bot *bot)
{
    vector<ArSensorReading> sensor = bot->getSonar();

    double rangeMax = 5000/cellScale,
           tolerance = 100/cellScale,
           botx = round(bot->getX()/cellScale),
           boty = round(bot->getY()/cellScale),
           botth = bot->getTh();

    this->reset(botx, boty);

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
            if (angleSonar-15 > sensor.at(i-1).getSensorTh()+15) angleStart = angleSonar-10;
            else if (angleSonar-15 < sensor.at(i-1).getSensorTh()+15) angleStart = angleSonar-10;
            if (angleSonar+15 > sensor.at(i-1).getSensorTh()-15) angleEnd = angleSonar+10;
            else if (angleSonar+15 < sensor.at(i-1).getSensorTh()-15) angleEnd = angleSonar+10;
        }

        for (double angle = angleStart; angle <= angleEnd; angle+=0.1) {
            int x = round(rangeMax*cos((botth-angle)*M_PI/180)),
                y = round(rangeMax*sin((botth-angle)*M_PI/180));

            double cellMax = max(fabs(x), fabs(y)),
                   stepX = x/cellMax,
                   stepY = y/cellMax;

            for (int cell = 0; cell <= cellMax; cell++) {
                int cellX = cell*stepX+botx,
                    cellY = cell*stepY+boty;
                double range = Util::distanceBetweenPoints(cellX, cellY, botx, boty, width, height);

                if (!(cellX == botx && cellY == boty)) {
                    if (rangeSensor >= rangeMax) {
                        this->assign(cellX, cellY, i+1.5);
                        this->at(cellX, cellY)->setRegion(3);
                        this->at(cellX, cellY)->bayesianProbability(range, rangeMax, fabs(angle-angleSonar), 15);
                    } else if (rangeA < range) {
                        this->assign(cellX, cellY, i+1.5);
                        this->at(cellX, cellY)->setRegion(3);
                    } else if (rangeB <= range && range <= rangeA) {
                        this->assign(cellX, cellY, i+1);
                        this->at(cellX, cellY)->setRegion(1);
                        this->at(cellX, cellY)->bayesianProbability(range, rangeMax, fabs(angle-angleSonar), 15);
                    } else if (range < rangeB) {
                        this->assign(cellX, cellY, i+1.5);
                        this->at(cellX, cellY)->setRegion(2);
                        this->at(cellX, cellY)->bayesianProbability(range, rangeMax, fabs(angle-angleSonar), 15);
                    }
                }
            }
        }
    }
}

void OccupancyGrid::updateWithHistogramic(Bot *bot)
{
    vector<ArSensorReading> sensor = bot->getSonar();

    double rangeMax = 5000/cellScale,
           botx = round(bot->getX()/cellScale),
           boty = round(bot->getY()/cellScale),
           botth = bot->getTh(),
           angleSonar, rangeSensor;

    this->reset(botx, boty);

    for (int i = 0; i < sensor.size(); i++) {
        angleSonar = sensor.at(i).getSensorTh()*-1;
        rangeSensor = sensor.at(i).getRange()/cellScale;

        int x = round(rangeSensor*cos((botth-angleSonar)*M_PI/180)),
            y = round(rangeSensor*sin((botth-angleSonar)*M_PI/180));

        this->assign(x+botx, y+boty, i+1);
        if (!this->at(x+botx, y+boty)->getHistogramic()) this->at(x+botx, y+boty)->setHistogramic();
        if (rangeSensor < rangeMax) {
            this->at(x+botx, y+boty)->getHistogramic()->addCV();
            int GRO = this->at(x+botx, y+boty)->getHistogramic()->getCV();
            for (int j = x-1; j < x+1; j++) {
                for (int k = y-1; k < y+1; k++) {
                    if (!(j == x && k == y) && this->at(j+botx, k+boty)) {
                        if (this->at(j+botx, k+boty)->getHistogramic()) {
                            GRO += 0.5*this->at(j+botx, k+boty)->getHistogramic()->getCV();
                        }
                    }
                }
            }
            GRO = min(Histogramic::max, GRO);
            //std::cout << "GRO" << GRO << std::endl;
            this->at(x+botx, y+boty)->getHistogramic()->setCV(GRO);
        }

        double cellMax = max(fabs(x), fabs(y)),
               stepX = x/cellMax,
               stepY = y/cellMax;

        for (int cell = 0; cell < cellMax; cell++) {
            int cellX = cell*stepX+botx;
            int cellY = cell*stepY+boty;
            if (!(cellX == botx && cellY == boty)) {
                this->assign(cellX, cellY, i+1.5);
                if (!this->at(cellX, cellY)->getHistogramic()) this->at(cellX, cellY)->setHistogramic();
                this->at(cellX, cellY)->getHistogramic()->subCV();
            }
        }
    }
}

void OccupancyGrid::updatePotentialFields(Bot *bot)
{
    double limitx = width/2,
           limity = height/2,
           error = 1, errorMax = 0.1;

    for (double x = limitx*-1; x < limitx; x++)  {
        for (double y = limity*-1; y < limity; y++) {
            if (fabs(x) == fabs(limitx) || fabs(y) == fabs(limity)) {
                this->assign(x, y, -1);
                this->at(x, y)->getPotentialField()->setPotential(PotentialField::obstacle);
                this->at(x, y)->getPotentialField()->setTh(0);
            } else if (this->at(x, y)) {
                if (this->at(x, y)->getBayesian() || this->at(x, y)->getHistogramic()) {
                    if (this->at(x, y)->getBayesian()) {
                        if (this->at(x, y)->getBayesian()->getOccupied() > 0.5) {
                            this->at(x, y)->getPotentialField()->setPotential(PotentialField::obstacle);
                            this->at(x, y)->getPotentialField()->setTh(0);
                        }
                    } else if (this->at(x, y)->getHistogramic()) {
                        if (this->at(x, y)->getHistogramic()->getCV() >= 3) {
                            this->at(x, y)->getPotentialField()->setPotential(PotentialField::obstacle);
                            this->at(x, y)->getPotentialField()->setTh(0);
                        }
                    }
                }
            }
        }
    }

    double rangeMax = 5000/cellScale,
           botx = bot->getX()/cellScale,
           boty = bot->getY()/cellScale,
           botWidth = (botx+rangeMax > width/2 ? width/2 : botx+rangeMax),
           botHeight = (boty+rangeMax > height/2 ? height/2 : boty+rangeMax);

    double left = limitx*-1, right = limitx, top = limity*-1, bottom = limity;
    //double left = botWidth*-1, right = botWidth, top = botHeight*-1, bottom = botHeight;

    //while (error > errorMax) {
        error = 0;
        //std::cout << "error" << error << std::endl;
        for (double x = left; x < right; x++)  {
            for (double y = top; y < bottom; y++) {
                this->calculatePotential(x, y);
                if (this->at(x, y)) {
                    if (this->at(x, y)->getPotentialField()) {
                        //std::cout << "parcial " << this->at(x, y)->getPotentialField()->getError() << std::endl;
                    }
                }
            }
        }
        for (double x = right; x > left; x--)  {
            for (double y = top; y < bottom; y++) {
                this->calculatePotential(x, y);
                if (this->at(x, y)) {
                    if (this->at(x, y)->getPotentialField()) {
                        //std::cout << "parcial " << this->at(x, y)->getPotentialField()->getError() << std::endl;
                    }
                }
            }
        }
        for (double x = left; x < right; x++)  {
            for (double y = bottom; y > top; y--) {
                this->calculatePotential(x, y);
                if (this->at(x, y)) {
                    if (this->at(x, y)->getPotentialField()) {
                        //std::cout << "parcial " << this->at(x, y)->getPotentialField()->getError() << std::endl;
                    }
                }
            }
        }
        for (double x = right; x > left; x--)  {
            for (double y = bottom; y > top; y--) {
                this->calculatePotential(x, y);
                if (this->at(x, y)) {
                    if (this->at(x, y)->getPotentialField()) {
                        //std::cout << "parcial " << this->at(x, y)->getPotentialField()->getError() << std::endl;
                        error += this->at(x, y)->getPotentialField()->getError();
                    }
                }
            }
        }
        //std::cout << "error" << error << std::endl;
    //}
}

void OccupancyGrid::calculatePotential(double x, double y)
{
    double dx, dy;
    double limitx = width/2,
           limity = height/2,
           gauss, p1, p2, p3, p4;
    if (this->at(x, y)) {
        if (this->at(x, y)->getBayesian() || this->at(x, y)->getHistogramic()) {
            if (this->at(x, y)->getPotentialField()->getPotential() != PotentialField::obstacle) {
                p1 = PotentialField::target;
                if (this->at(x+1, y)) {
                    if (this->at(x+1, y)->getPotentialField()) {
                        p1 = this->at(x+1, y)->getPotentialField()->getPotential();
                    }
                }
                p2 = PotentialField::target;
                if (this->at(x-1, y)) {
                    if (this->at(x-1, y)->getPotentialField()) {
                        p2 = this->at(x-1, y)->getPotentialField()->getPotential();
                    }
                }
                p3 = PotentialField::target;
                if (this->at(x, y+1)) {
                    if (this->at(x, y+1)->getPotentialField()) {
                        p3 = this->at(x, y+1)->getPotentialField()->getPotential();
                    }
                }
                p4 = PotentialField::target;
                if (this->at(x, y-1)) {
                    if (this->at(x, y-1)->getPotentialField()) {
                        p4 = this->at(x, y-1)->getPotentialField()->getPotential();
                    }
                }

                gauss = (p1+p2+p3+p4)/4.0;

                double th = atan2(-(p3-p4),-(p1-p2))*180/M_PI;
                this->at(x, y)->getPotentialField()->setError(pow(this->at(x, y)->getPotentialField()->getPotential()-gauss, 2));
                this->at(x, y)->getPotentialField()->setPotential(gauss);
                this->at(x, y)->getPotentialField()->setTh(th);
            }
        }
    }
}

OccupancyGridCell::OccupancyGridCell(double sensorId) :
    QObject() 
{ 
    this->sensorId = sensorId;
    potentialField = new PotentialField();
    bayesian = NULL;
    histogramic = NULL;
    scanTime = NULL;
    changed = false;
}
 
void OccupancyGridCell::setSensorId(double sensorId)
{ 
    if (!changed) this->sensorId != sensorId;
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
    if (changed && bayesian->getOccupied() > 0.9) this->setScanTime();
    if (this->isTimeToScanAgain()) {
        if (region == 1) bayesian->setOccupiedRead(0.5*((R-r)/R + (beta-alpha)/beta)*0.98);
        else if (region == 2) bayesian->setEmptyRead(0.5*((R-r)/2*R + (beta-alpha)/beta)*0.98);//range division multiplied, less false positives in region B
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

PotentialField *OccupancyGridCell::getPotentialField()
{
    return potentialField;
}

void OccupancyGridCell::updatePotentialField(double potential, double th)
{
    double value;
    if (this->getBayesian()) {
        value = this->getBayesian()->getOccupied();
    } else if (this->getHistogramic()) {
        value = this->getHistogramic()->getCV();
        if (value >= 6) {
            potentialField->setPotential(PotentialField::obstacle);
            potentialField->setTh(-1);
        } else {
            potentialField->setPotential(potential);
            potentialField->setTh(th);
        }
    }

}
