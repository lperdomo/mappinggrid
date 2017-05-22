#ifndef KEYBOARD_H 
#define KEYBOARD_H 
 
class Keyboard 
{ 
public: 
    Keyboard(); 
    ~Keyboard(); 
    void setArrowUp(bool pressed); 
    bool isArrowUp(); 
    void setArrowDown(bool pressed); 
    bool isArrowDown(); 
    void setArrowLeft(bool pressed); 
    bool isArrowLeft(); 
    void setArrowRight(bool pressed); 
    bool isArrowRight(); 
    static Keyboard *getInstance(); 
    static Keyboard *instance; 
private: 
    bool arrowUp; 
    bool arrowDown; 
    bool arrowLeft; 
    bool arrowRight; 
}; 
 
#endif // KEYBOARD_H 
