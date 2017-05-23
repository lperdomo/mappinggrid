#include "scenegrid.h" 
#include "keyboard.h" 
 
#include <math.h> 
 
#include <iostream> 
 
SceneGridBot::SceneGridBot()
{ 
} 
 
void SceneGridBot::setXY(double x, double y)
{ 
    this->x = x; 
    this->y = y; 
} 
 
void SceneGridBot::setTh(double th)
{ 
    this->th = th;
} 
 
double SceneGridBot::getX()
{ 
    return x; 
} 
 
double SceneGridBot::getY()
{ 
    return y; 
} 
 
double SceneGridBot::getTh()
{ 
    return th*-1;
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
                //double value = grid->at(x, y)->getBayes()->getOccupied();
                std::cout << value << std::endl;
                painter->setPen(QPen(Qt::lightGray));
                painter->setBrush(QBrush(Qt::white));
                painter->drawRect(size*x, size*y*-1, size, size);

                if (value == 1) drawColoredRect(painter, x, y, QColor(255, 255, 0, 127));
                else if (value == 2) drawColoredRect(painter, x, y, QColor(0, 255, 255, 127));
                else if (value == 3) drawColoredRect(painter, x, y, QColor(255, 0, 255, 127));
                else if (value == 4) drawColoredRect(painter, x, y, QColor(255, 255, 0, 127));
                else if (value == 5) drawColoredRect(painter, x, y, QColor(0, 255, 255, 127));
                else if (value == 6) drawColoredRect(painter, x, y, QColor(255, 0, 255, 127));
                else if (value == 7) drawColoredRect(painter, x, y, QColor(255, 255, 0, 127));
                else if (value == 8) drawColoredRect(painter, x, y, QColor(0, 255, 255, 127));
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
    bot = new SceneGridBot();
    gridItem = new SceneGridItem(grid); 
    this->addItem(gridItem);
} 
 
SceneGrid::~SceneGrid() 
{ 
    delete bot;
    delete gridItem; 
} 

void SceneGrid::drawBackground(QPainter *painter, const QRectF &rect)
{
    //painter->setPen(QPen(Qt::lightGray));
    //painter->setBrush(QBrush(Qt::lightGray));
    //painter->drawRect(rect.left(), rect.top(), rect.right()*2, rect.bottom()*2);
}

void SceneGrid::drawForeground(QPainter *painter, const QRectF &rect) 
{ 
    drawBot(painter, rect);
    drawGrid(painter, rect);
} 

void SceneGrid::drawGrid(QPainter *painter, const QRectF &rect)
{
    double size = gridItem->grid->getCellSize();
    qreal x, y;
    qreal left = int(rect.left()) - (int(rect.left()) % (int(size)));
    qreal top = int(rect.top()) - (int(rect.top()) % (int(size)));

    /*QVarLengthArray<QLineF, 100> lines;

    painter->setPen(QPen(Qt::darkGray));
    for (x = left; x < rect.right(); x += size)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (y = top; y < rect.bottom(); y += size)
        lines.append(QLineF(rect.left(), y, rect.right(), y));
    painter->drawLines(lines.data(), lines.size());*/

    painter->setPen(QPen(Qt::black));
    painter->drawLine(QLineF(0, rect.top(), 0, rect.bottom()));
    painter->drawLine(QLineF(rect.left(), 0, rect.right(), 0));
}

void SceneGrid::drawBot(QPainter *painter, const QRectF &rect)
{
    /*double size = gridItem->grid->getCellSize(),
           scale = gridItem->grid->getCellScale(),
           distanceLimit = 5*size;
    double botx = round(bot->getX()/scale),
           boty = round(bot->getY()/scale),
           botth = bot->getTh(),
           botWidth = (botx+distanceLimit > rect.right() ? rect.right() : botx+distanceLimit),
           botHeight = (boty+distanceLimit > rect.bottom() ? rect.bottom() : boty+distanceLimit);

    for (qreal x = botx-distanceLimit; x <= botWidth; x++) {
        for (qreal y = boty-distanceLimit; y <= botHeight; y++) {
            double distance = sqrt(pow((x - rect.right()) - (botx - rect.right()), 2)
                                 + pow((rect.bottom() - y) - (rect.bottom() - boty), 2));
            double angle = round(atan2((rect.bottom() - y) - (rect.bottom() - boty)
                                     , (x - rect.right()) - (botx - rect.right()))*180/M_PI);
            if (distance <= distanceLimit) {
                if (x == botx && y == boty)
                    drawColoredRect(painter, x, y, Qt::red);
                else if (Util::isAngleAtRange(botth+90, angle, 15))
                    drawColoredRect(painter, x, y, QColor(255, 255, 0, 127));
                else if (Util::isAngleAtRange(botth+50, angle, 15) && !Util::isAngleAtRange(botth+30, angle, 10))
                    drawColoredRect(painter, x, y, QColor(0, 255, 255, 127));
                else if (Util::isAngleAtRange(botth+40, angle, 0.5))
                    drawColoredRect(painter, x, y, QColor(0, 0, 255, 127));
                else if (Util::isAngleAtRange(botth+30, angle, 15) && !Util::isAngleAtRange(botth+10, angle, 10))
                    drawColoredRect(painter, x, y, QColor(255, 0, 255, 127));
                else if (Util::isAngleAtRange(botth+20, angle, 0.5))
                    drawColoredRect(painter, x, y, QColor(255, 0, 0, 127));
                else if (Util::isAngleAtRange(botth+10, angle, 15) && !Util::isAngleAtRange(botth-10, angle, 10))
                    drawColoredRect(painter, x, y, QColor(255, 255, 0, 127));
                else if (Util::isAngleAtRange(botth, angle, 0.5))
                    drawColoredRect(painter, x, y, QColor(0, 255, 0, 127));
                else if (Util::isAngleAtRange(botth-10, angle, 15) && !Util::isAngleAtRange(botth-30, angle, 10))
                    drawColoredRect(painter, x, y, QColor(0, 255, 255, 127));
                else if (Util::isAngleAtRange(botth-20, angle, 0.5))
                    drawColoredRect(painter, x, y, QColor(0, 0, 255, 127));
                else if (Util::isAngleAtRange(botth-30, angle, 15) && !Util::isAngleAtRange(botth-50, angle, 10))
                    drawColoredRect(painter, x, y, QColor(255, 0, 255, 127));
                else if (Util::isAngleAtRange(botth-40, angle, 0.5))
                    drawColoredRect(painter, x, y, QColor(255, 0, 0, 127));
                else if (Util::isAngleAtRange(botth-50, angle, 15))
                    drawColoredRect(painter, x, y, QColor(255, 255, 0, 127));
                else if (Util::isAngleAtRange(botth-90, angle, 15))
                    drawColoredRect(painter, x, y, QColor(0, 255, 255, 127));
            }
        }
    }*/
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
