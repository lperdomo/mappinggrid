#include "scenegrid.h" 
#include "keyboard.h" 
 
#include <math.h> 
 
#include <iostream> 
 
SceneGridPoint::SceneGridPoint() 
{ 
} 
 
void SceneGridPoint::setXY(double x, double y) 
{ 
    this->x = x; 
    this->y = y; 
} 
 
void SceneGridPoint::setTh(double th) 
{ 
    this->th = th; 
} 
 
double SceneGridPoint::getX() 
{ 
    return x; 
} 
 
double SceneGridPoint::getY() 
{ 
    return y; 
} 
 
double SceneGridPoint::getTh() 
{ 
    return th; 
} 
 
SceneGridItem::SceneGridItem(OccupancyGrid *grid) : 
    QGraphicsItem() 
{ 
    this->grid = grid; 
} 
 
SceneGridItem::~SceneGridItem() 
{ 
} 
 
QRectF SceneGridItem::boundingRect() const 
{ 
    return QRectF((grid->getWidth()/2)*-1, (grid->getHeight()/2)*-1, grid->getWidth()/2, grid->getHeight()/2); 
} 
 
void SceneGridItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) 
{ 
    QRectF rect = boundingRect(); 
    double size = grid->getCellSize(); 
    for (double x = rect.left(); x < rect.width(); x++) { 
        for (double y = rect.top(); y < rect.height(); y++) { 
            if (grid->at(x, y)) { 
                double value = grid->at(x, y)->getValue(); 
                if (value == 1) painter->setBrush(QBrush(Qt::red)); 
                else if (value == 2) painter->setBrush(QBrush(QColor(255, 255, 0, 127))); 
                else if (value == 3) painter->setBrush(QBrush(QColor(0, 255, 255, 127))); 
                else if (value == 4) painter->setBrush(QBrush(QColor(0, 0, 255, 127))); 
                else if (value == 5) painter->setBrush(QBrush(QColor(255, 0, 255, 127))); 
                else if (value == 6) painter->setBrush(QBrush(QColor(255, 0, 0, 127))); 
                else if (value == 7) painter->setBrush(QBrush(QColor(255, 255, 0, 127))); 
                else if (value == 8) painter->setBrush(QBrush(QColor(0, 255, 0, 127))); 
                else if (value == 9) painter->setBrush(QBrush(QColor(0, 255, 255, 127))); 
                else if (value == 10) painter->setBrush(QBrush(QColor(0, 0, 255, 127))); 
                else if (value == 11) painter->setBrush(QBrush(QColor(255, 0, 255, 127))); 
                else if (value == 12) painter->setBrush(QBrush(QColor(255, 0, 0, 127))); 
                else if (value == 13) painter->setBrush(QBrush(QColor(255, 255, 0, 127))); 
                else if (value == 14) painter->setBrush(QBrush(QColor(0, 255, 255, 127))); 
                else if (value == 0) painter->setBrush(QBrush(Qt::white)); 
                painter->drawRect(size*x, size*y*-1, size, size); 
            } 
        } 
    } 
} 
 
SceneGrid::SceneGrid(qreal x, qreal y, qreal width, qreal height, OccupancyGrid *grid) : 
    QGraphicsScene(x, y, width, height) 
{ 
    observer = new SceneGridPoint(); 
    gridItem = new SceneGridItem(grid); 
    this->addItem(gridItem);     
} 
 
SceneGrid::~SceneGrid() 
{ 
    delete observer; 
    delete gridItem; 
} 
 
