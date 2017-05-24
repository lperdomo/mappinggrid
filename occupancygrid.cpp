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

OccupancyGridCell::OccupancyGridCell(double sensorId) :
    QObject() 
{ 
    this->sensorId = sensorId;
    bayes = new Bayes(0.5, 0.5);
}
 
void OccupancyGridCell::setSensorId(double sensorId)
{ 
    this->sensorId = sensorId;
} 
 
double OccupancyGridCell::getSensorId()
{ 
    return sensorId;
}

Bayes *OccupancyGridCell::getBayes()
{
    return bayes;
}
