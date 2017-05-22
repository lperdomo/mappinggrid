#ifndef PID_H 
#define PID_H 
 
class PID 
{ 
public: 
    PID(double kProbability, double kDerivative, double kIntegral, double target); 
    double calculate(double closestReading, double timeInterval); 
private: 
    double kProbability; 
    double kDerivative; 
    double kIntegral; 
    double integralSum; 
    double errorPrevious; 
    double target; 
}; 
 
#endif // PID_H 
