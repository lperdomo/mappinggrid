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
 
void OccupancyGrid::assign(double x, double y, double value) 
{ 
    x = x + (width/2); 
    y = (height/2) - y; 
    if (!matrix[x][y]) { 
        matrix[x][y] = new OccupancyGridCell(value); 
    } else { 
        matrix[x][y]->setValue(value); 
    } 
} 
 
bool OccupancyGrid::isAngleAtRange(double angle1, double angle2, double range) 
{ 
    int difference = angle1 - angle2; 
    if (abs(difference % 360) <= range || 360 - abs(difference % 360) <= range) { 
        return true; 
    } 
    return false; 
} 
 
OccupancyGridCell::OccupancyGridCell(double value) : 
    QObject() 
{ 
    this->value = value; 
} 
 
void OccupancyGridCell::setValue(double value) 
{ 
    this->value = value; 
} 
 
double OccupancyGridCell::getValue() 
{ 
    return value; 
} 
 
 
/** 
 * void CellMap::setProbabilidadeOcupada(double value) 
{ 
    if(value >0 && value <1.0) 
    { 
        pOcupada = value; 
        pVazia = 1.0-pOcupada; 
    } 
} 
 
void CellMap::setProbabilidadeVazia(double value) 
{ 
    if(value >0 && value <1.0) 
    { 
        pVazia = value; 
        pOcupada = 1.0-pVazia; 
    } 
}*/ 
