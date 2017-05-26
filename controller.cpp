#include "controller.h" 
#include "bot.h" 
 
#include <iostream> 
 
Controller::Controller(double width, double height, double cellSize, double cellScale) : 
    QObject() 
{ 
    grid = new OccupancyGrid(width, height, cellSize, cellScale); 
    scene = new SceneGrid((width/2)*-1, (height/2)*-1, width, height, grid); 
    view = new QGraphicsView(scene); 
    thread = new QThread(); 
} 
 
Controller::~Controller() 
{ 
    delete bot; 
    delete view; 
    delete scene; 
    delete grid; 
    delete thread; 
} 
 
void Controller::setBot(Bot *bot) 
{ 
    this->bot = bot; 
    this->connect(this->bot, SIGNAL(moving()), this, SLOT(update())); 
    this->bot->connect(thread, SIGNAL(started()), this->bot, SLOT(doTeleOp())); 
    //this->bot->connect(thread, SIGNAL(started()), this->bot, SLOT(doWallFollowing())); 
    this->bot->moveToThread(thread); 
} 
 
void Controller::run() 
{ 
    thread->start(); 
} 
 
void Controller::update() 
{
    this->updateOccupancyGrid();
    this->showView(); 
} 
 
void Controller::updateOccupancyGrid()
{
    double size = grid->getCellSize(),
           scale = grid->getCellScale(),
           rangeMax = 5000,
           rangeA = 2500,
           rangeB = rangeA+1000,
           distanceLimit = rangeMax/scale;
    double botx = round(bot->getX()/scale),
           boty = round(bot->getY()/scale),
           botth = bot->getTh(),
           botWidth = (botx+distanceLimit > grid->getWidth()/2 ? grid->getWidth()/2 : botx+distanceLimit),
           botHeight = (boty+distanceLimit > grid->getHeight()/2 ? grid->getHeight()/2 : boty+distanceLimit);

    double probMax = 0.9999,
           probMin = 0.0001,
           probRangeB = 2,
           tolerance = 200;

    for (double x = botx-distanceLimit-1; x <= botWidth+1; x++) {
        for (double y = boty-distanceLimit-1; y <= botHeight+1; y++) {

            double distance = sqrt(pow((x - botWidth) - (botx - botWidth), 2)
                                 + pow((botHeight - y) - (botHeight - boty), 2));
            double angle = atan2((botHeight - y) - (botHeight - boty)
                                     , (x - botWidth) - (botx - botWidth))*180/M_PI;//+botth

            if (x == botx && y == boty) {
                grid->assign(x, y, 0);
            } else if (distance <= distanceLimit) {
                bool inRange = false;
                for (int i = 0; (i < bot->getSonar()->size() && inRange == false); i++) {
                    if (bot->isCloseToSonarRange(angle, i)) {
                        double angleSonar = bot->getSonar()->at(i).getSensorTh()*-1,
                               range = bot->getSonar()->at(i).getRange(),
                               empty,
                               occupied;
                        //std::cout << "angle" << angle << " distance" << distance << std::endl;
                        /*if (range/scale >= (rangeA-tolerance)/scale && range/scale <= (rangeB+tolerance)/scale) {
                            std::cout << "range >= rangeA && range <= rangeB" << std::endl;
                            occupied = 0.5*((distanceLimit-distance)/distanceLimit + (15-angle/15))*0.98;
                            empty = 1-occupied;
                        if (range/scale < rangeA/scale) {
                            //std::cout << "range <= rangeA" << std::endl;
                            empty = 0.5*((distanceLimit-distance)/distanceLimit + (15-angle/15));
                            empty = (empty > 1 ? 1 : empty);
                            occupied = 1-empty;*/
                        //if (distance) {
                        //    std::cout << "i" << i << " range" << range << "range/scale" << range/scale << " distance" << distance << std::endl;
                        //    empty = 0.5;
                        //    occupied = 1-empty;
                        //} else if ((range-tolerance)/scale > distance) {
                        //    empty = 0.5*((distanceLimit-distance)/distanceLimit + (15-angle/15));
                        //    empty = (empty > 1 ? 1 : empty);
                        //    occupied = 1-empty;
                        //} else if ((range-tolerance)/scale <= distance && (range+tolerance)/scale >= distance) {
                        //    occupied = 0.5*((distanceLimit-distance)/distanceLimit + (15-angle/15))*0.98;
                        //    empty = 1-occupied;
                        //} else {
                            //std::cout << "range > rangeA" << std::endl;
                            //empty = probMax;
                            //occupied = 1-empty;
                        //}
                            //occupied = 1-empty;
                        //} else if (range - variance <= distance && range + variance >= distance) {
                            //occupied = (max(0.5*((2*rangeMax-distance)/(2*rangeMax) + (15-fabs(angle-angleSonar))/15), 1-probMax));
                            //empty = 1-occupied;
                        //}
                        empty = probMax;
                        occupied = 1-empty;
                        bool teste = false;
                        if (distance >= (rangeA-tolerance)/scale && distance <= (rangeB-tolerance)/scale) {
                            grid->assign(x, y, i+1);
                            if (range/scale >= (rangeA-tolerance)/scale && range/scale <= (rangeB+tolerance)/scale) {
                                occupied = (0.5*((distanceLimit-distance)/distanceLimit + ((15-fabs((angle+botth)-angleSonar))/15)))*0.98;
                                std::cout << "distanceLimit=" << distanceLimit
                                          << " distance" << distance
                                          << " abs((angle+botth)-angleSonar)" << fabs((angle+botth)-angleSonar) << std::endl;
                                empty = 1-occupied;
                                teste = true;
                                //occupied = 0.9;
                                //empty = 1-occupied;
                            }
                        } else if (distance < rangeA/scale) {
                            grid->assign(x, y, i+1.5);
                            //std::cout << "range" << range << " distance" << distance << " range/scale" << range/scale << " rangeA/scale" << rangeA/scale << std::endl;
                            /*if (range/scale < rangeA/scale) {
                                empty = 0.5*((distanceLimit-distance)/distanceLimit + (15-angle/15))*0.98;
                                empty = (empty > 1 ? 1 : empty);
                                occupied = 1-empty;
                            } else {
                                empty = probMax;
                                occupied = 1-empty;
                            }*/
                        } else {
                            grid->assign(x, y, i+1.5);
                            //empty = probMax;
                            //occupied = 1-empty;
                        }
                        if (teste == true) std::cout
                                << "occupied=" << occupied << "empty" << empty
                                << " grid->at(x, y)->getBayes()->getOccupied()" << grid->at(x, y)->getBayes()->getOccupied()
                                << " grid->at(x, y)->getBayes()->getEmpty()" << grid->at(x, y)->getBayes()->getEmpty()
                                << " result occupied=" << occupied << " empty" << empty << std::endl;

                        occupied = occupied*grid->at(x, y)->getBayes()->getOccupied()
                                /(occupied*grid->at(x, y)->getBayes()->getOccupied()
                                  + empty*grid->at(x, y)->getBayes()->getEmpty());
                        if (occupied > probMax) occupied = probMax;
                        else if (occupied < probMin) occupied = probMin;
                        empty = 1-occupied;

                        if (teste == true) std::cout
                                << "result occupied=" << occupied << "empty" << empty << std::endl;
                        /*if (occupied > 0.5) {
                            std::cout << "occupied" << occupied << std::endl;
                            std::cout << "prevoccupied" << grid->at(x, y)->getBayes()->getOccupied() << std::endl;
                            std::cout << "empty" << empty << std::endl;
                            std::cout << "prevempty" << grid->at(x, y)->getBayes()->getEmpty() << std::endl;

                        }*/
                        if (grid->at(x, y)->getBayes()->isTimeToScanAgain()) {
                            grid->at(x, y)->getBayes()->setOccupied(occupied);
                        }
                        //std::cout << "TESTE" << grid->at(x, y)->getBayes()->getOccupied() << std::endl;
                        //        /(grid->at(x, y)->getBayes()->getOccupied()
                        //          + empty*grid->at(x, y)->getBayes()->getEmpty());
                        //grid->at(x, y)->getBayes()->setOccupied(occupied);
                        inRange = true;
                    }
                }
                if (inRange == false && grid->at(x, y)) grid->assign(x, y, -1);
            } else if (grid->at(x, y)) grid->assign(x, y, -1);
        }
    }
}


void Controller::showView() 
{ 
    if (!view->isVisible()) { 
        view->show(); 
    } else { 
        view->viewport()->update(); 
    } 
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
