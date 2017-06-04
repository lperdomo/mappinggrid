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
    //PotentialField::min = cellScale;
    //PotentialField::max = cellScale/cos(M_PI*45.0/180.0);
    //PotentialField::obstacle = width*height*PotentialField::max*2;
    PotentialField::target = 0;
    PotentialField::obstacle = 1;

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

void OccupancyGrid::reset(double botx, double boty)
{
    double rangeMax = 5000/cellScale,
           botWidth = (botx+rangeMax > width/2 ? width/2 : botx+rangeMax),
           botHeight = (boty+rangeMax > height/2 ? height/2 : boty+rangeMax);

    for (double x = botx-rangeMax-2; x <= botWidth+2; x++) {
        for (double y = boty-rangeMax-1; y <= botHeight+1; y++) {
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
           tolerance = 200/cellScale,
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

void OccupancyGrid::updatePotentialFields()
{
    double limitx = width/2,
           limity = height/2,
           error = 1, errorMax = 0.004;

    for (double x = limitx*-1; x < limitx; x++)  {
        for (double y = limity*-1; y < limity; y++) {
            if (this->at(x, y)) {
                if (this->at(x, y)->getBayesian() || this->at(x, y)->getHistogramic()) {
                    if (this->at(x, y)->getBayesian()) {
                        if (this->at(x, y)->getBayesian()->getOccupied() > 0.5) {
                            this->at(x, y)->getPotentialField()->setPotential(PotentialField::obstacle);
                            this->at(x, y)->getPotentialField()->setTh(0);
                        }
                    } else if (this->at(x, y)->getHistogramic()) {
                        if (this->at(x, y)->getHistogramic()->getCV() >= 4) {
                            this->at(x, y)->getPotentialField()->setPotential(PotentialField::obstacle);
                            this->at(x, y)->getPotentialField()->setTh(0);
                        }
                    }
                }
            }
        }
    }


    //while (error > errorMax) {
        error = 0;
        //std::cout << "error" << error << std::endl;
        for (double x = limitx*-1; x < limitx; x++)  {
            for (double y = limity*-1; y < limity; y++) {
                this->calculatePotential(x, y);
            }
        }
        for (double x = limitx; x > limitx*-1; x--)  {
            for (double y = limity*-1; y < limity; y++) {
                this->calculatePotential(x, y);
            }
        }
        for (double x = limitx*-1; x < limitx; x++)  {
            for (double y = limity; y > limity*-1; y--) {
                this->calculatePotential(x, y);
            }
        }
        for (double x = limitx; x > limitx*-1; x--)  {
            for (double y = limity; y > limity*-1; y--) {
                this->calculatePotential(x, y);
                if (this->at(x, y)) {
                    //if (this->at(x, y)->getPotentialField())
                        //error += this->at(x, y)->getPotentialField()->getError();
                }
            }
        }
        //std::cout << "error" << error << std::endl;
    //}
}

void OccupancyGrid::calculatePotential(double x, double y)
{
    double dx, dy;
    if (this->at(x, y)) {
        if (this->at(x, y)->getBayesian() || this->at(x, y)->getHistogramic()) {
            double limitx = width/2,
                   limity = height/2,
                   gauss, p1, p2, p3, p4;
            if (x+1 <= limitx) {
                p1 = PotentialField::target;
                if (this->at(x+1, y)) {
                    if (this->at(x+1, y)->getPotentialField()) {
                        p1 = this->at(x+1, y)->getPotentialField()->getPotential();
                    }
                }
            } else {
                p1 = PotentialField::obstacle;
            }
            if (x-1 >= limitx*-1) {
                p2 = PotentialField::target;
                if (this->at(x-1, y)) {
                    if (this->at(x-1, y)->getPotentialField()) {
                        p2 = this->at(x-1, y)->getPotentialField()->getPotential();
                    }
                }
            } else {
                p2 = PotentialField::obstacle;
            }
            if (y+1 <= limity) {
                p3 = PotentialField::target;
                if (this->at(x, y+1)) {
                    if (this->at(x, y+1)->getPotentialField()) {
                        p3 = this->at(x, y+1)->getPotentialField()->getPotential();
                    }
                }
            } else {
                p3 = PotentialField::obstacle;
            }
            if (y-1 >= limity*-1) {
                p4 = PotentialField::target;
                if (this->at(x, y-1)) {
                    if (this->at(x, y-1)->getPotentialField()) {
                        p4 = this->at(x, y-1)->getPotentialField()->getPotential();
                    }
                }
            } else {
                p4 = PotentialField::obstacle;
            }

            gauss = (p1+p2+p3+p4)*0.4;
            dx = (p1-p2)/2;
            dy = (p4-p3)/2;
            if (fabs(dy) < fabs(dx)) {
                dx = 0;
            } else {
                dy = 0;
            }
            //double th = atan2(dy, dx)*180/M_PI;
            double th = atan2(-(p3-p4),-(p2-p1))*180/M_PI;
            //this->at(x, y)->getPotentialField()->setError(pow(this->at(x, y)->getPotentialField()->getPotential()-gauss, 2));
            this->at(x, y)->getPotentialField()->setPotential(gauss);
            this->at(x, y)->getPotentialField()->setTh(th);
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

        if (region == 2 && bayesian->getOccupied() > 0.5) {
        std::cout << "rangeSensor" << r << " maximo" << R << " alpha" << alpha << " beta" << beta << std::endl;
        std::cout << "prob " << 0.5*((R-r)/R + (beta-alpha)/beta) << std::endl;
        std::cout << "result" << bayesian->getOccupied() << std::endl;
        }

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
