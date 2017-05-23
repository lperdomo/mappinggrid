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
    this->updateBotOnScene();
    this->showView(); 
} 
 
void Controller::updateBotOnScene()
{ 
    scene->bot->setXY(bot->getX(), bot->getY());
    scene->bot->setTh(bot->getTh());
} 
 
void Controller::updateOccupancyGrid()
{
    double size = grid->getCellSize(),
           scale = grid->getCellScale(),
           distanceLimit = 5*size;
    double botx = round(bot->getX()/scale),
           boty = round(bot->getY()/scale),
           botth = bot->getTh(),
           botWidth = (botx+distanceLimit > grid->getWidth()/2 ? grid->getWidth()/2 : botx+distanceLimit),
           botHeight = (boty+distanceLimit > grid->getHeight()/2 ? grid->getHeight()/2 : boty+distanceLimit);

    double rangeMax = 5000,
           probMax = 0.9,
           variance = 100;
    for (double x = botx-distanceLimit; x <= botWidth; x++) {
        for (double y = boty-distanceLimit; y <= botHeight; y++) {
            double distance = sqrt(pow((x - botWidth) - (botx - botWidth), 2)
                                 + pow((botHeight - y) - (botHeight - boty), 2));
            double angle = round(atan2((botHeight - y) - (botHeight - boty)
                                     , (x - botWidth) - (botx - botWidth))*180/M_PI);
            std::cout << "asssssssssssssssss" << bot->getSonar()->size() << std::endl;

            for (int i = 0; i < bot->getSonar()->size(); i++) {
                double angleSonar = bot->getSonar()->at(i).getSensorTh();
                std::cout << "sssss" << std::endl;
                if (fabs(angle-angleSonar) <= 30) {
                    double range = bot->getSonar()->at(i).getRange(),
                           empty = 0.5,
                           occupied = 0.5;
                    if (range == rangeMax) {
                        empty = probMax;
                        occupied = 1-empty;
                    } else if (range - variance > distance) {
                        empty = max(0.5*((2*rangeMax-distance)/(2*rangeMax) + (30-fabs(angle-angleSonar))/30), 1-probMax);
                        occupied = 1-empty;
                    } else if (range - variance <= distance && range + variance >= distance) {
                        occupied = (max(0.5*((2*rangeMax-distance)/(2*rangeMax) + (30-fabs(angle-angleSonar))/30), 1-probMax));
                        empty = 1-occupied;
                    }
                    occupied = occupied*grid->at(x, y)->getBayes()->getOccupied()
                            /(grid->at(x, y)->getBayes()->getOccupied()
                              + empty*grid->at(x, y)->getBayes()->getEmpty());
                    std::cout << "eita" << std::endl;
                    grid->assign(x, y, 0);
                    grid->at(x, y)->getBayes()->setOccupied(occupied);

                }
            }
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
