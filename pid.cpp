#include "pid.h" 
 
PID::PID(double kProbability, double kDerivative, double kIntegral, double target) 
{ 
    this->kProbability = kProbability; 
    this->kDerivative = kDerivative; 
    this->kIntegral = kIntegral; 
    this->target = target; 
} 
 
double PID::calculate(double closestReading, double timeInterval) 
{ 
    double error = target - closestReading; 
    integralSum += error * timeInterval; 
    double derivative = (error-errorPrevious)/timeInterval; 
    errorPrevious = error; 
    return kProbability*error + kIntegral*integralSum + kDerivative*derivative; 
} 
