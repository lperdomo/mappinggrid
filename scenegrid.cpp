#include "scenegrid.h" 
#include "keyboard.h" 
 
#include <math.h> 
 
#include <iostream> 
 
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
                double occupied = 0;
                if (grid->at(x, y)->getBayesian() || grid->at(x, y)->getHistogramic()) {
                    if (grid->at(x, y)->getBayesian()) occupied = grid->at(x, y)->getBayesian()->getOccupied();
                    if (grid->at(x, y)->getHistogramic()) occupied = grid->at(x, y)->getHistogramic()->proportionalCV();
                    painter->setPen(QPen(QColor(220, 220, 220)));
                    int cor = 255-(255*occupied);
                    if (cor >= 255) cor = 254;
                    else if (cor <= 0) cor = 1;
                    painter->setBrush(QBrush(QColor(cor, cor, cor, 255)));
                    painter->drawRect(size*x, size*y*-1, size, size);
                    if (size >= 8) {
                        if (grid->at(x, y)->getPotentialField()) {
                            double cx = size*x+(size/2), cy = size*y-(size/2),
                                   nx = cx+((size/3)*(cos(grid->at(x, y)->getPotentialField()->getTh()*M_PI/180))),
                                   ny = cy+((size/3)*(sin(grid->at(x, y)->getPotentialField()->getTh()*M_PI/180)));
                            if (grid->at(x, y)->getPotentialField()->getPotential() != PotentialField::obstacle) {
                                painter->setPen(QPen(Qt::black));
                                painter->setBrush(QBrush(Qt::black));
                                painter->drawLine(cx, cy*-1, nx, ny*-1);
                                painter->drawRect(nx, ny*-1, 1, 1);

                                /*
                                int core = 255-(255*grid->at(x, y)->getPotentialField()->getPotential());
                                if (core >= 255) core = 254;
                                else if (core <= 0) core = 1;
                                painter->setBrush(QBrush(QColor(0, cor, 0, 255)));
                                painter->drawRect(size*x, size*y*-1, size, size);
                                painter->drawText(size*x, cy*-1, QString::number(grid->at(x, y)->getPotentialField()->getPotential()));
                                */
                            } else {
                                /*
                                painter->setBrush(QBrush(QColor(0, 0, 0, 255)));
                                painter->drawRect(size*x, size*y*-1, size, size);
                                painter->setBrush(QBrush(QColor(255, 255, 255, 255)));
                                painter->setPen(QPen(Qt::white));
                                painter->drawText(size*x, cy*-1, QString::number(grid->at(x, y)->getPotentialField()->getPotential()));
                                */
                            }
                        }
                    }
                }

                double sensorId = grid->at(x, y)->getSensorId();
                if (sensorId == 0) drawColoredRect(painter, x, y, Qt::red);
                else if (sensorId == 1) drawColoredRect(painter, x, y, QColor(255, 255, 0, 155));
                else if (sensorId == 1.5) drawColoredRect(painter, x, y, QColor(200, 200, 0, 75));
                else if (sensorId == 2) drawColoredRect(painter, x, y, QColor(0, 255, 255, 155));
                else if (sensorId == 2.5) drawColoredRect(painter, x, y, QColor(0, 200, 200, 75));
                else if (sensorId == 3) drawColoredRect(painter, x, y, QColor(255, 0, 255, 155));
                else if (sensorId == 3.5) drawColoredRect(painter, x, y, QColor(200, 0, 200, 75));
                else if (sensorId == 4) drawColoredRect(painter, x, y, QColor(255, 255, 0, 155));
                else if (sensorId == 4.5) drawColoredRect(painter, x, y, QColor(200, 200, 0, 75));
                else if (sensorId == 5) drawColoredRect(painter, x, y, QColor(0, 255, 255, 155));
                else if (sensorId == 5.5) drawColoredRect(painter, x, y, QColor(0, 200, 200, 75));
                else if (sensorId == 6) drawColoredRect(painter, x, y, QColor(255, 0, 255, 155));
                else if (sensorId == 6.5) drawColoredRect(painter, x, y, QColor(200, 0, 200, 75));
                else if (sensorId == 7) drawColoredRect(painter, x, y, QColor(255, 255, 0, 155));
                else if (sensorId == 7.5) drawColoredRect(painter, x, y, QColor(200, 200, 0, 75));
                else if (sensorId == 8) drawColoredRect(painter, x, y, QColor(0, 255, 255, 155));
                else if (sensorId == 8.5) drawColoredRect(painter, x, y, QColor(0, 200, 200, 75));
            }
        } 
    } 
}

void SceneGridItem::drawColoredRect(QPainter *painter, double x, double y, QColor color)
{
    double size = grid->getCellSize();
    painter->setPen(QPen(QColor(255, 255, 255, 1)));
    painter->setBrush(QBrush(color));
    painter->drawRect(size*x, size*y*-1, size, size);
}
 
SceneGrid::SceneGrid(qreal x, qreal y, qreal width, qreal height, OccupancyGrid *grid) : 
    QGraphicsScene(x, y, width, height) 
{ 
    gridItem = new SceneGridItem(grid); 
    this->addItem(gridItem);
} 
 
SceneGrid::~SceneGrid() 
{ 
    delete gridItem; 
} 

void SceneGrid::drawForeground(QPainter *painter, const QRectF &rect) 
{ 
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