void SceneGrid::drawForeground(QPainter *painter, const QRectF &rect) 
{ 
    double size = gridItem->grid->getCellSize(); 
    qreal x, y; 
    qreal left = int(rect.left()) - (int(rect.left()) % (int(size))); 
    qreal top = int(rect.top()) - (int(rect.top()) % (int(size))); 
 
    double botx = round(observer->getX()/gridItem->grid->getCellScale()), 
           boty = round(observer->getY()/gridItem->grid->getCellScale()), 
           botth = observer->getTh()*-1; 
    double limitx = (botx+125 > (gridItem->grid->getWidth()/2) ? (gridItem->grid->getWidth()/2)-botx : botx+125), 
           limity = (botx+125 > (gridItem->grid->getHeight()/2) ? (gridItem->grid->getHeight()/2)-botx : botx+125), 
           beginx = (limitx)*-1, 
           beginy = (limity)*-1; 
 
    for (double x = beginx; x < limitx; x++) { 
        for (double y = beginy; y < limity; y++) { 
            double distance = sqrt(pow((x - limitx) - (botx - limitx), 2) + pow((limity - y) - (limity - boty), 2)); 
            double angle = round(atan2((limity - y) - (limity - boty), (x - limitx) - (botx - limitx))*180/M_PI); 
 
            if (x == botx && y == boty) { 
                painter->setBrush(QBrush(Qt::red)); 
            } else if (distance <= 25) { 
                //std::cout << "bot->getTh()" << bot->getTh() << std::endl; 
                if (gridItem->grid->isAngleAtRange(botth+90, angle, 15)) { 
                    painter->setBrush(QBrush(QColor(255, 255, 0, 127))); 
                } else if (gridItem->grid->isAngleAtRange(botth+50, angle, 15) && !gridItem->grid->isAngleAtRange(botth+30, angle, 10)) { 
                    painter->setBrush(QBrush(QColor(0, 255, 255, 127))); 
                } else if (gridItem->grid->isAngleAtRange(botth+40, angle, 0.5)) { 
                    painter->setBrush(QBrush(QColor(0, 0, 255, 127))); 
                } else if (gridItem->grid->isAngleAtRange(botth+30, angle, 15) && !gridItem->grid->isAngleAtRange(botth+10, angle, 10)) { 
                    painter->setBrush(QBrush(QColor(255, 0, 255, 127))); 
                } else if (gridItem->grid->isAngleAtRange(botth+20, angle, 0.5)) { 
                    painter->setBrush(QBrush(QColor(255, 0, 0, 127))); 
                } else if (gridItem->grid->isAngleAtRange(botth+10, angle, 15) && !gridItem->grid->isAngleAtRange(botth-10, angle, 10)) { 
                    painter->setBrush(QBrush(QColor(255, 255, 0, 127))); 
                } else if (gridItem->grid->isAngleAtRange(botth, angle, 0.5)) { 
                    painter->setBrush(QBrush(QColor(0, 255, 0, 127))); 
                } else if (gridItem->grid->isAngleAtRange(botth-10, angle, 15) && !gridItem->grid->isAngleAtRange(botth-30, angle, 10)) { 
                    painter->setBrush(QBrush(QColor(0, 255, 255, 127))); 
                } else if (gridItem->grid->isAngleAtRange(botth-20, angle, 0.5)) { 
                    painter->setBrush(QBrush(QColor(0, 0, 255, 127))); 
                } else if (gridItem->grid->isAngleAtRange(botth-30, angle, 15) && !gridItem->grid->isAngleAtRange(botth-50, angle, 10)) { 
                    painter->setBrush(QBrush(QColor(255, 0, 255, 127))); 
                } else if (gridItem->grid->isAngleAtRange(botth-40, angle, 0.5)) { 
                    painter->setBrush(QBrush(QColor(255, 0, 0, 127))); 
                } else if (gridItem->grid->isAngleAtRange(botth-50, angle, 15)) { 
                    painter->setBrush(QBrush(QColor(255, 255, 0, 127))); 
                } else if (gridItem->grid->isAngleAtRange(botth-90, angle, 15)) { 
                    painter->setBrush(QBrush(QColor(0, 255, 255, 127))); 
                } else { 
                    painter->setBrush(QBrush(Qt::white)); 
                } 
            } else { 
                painter->setBrush(QBrush(Qt::white)); 
            } 
            painter->drawRect(size*x, size*y*-1, size, size); 
        } 
    } 
 
 
    QVarLengthArray<QLineF, 100> lines; 
 
    painter->setPen(QPen(Qt::darkGray)); 
    for (x = left; x < rect.right(); x += size) 
        lines.append(QLineF(x, rect.top(), x, rect.bottom())); 
    for (y = top; y < rect.bottom(); y += size) 
        lines.append(QLineF(rect.left(), y, rect.right(), y)); 
 
    painter->drawLines(lines.data(), lines.size()); 
 
    painter->setPen(QPen(Qt::black)); 
    painter->drawLine(QLineF(0, rect.top(), 0, rect.bottom())); 
    painter->drawLine(QLineF(rect.left(), 0, rect.right(), 0)); 
} 
 
void SceneGrid::keyPressEvent(QKeyEvent *event) 
{ 
    if (event->key() == Qt::Key_Up) { 
        Keyboard::getInstance()->setArrowUp(true); 
    } else if (event->key() == Qt::Key_Down) { 
        Keyboard::getInstance()->setArrowDown(true); 
    } else if (event->key() == Qt::Key_Left) { 
        Keyboard::getInstance()->setArrowLeft(true); 
    } else if (event->key() == Qt::Key_Right) { 
        Keyboard::getInstance()->setArrowRight(true); 
    } 
} 
 
void SceneGrid::keyReleaseEvent(QKeyEvent *event) 
{ 
    if (event->key() == Qt::Key_Up) { 
        Keyboard::getInstance()->setArrowUp(false); 
    } else if (event->key() == Qt::Key_Down) { 
        Keyboard::getInstance()->setArrowDown(false); 
    } else if (event->key() == Qt::Key_Left) { 
        Keyboard::getInstance()->setArrowLeft(false); 
    } else if (event->key() == Qt::Key_Right) { 
        Keyboard::getInstance()->setArrowRight(false); 
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
