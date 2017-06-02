#include "potentialfield.h"

double PotentialField::min = 0;
double PotentialField::max = 0;
double PotentialField::obstacle = 0;

PotentialField::PotentialField()
{
    potential = -PotentialField::obstacle;
}

void PotentialField::setPotential(double potential)
{
    this->potential = potential;
}

double PotentialField::getPotential()
{
    return potential;
}

















































/**
 *
#include "campopotencial.h"

CampoPotencial::CampoPotencial(Mapping *map, int mapSize): QThread(), IRenderMap()
{
    angleAndar = 0.0;
    mMapping = map;
    mMapSize = mapSize;
    celRange = 100;
    Dmin = celRange;
    Dmax = celRange/cos(M_PI*45.0/180.0);
    Dobstacle = mMapSize*mMapSize*Dmax*2;
    mPotencial = new double*[mMapSize];
    for(int x=0;x<mMapSize;x++)
    {
        mPotencial[x] = new double[mMapSize];
        for(int y=0;y<mMapSize;y++)
        {
            mPotencial[x][y] = -Dobstacle;
        }
    }
}

void CampoPotencial::run()
{
    gettimeofday(&begin,NULL);
    time = true;
    bool relaxa = true;
    float m = 0.0;
    while(true)
    {

        //qDebug() << "vamos computar!";
        if(relaxa)
        {
            m = ComputaDistancia();
            //cout << m << endl;
            //if(m<40000000)
             //   relaxa = false;
        }


        if((time) && (!relaxa))
        {
            gettimeofday(&end,NULL);

            qDebug() << "Grafo criado!";
            qDebug() << "Tempo criacao: " << ((double)(end.tv_usec-begin.tv_usec))/1000000 + (double)(end.tv_sec-begin.tv_sec);
            time = false;
        }
    }
    qDebug() << "saindo!";
}

float CampoPotencial::ComputaDistancia()
{

    float ret =0;
    //cout << "computando... " << endl;
    for(int x=1;x<mMapSize-1;x++)
    {
        for(int y=0;y<mMapSize;y++)
        {
            //qDebug() << x << y << mMapSize << mMap[x][y] ;
            float value;
            //value = mMapping->mapCell[x][y].isHimmProbability();
            //value = mMapping->mapCell[x][y].isBayesProbability();
            value = mMapping->mapCell[x][y].cellValue();
            if(value == 0.0)
            {
                mPotencial[x][y] = Dobstacle;
            }
            /*
            else if(value == 0.7)
            {
                //mPotencial[x][y] = -Dobstacle;
                double n = (mPotencial[x-1][y]+mPotencial[x+1][y]+mPotencial[x][y-1]+mPotencial[x][y+1])/4.0;
                ret += pow(n-mPotencial[x][y],2);
                mPotencial[x][y] = n;
            }*/
/*            else// if(value == 1.0)
            {
                double n = (mPotencial[x-1][y]+mPotencial[x+1][y]+mPotencial[x][y-1]+mPotencial[x][y+1])/4.0;
                ret += pow(n-mPotencial[x][y],2);
                mPotencial[x][y] = n;
            }
        }
    }
    return ret;
}

void CampoPotencial::MoveRobot()
{

}

double CampoPotencial::distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}*/
