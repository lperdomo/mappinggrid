#include "controller.h" 
#include "bot.h" 
 
#include <iostream> 
 
Controller::Controller(double width, double height, double cellSize, double cellScale) : 
    QObject() 
{ 
    grid = new OccupancyGrid(width, height, cellSize, cellScale); 
    scene = new SceneGrid((width/2)*cellSize*-1, (height/2)*cellSize*-1, width*cellSize, height*cellSize, grid);
    view = new QGraphicsView(scene); 
    thread = new QThread();
    threadB = new QThread();
} 
 
Controller::~Controller() 
{ 
    delete bot; 
    delete view;
    delete scene; 
    delete grid; 
    delete thread;
    delete threadB;
}
 
void Controller::setBot(Bot *bot) 
{ 
    this->bot = bot; 
    this->grid->setBot(bot);
    this->grid->connect(this->bot, SIGNAL(reading()), grid, SLOT(doOccupancyGrid()));
    this->connect(this->bot, SIGNAL(reading()), this, SLOT(update()));
    this->bot->connect(thread, SIGNAL(started()), this->bot, SLOT(doExploration()));
    //this->bot->connect(thread, SIGNAL(started()), this->bot, SLOT(doTeleOp()));
    //this->bot->connect(thread, SIGNAL(started()), this->bot, SLOT(doWallFollowing())); 
    this->bot->moveToThread(thread);
} 
 
void Controller::run() 
{ 
    thread->start();
    threadB->start();
} 
 
void Controller::update() 
{
    this->updateBot();
    this->showView();
} 
 
void Controller::showView() 
{ 
    if (!view->isVisible()) { 
        view->show(); 
    } else { 
        //view->centerOn(round(bot->getX()/grid->getCellScale())*grid->getCellSize(), round(bot->getY()/grid->getCellScale())*grid->getCellSize()*-1);
        view->viewport()->update();

        //std::cout << "x " << round(bot->getX()/grid->getCellScale())*grid->getCellSize() << " y " << round(bot->getY()/grid->getCellScale())*grid->getCellSize()*-1 << std::endl;
    } 
} 

void Controller::updateBot()
{
    double botx = round(bot->getX()/grid->getCellScale()),
           boty = round(bot->getY()/grid->getCellScale()),
           botth = bot->getTh();
    double potential = grid->at(botx, boty)->getPotentialField()->getPotential();
    double degree = fmod((grid->at(botx, boty)->getPotentialField()->getTh()-botth+180+360), 360)-180;
    double rad = degree*(M_PI/180);

    //if (potential < 1) {
        if (degree <= 90 && degree >= -90) {
            bot->setLeftWheel(50-50*sin(rad));
            bot->setRightWheel(50+50*sin(rad));
        } else {
            bot->setLeftWheel(10-20*sin(rad));
            bot->setRightWheel(10+20*sin(rad));
        }
    //} else {
    //    bot->setLeftWheel(0);
    //    bot->setRightWheel(0);
    //}
}
